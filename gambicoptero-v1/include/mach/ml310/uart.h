// EPOS ML310 UART Mediator Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __ml310_uart_h
#define __ml310_uart_h

#include <uart.h>
#include <cpu.h>
#include <ic.h>

__BEGIN_SYS

class ML310_UART: protected UART_Common
{
private:
    typedef CPU::Reg8 Reg8;
    static const unsigned int CLOCK = Traits<ML310_UART>::CLOCK / 16; //Verify !!!

    // ML310_UART private imports, types and constants
    /* Directly Accessed Memory Mapped Input/Output Registers */
    /* 16450/16550 compatible UART, register offsets as byte registers */
    enum {
        UART_RBR_REG_OFFSET     = 0x1003,       /* receive buffer, read only */
        UART_THR_REG_OFFSET     = 0x1003,       /* transmit holding register */
        UART_IER_REG_OFFSET     = 0x1007,       /* interrupt enable */
        UART_IIR_REG_OFFSET     = 0x100B,       /* interrupt id, read only */
        UART_FCR_REG_OFFSET     = 0x100B,       /* fifo control, write only */
        UART_LCR_REG_OFFSET     = 0x100F,       /* line control register */
        UART_MCR_REG_OFFSET     = 0x1013,       /* modem control register */
        UART_LSR_REG_OFFSET     = 0x1017,       /* line status register */
        UART_MSR_REG_OFFSET     = 0x101B,       /* modem status register */
        UART_DRLS_REG_OFFSET    = 0x1003,       /* divisor register LSB */
        UART_DRLM_REG_OFFSET    = 0x1007,       /* divisor register MSB */
        /* Interrupt Enable Register bits */
        IER_MODEM_STATUS        = 0x08,         /* modem status interrupt */
        IER_RX_LINE             = 0x04,         /* receive status interrupt */
        IER_TX_EMPTY            = 0x02,         /* transmitter empty interrupt */
        IER_RX_DATA             = 0x01,         /* receiver data available */
        /* Interrupt ID Register bits */
        INT_ID_MASK             = 0x0F,         /* only the interrupt ID */
        INT_ID_FIFOS_ENABLED    = 0xC0,         /* only the FIFOs enable */
        /* FIFO Control Register bits */
        FIFO_RX_TRIG_MSB        = 0x80,         /* trigger level MSB */
        FIFO_RX_TRIG_LSB        = 0x40,         /* trigger level LSB */
        FIFO_TX_RESET           = 0x04,         /* reset the transmit FIFO */
        FIFO_RX_RESET           = 0x02,         /* reset the receive FIFO */
        FIFO_ENABLE             = 0x01,         /* enable the FIFOs */
        FIFO_RX_TRIGGER         = 0xC0,         /* both trigger level bits */
        /* Line Control Register bits */
        LCR_DLAB                = 0x80,         /* divisor latch access */
        LCR_SET_BREAK           = 0x40,         /* cause a break condition */
        LCR_STICK_PARITY        = 0x20,
        LCR_EVEN_PARITY         = 0x10,         /* 1 = even, 0 = odd parity */
        LCR_ENABLE_PARITY       = 0x08,
        LCR_2_STOP_BITS         = 0x04,         /* 1 = 2 stop bits,0 = 1 stop bit */
        LCR_8_DATA_BITS         = 0x03,
        LCR_7_DATA_BITS         = 0x02,
        LCR_6_DATA_BITS         = 0x01,
        LCR_LENGTH_MASK         = 0x03,         /* both length bits mask */
        LCR_PARITY_MASK         = 0x18,         /* both parity bits mask */
        /* Modem Control Register bits */
        MCR_LOOP                = 0x10,         /* local loopback */
        MCR_OUT_2               = 0x08,         /* general output 2 signal */
        MCR_OUT_1               = 0x04,         /* general output 1 signal */
        MCR_RTS                 = 0x02,         /* RTS signal */
        MCR_DTR                 = 0x01,         /* DTR signal */
        /* Line Status Register bits */
        LSR_RX_FIFO_ERROR       = 0x80,         /* an errored byte is in the FIFO */
        LSR_TX_EMPTY            = 0x40,         /* transmitter is empty */
        LSR_TX_BUFFER_EMPTY     = 0x20,         /* transmit holding reg empty */
        LSR_BREAK_INT           = 0x10,         /* break detected interrupt */
        LSR_FRAMING_ERROR       = 0x08,         /* framing error on current byte */
        LSR_PARITY_ERROR        = 0x04,         /* parity error on current byte */
        LSR_OVERRUN_ERROR       = 0x02,         /* overrun error on receive FIFO */
        LSR_DATA_READY          = 0x01,         /* receive data ready */
        LSR_ERROR_BREAK         = 0x1E,         /* errors except FIFO error and break detected */
        DIVISOR_BYTE_MASK       = 0xFF
    };


public:

    ML310_UART(unsigned int unit = 0) : _unit(unit) {
        rbr = (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_RBR_REG_OFFSET);
        thr = (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_THR_REG_OFFSET);
        ier = (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_IER_REG_OFFSET);
        iir = (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_IIR_REG_OFFSET);
        fcr = (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_FCR_REG_OFFSET);
        lcr = (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_LCR_REG_OFFSET);
        mcr = (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_MCR_REG_OFFSET);
        lsr = (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_LSR_REG_OFFSET);
        msr = (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_MSR_REG_OFFSET);
        drls= (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_DRLS_REG_OFFSET);
        drlm= (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_DRLM_REG_OFFSET);
        int_disable(); //Disable Interrupts
    }

    ML310_UART(unsigned int baud, unsigned int data_bits, unsigned int parity,
             unsigned int stop_bits, unsigned int unit = 0) {
        rbr = (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_RBR_REG_OFFSET);
        thr = (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_THR_REG_OFFSET);
        ier = (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_IER_REG_OFFSET);
        iir = (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_IIR_REG_OFFSET);
        fcr = (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_FCR_REG_OFFSET);
        lcr = (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_LCR_REG_OFFSET);
        mcr = (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_MCR_REG_OFFSET);
        lsr = (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_LSR_REG_OFFSET);
        msr = (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_MSR_REG_OFFSET);
        drls= (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_DRLS_REG_OFFSET);
        drlm= (volatile unsigned char *)(Traits<ML310_UART>::BASE_ADDRESS + UART_DRLM_REG_OFFSET);
        int_disable(); //Disable Interrupts
        config(CLOCK / baud, data_bits, parity, stop_bits);
    }

    void config(unsigned int baud, unsigned int data_bits,
                unsigned int parity, unsigned int stop_bits) {
        /* - MUST REVIEW !
        // Disable all interrupts
        reg(IER, 0);

        // Set clock divisor
        dlab(true);
        reg(DLL, div);
        reg(DLH, div >> 8);
        dlab(false);

        // Set data word length (5, 6, 7 or 8)  
        Reg8 lcr = dbits - 5;

        // Set parity (0 [no], 1 [odd], 2 [even])
        if(par) {
            lcr |= 1 << 3;
            lcr |= (par - 1) << 4;
        }

        // Set stop-bits (1, 2 or 3 [1.5])
        lcr |= (sbits > 1) ? (1 << 2) : 0;

        reg(LCR, lcr);

        // Enables Tx and Rx FIFOs, clear them, set trigger to 14 bytes
        reg(FCR, 0xc7);

        // Set DTR, RTS and OUT2 of MCR
        reg(MCR, reg(MCR) | 0x0b);
        */
    }

    void config(unsigned int * baud, unsigned int * data_bits,
                unsigned int * parity, unsigned int * stop_bits) {
        /* - MUST REVIEW
        // Get clock divisor
        dlab(true); 
        *div = (reg(DLH) << 8) | reg(DLL);
        dlab(false);

        Reg8 lcr = reg(LCR);

        // Get data word length (LCR bits 0 and 1)
        *dbits = (lcr & 0x03) + 5;

        // Get parity (LCR bits 3 [enable] and 4 [odd/even])
        *par = (lcr & 0x08) ? ((lcr & 0x10) ? 2 : 1 ) : 0;

        // Get stop-bits  (LCR bit 2 [0 - >1, 1&D5 -> 1.5, 1&!D5 -> 2)
        *sbits = (lcr & 0x04) ? ((*dbits == 5) ? 3 : 2 ) : 1;
        */
    }

    Reg8 rxd() { return *rbr; }
    void txd(Reg8 c) { *thr = c; }

    void reset() { 
        unsigned int b, db, p, sb;
        config(&b, &db, &p, &sb);
        config(b, db, p, sb);
    }

    void loopback(bool flag) { *mcr |= ( flag << 4); }

    void int_enable(bool receive = true, bool send = true,
                    bool line = true, bool modem = true) {
        *ier = receive | (send << 1) | (line << 2) | (modem << 3);
    }
    void int_disable() { *ier = 0; }

    bool rxd_full() { return (*lsr & LSR_DATA_READY); }
    bool txd_empty() { return (*lsr & LSR_TX_BUFFER_EMPTY); }

    void dtr() { *mcr |= MCR_DTR; }
    void rts() { *mcr |= MCR_RTS; }
    bool cts() { return *msr & (1 << 0); }
    bool dsr() { return *msr & (1 << 1); }
    bool dcd() { return *msr & (1 << 3); }
    bool ri()  { return *msr & (1 << 2); }

    bool overrun_error() { return (*lsr & LSR_OVERRUN_ERROR); }
    bool parity_error()  { return (*lsr & LSR_PARITY_ERROR); }
    bool framing_error() { return (*lsr & LSR_FRAMING_ERROR); }

    char get() { while(!rxd_full()); return rxd(); }
    void put(char c) { while(!txd_empty()); txd(c); }

    static void init(); 

private:
    void dlab(bool f) { *lcr = *lcr & 0x7f | (f << 7); }

private:
    int _unit;
    volatile unsigned char * rbr;
    volatile unsigned char * thr;
    volatile unsigned char * ier;
    volatile unsigned char * iir;
    volatile unsigned char * fcr;
    volatile unsigned char * lcr;
    volatile unsigned char * mcr;
    volatile unsigned char * lsr;
    volatile unsigned char * msr;
    volatile unsigned char * drls;
    volatile unsigned char * drlm;

};

__END_SYS

#endif
