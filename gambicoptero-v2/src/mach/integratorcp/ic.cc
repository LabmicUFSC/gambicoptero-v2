#include <mach/integratorcp/ic.h>

__BEGIN_SYS

IntegratorCP_IC::Interrupt_Handler IntegratorCP_IC::_vector[29] =
{ 
    &IntegratorCP_IC::default_handler, &IntegratorCP_IC::default_handler,
    &IntegratorCP_IC::default_handler, &IntegratorCP_IC::default_handler,
    &IntegratorCP_IC::default_handler, &IntegratorCP_IC::default_handler,
    &IntegratorCP_IC::default_handler, &IntegratorCP_IC::default_handler,
    &IntegratorCP_IC::default_handler, &IntegratorCP_IC::default_handler,
    &IntegratorCP_IC::default_handler, &IntegratorCP_IC::default_handler,
    &IntegratorCP_IC::default_handler, &IntegratorCP_IC::default_handler,
    &IntegratorCP_IC::default_handler, &IntegratorCP_IC::default_handler,
    &IntegratorCP_IC::default_handler, &IntegratorCP_IC::default_handler,
    &IntegratorCP_IC::default_handler, &IntegratorCP_IC::default_handler,
    &IntegratorCP_IC::default_handler, &IntegratorCP_IC::default_handler,
    &IntegratorCP_IC::default_handler, &IntegratorCP_IC::default_handler,
    &IntegratorCP_IC::default_handler, &IntegratorCP_IC::default_handler,
    &IntegratorCP_IC::default_handler, &IntegratorCP_IC::default_handler,
    &IntegratorCP_IC::default_handler 
};

CPU::Reg32 IntegratorCP_IC::_mask = 0;

void IntegratorCP_IC::int_handler() {
    CPU::Reg32 status = CPU::in32(PIC_IRQ_STATUS);
        
    // search for interrupt id
    Interrupt_Id id = 0;
    CPU::Reg32 s = status;
    while (s >>= 1) {
        ++id;
    };
    
    db<IC>(TRC) << "IC::handle_int(id="<<id<<")\n";
   
    // call handler
    _vector[id](id);
    //db<IC>(TRC) << "return from handler\n";
}

void IntegratorCP_IC::default_handler(Interrupt_Id id) {
    db<IntegratorCP_IC>(INF) << "IntegratorCP_IC::default_handler(id="<<id<<")\n";    
}


__END_SYS
