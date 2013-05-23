/*--------------------------------------------------------------------

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
 * TITLE: Plasma Hardware Defines
 * AUTHOR: Steve Rhoads (rhoadss@yahoo.com)
 * DATE CREATED: 12/17/05
 * FILENAME: plasma.h
 * PROJECT: Plasma CPU core
 * COPYRIGHT: Software placed into the public domain by the author.
 *    Software 'as is' without warranty.  Author liable for nothing.
 * DESCRIPTION:
 *    Plasma Hardware Defines
 *--------------------------------------------------------------------*/
#ifndef __PLASMA_H__
#define __PLASMA_H__

////////////////////////////////
#define EXT_RAM_BASE 0x10000000
#define EXT_RAM_TOP  0x100FFFFF

/////////////////////////////
#define UART_BASE 0x80000000
typedef struct {
  //  All elements are 8 bits except for clkdiv (16), but we use uint32 to make
  //    the hardware for decoding easier
  volatile unsigned int clkdiv;  // Set to 50e6 divided by baud rate (no x16 factor)
  volatile unsigned int txlevel; // Number of spaces in the FIFO for writes
  volatile unsigned int rxlevel; // Number of available elements in the FIFO for reads
  volatile unsigned int txchar;  // Write characters to be sent here
  volatile unsigned int rxchar;  // Read received characters here
} uart_regs_t;
#define uart_regs ((uart_regs_t *) UART_BASE)
void
hal_uart_init(void)
{
    uart_regs->clkdiv = 868;  // 57600 bps
    //uart_regs->clkdiv = 5208;  // 9600 bps
}

void
hal_uart_putc(int ch)
{

  while (uart_regs->txlevel == 0)    // wait for fifo to have space
    ;

  uart_regs->txchar = ch;
}

void
hal_uart_putc_nowait(int ch)
{

  if(uart_regs->txlevel)   // If fifo has space
      uart_regs->txchar = ch;
}

int
hal_uart_getc(void)
{
  while ((uart_regs->rxlevel) == 0)  // wait for data to be ready
    ;

  return uart_regs->rxchar;
}
//////////////////////////////
#define GPIO_BASE 0x80000400

typedef struct {
  volatile unsigned int output;
  volatile unsigned int input;
} gpio_regs_t;

#define gpio_base ((gpio_regs_t *) GPIO_BASE)
//////////////////////////////////
#define PIC_BASE  0x80000C00

// Interrupt request lines
// Bit numbers (LSB == 0) that correpond to interrupts into PIC

#define IRQ_TIMER   0
#define IRQ_UART_RX 1
#define IRQ_UART_TX 2

#define IRQ_TO_MASK(x) (1 << (x))

#define PIC_TIMER_INT     IRQ_TO_MASK(IRQ_TIMER)
#define PIC_UART_RX_INT   IRQ_TO_MASK(IRQ_UART_RX)
#define PIC_UART_TX_INT   IRQ_TO_MASK(IRQ_UART_TX)

typedef struct {
  volatile unsigned int edge_enable; // mask: 1 -> edge triggered, 0 -> level
  volatile unsigned int polarity;    // mask: 1 -> rising edge
  volatile unsigned int mask;    // mask: 1 -> disabled
  volatile unsigned int pending;     // mask: 1 -> pending; write 1's to clear pending ints
} pic_regs_t;

#define pic_regs ((pic_regs_t *) PIC_BASE)

typedef void (*irq_handler_t)(unsigned irq);

#define NVECTORS 32

/*
 * Our secondary interrupt vector.
 */
void
nop_handler(unsigned irq)
{
  // nop
}
irq_handler_t pic_vector[NVECTORS] = {
  nop_handler, nop_handler, nop_handler, nop_handler,
  nop_handler, nop_handler, nop_handler, nop_handler,
  nop_handler, nop_handler, nop_handler, nop_handler,
  nop_handler, nop_handler, nop_handler, nop_handler,
  nop_handler, nop_handler, nop_handler, nop_handler,
  nop_handler, nop_handler, nop_handler, nop_handler,
  nop_handler, nop_handler, nop_handler, nop_handler,
  nop_handler, nop_handler, nop_handler, nop_handler
};


void
pic_init(void)
{
  // uP is level triggered

  pic_regs->mask = ~0;                     // mask all interrupts
  pic_regs->edge_enable = PIC_TIMER_INT;
  pic_regs->polarity = ~0;         // rising edge
  pic_regs->pending = ~0;                  // clear all pending ints
}

extern "C"{

void OS_InterruptServiceRoutine(unsigned int status)
{
    // pending and not masked interrupts
    int live = pic_regs->pending & ~pic_regs->mask;

    // FIXME loop while there are interrupts to service.
    //   That will reduce our overhead.

    // handle the first one set
    int i;
    int mask;
    for (i=0, mask=1; i < NVECTORS; i++, mask <<= 1){
      if (mask & live){       // handle this one
        // puthex_nl(i);
        //(*pic_vector[i])(i); //TODO maldito seja
        pic_regs->pending = mask; // clear pending interrupt
        return;
      }
    }
}

}



void
pic_register_handler(unsigned irq, irq_handler_t handler)
{
  if (irq >= NVECTORS)
    return;
  pic_vector[irq] = handler;

  pic_regs->mask &= ~IRQ_TO_MASK(irq);
}

unsigned int cp0regs_r() {
    unsigned int value;
    __asm__ __volatile__("mfc0 %0, $12" : "=r"(value) :);
    return value;
}
void cp0regs_w(unsigned int value) {
    __asm__ __volatile__("mtc0 %0, $12" : : "r"(value));
}

void int_enable() {
    unsigned int status = cp0regs_r();
    status = status | 0x01;
    cp0regs_w(status);
}

void int_disable() {
    unsigned int status = cp0regs_r();
    status = status & 0xFFFFFFFE;
    cp0regs_w(status);
}

//////////////////////////////////
#define TIMER_BASE  0x80000800

typedef struct {
  volatile unsigned int time;   // R: current, W: set time to interrupt
} timer_regs_t;

#define timer_regs ((timer_regs_t *) TIMER_BASE)
///////////////////////////////////////////

#endif //__PLASMA_H__

