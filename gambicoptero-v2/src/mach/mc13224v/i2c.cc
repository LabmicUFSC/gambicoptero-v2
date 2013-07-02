// EPOS MC13224V_I2C Mediator Implementation

#include <mach/mc13224v/i2c.h>
#include <cpu.h>
#include <machine.h>
#include <gpio_pin.h>

__BEGIN_SYS


MC13224V_I2C::MC13224V_I2C()
 : _scl(I2C_SCL_GPIO, GPIO_Pin::FUNC_ALT1),
   _sda(I2C_SDA_GPIO, GPIO_Pin::FUNC_ALT1),
   _sync_timeout(0)
{
    db<MC13224V_I2C>(TRC) << "MC13224V_I2C::MC13224V_I2C()\n";

    CPU::out32(Machine::IO::GPIO_PAD_PU_EN0, // disable internal pull-up resistors
                CPU::in32(Machine::IO::GPIO_PAD_PU_EN0)
                & ~(1 << I2C_SCL_GPIO)
                & ~(1 << I2C_SDA_GPIO)
        );

    enable();
}

MC13224V_I2C::~MC13224V_I2C()
{
    db<MC13224V_I2C>(TRC) << "MC13224V_I2C::~MC13224V_I2C()\n";

    disable();
}

void MC13224V_I2C::enable()
{
    db<MC13224V_I2C>(TRC) << "void MC13224V_I2C::enable()\n";

    CPU::out8(Machine::IO::I2C_CKER, 1 << I2C_CKER_CKEN); // enable clock signal to i2c module
    CPU::out8(Machine::IO::I2C_FDR, _freq_div); // setup clock prescaler (for SCL)
    CPU::out8(Machine::IO::I2C_CR,
                CPU::in8(Machine::IO::I2C_CR)
                | I2C_CR_MEN // enable I2C module
        );
}

void MC13224V_I2C::disable()
{
    db<MC13224V_I2C>(TRC) << "void MC13224V_I2C::disable()\n";

    CPU::out8(Machine::IO::I2C_CR, 0x00); // disable everything
    CPU::out8(Machine::IO::I2C_CKER, 0); // disable clock signal to i2c module
}

bool MC13224V_I2C::busy()
{
    db<MC13224V_I2C>(TRC) << "bool MC13224V_I2C::busy()\n";

    return ((CPU::in8(Machine::IO::I2C_SR) & (1 << I2C_SR_MBB)) > 0);
}

void MC13224V_I2C::set_timeout(TSC::Time_Stamp tm) {
    _sync_timeout = tm;
}

bool MC13224V_I2C::sync()
{
    if(_sync_timeout == 0)
        while(!(CPU::in8(Machine::IO::I2C_SR) & (1 << I2C_SR_MCF)));
    else {
        TSC::Time_Stamp begin = TSC::time_stamp();
        TSC::Time_Stamp target = TSC::time_stamp() + _sync_timeout;
        TSC::Time_Stamp now;
        while(!(CPU::in8(Machine::IO::I2C_SR) & (1 << I2C_SR_MCF)))
        {
            now = TSC::time_stamp();
            if(now > target || now < begin) // if timed-out or overflowed
            {
                db<MC13224V_I2C>(ERR) << "I2C error: Sync timeout! Will reset hardware.\n";
                disable();
                CPU::busy_wait_10us(10);
                enable();
                return false;
            }
        }
    }
    return true;
}

void MC13224V_I2C::start()
{
    CPU::out8(Machine::IO::I2C_CR,
                (1 << I2C_CR_MEN)
              | (1 << I2C_CR_MSTA)
              | (1 << I2C_CR_MTX)
          );
}

void MC13224V_I2C::restart()
{
    CPU::out8(Machine::IO::I2C_CR,
              CPU::in8(Machine::IO::I2C_CR)
              | (1 << I2C_CR_RSTA)
          );
}

void MC13224V_I2C::receive()
{
    CPU::out8(Machine::IO::I2C_CR,
              CPU::in8(Machine::IO::I2C_CR)
              & ~(1 << I2C_CR_MTX)
          );
}

void MC13224V_I2C::nack()
{
    // Will NACK last byte to shut-up the slave.
    CPU::out8(Machine::IO::I2C_CR,
              CPU::in8(Machine::IO::I2C_CR)
            | (1 << I2C_CR_TXAK)
        );
}

void MC13224V_I2C::stop()
{
    CPU::out8(Machine::IO::I2C_CR,
               CPU::in8(Machine::IO::I2C_CR)
               & ~(1 << I2C_CR_MSTA)
        );
}

bool MC13224V_I2C::put_byte(unsigned char b)
{
    db<MC13224V_I2C>(TRC) << "bool MC13224V_I2C::put_byte(" << b << ")\n";

    bool success = true;

    CPU::out8(Machine::IO::I2C_DR, b);

    CPU::busy_wait_10us(4); // => not documented => hardware bug? ACK bit won't be cleared before testing if doesn't wait
    success = sync(); // wait until byte is sent

    if (CPU::in8(Machine::IO::I2C_SR) & (1 << I2C_SR_RXAK)) {
        db<MC13224V_I2C>(ERR) << "I2C error: RXACK missed when putting "<< b << ".\n";
        success = false;
    }

    if (CPU::in8(Machine::IO::I2C_SR) & (1 << I2C_SR_MAL)) {
        CPU::out8(Machine::IO::I2C_SR,
                    CPU::in8(Machine::IO::I2C_SR)
                    & ~(1 << I2C_SR_MAL)
            );
        db<MC13224V_I2C>(ERR) << "I2C error: Arbitration lost.\n";
        success = false;
    }

    return success;
}

bool MC13224V_I2C::get_byte(unsigned char * b, bool set_nack)
{
    db<MC13224V_I2C>(TRC) << "bool MC13224V_I2C::get_byte(...)\n";

    bool success = true;

    // wait until byte is received
    success = sync();

    if (set_nack) nack();

    *b = CPU::in8(Machine::IO::I2C_DR);

    if (CPU::in8(Machine::IO::I2C_SR) & (1 << I2C_SR_MAL)) {
        CPU::out8(Machine::IO::I2C_SR,
                    CPU::in8(Machine::IO::I2C_SR)
                    & ~(1 << I2C_SR_MAL)
            );
        db<MC13224V_I2C>(ERR) << "I2C error: Arbitration lost.\n";
        success = false;
    }

    return success;
}


bool MC13224V_I2C::write_register(unsigned char dst, unsigned char reg, unsigned char byte) {
    unsigned char buf[1];
    buf[0] = byte;
    return this->write_register(dst, reg, 1, buf);
}

bool MC13224V_I2C::write_register(
        unsigned char dst,
        unsigned char reg,
        int len,
        unsigned char * buff)
{
    db<MC13224V_I2C>(TRC) << "void MC13224V_I2C::write_register("
            << dst << ", "
            << reg << ", "
            << len << ", "
            << (void*) buff << ")\n";

    while(busy()); // wait for bus activity to end.

    bool success = true;

    // assuming i'm master
    start(); // generate START condition
    success = put_byte((dst & 0x7f) << 1); // send slave address
    success = put_byte(reg); // send register number
    for(int i = 0; i < len; i++)
        success = put_byte(buff[i]); // send data byte
    success = sync(); // wait until byte last operation finishes
    stop();// generate STOP condition

    return success;
}

bool MC13224V_I2C::read_register(
        unsigned char dst,
        unsigned char reg,
        int len,
        unsigned char * buff)
{
    db<MC13224V_I2C>(TRC) << "void MC13224V_I2C::read_register("
            << dst << ", "
            << reg << ", "
            << len << ", "
            << (void*) buff << ")\n";

    while(busy()); // wait for bus activity to end.

    bool success = true;

    // assuming i'm master
    start(); // generate START condition
    success = put_byte((dst << 1) & ~0x01); // send slave address
    success = put_byte(reg); // send register number
    restart(); // generate RESTART condition
    success = put_byte((dst << 1) | 0x01); // send slave address requesting data
    receive(); // to receive mode

    // discard address in I2C_DR => not documented => hardware bug?
    success = get_byte(&buff[0]);
    if (len == 1) nack(); // last byte received is not ack'd to shut-up receiver
    success = sync();

    bool set_nack = false;
    for(int i = 0; i < len; i++) {
        if (i == (len - 2)) // if next-to-last byte being received
            set_nack = true;
        success = get_byte(&buff[i], set_nack); // receive data
    }

    success = sync(); // wait until byte last operation finishes
    stop(); // generate STOP condition

    return success;
}

__END_SYS
