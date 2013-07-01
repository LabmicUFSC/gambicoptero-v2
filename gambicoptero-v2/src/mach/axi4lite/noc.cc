// EPOS AXI4LITE_NIC Implementation

#include <machine.h>
#include <noc.h>


__BEGIN_SYS

void AXI4LITE_NOC::send_header(Address const* address){
    header_dst_local_addr(address->local);
    header_dst_router_x_addr(address->router_x);
    header_dst_router_y_addr(address->router_y);
}
void AXI4LITE_NOC::send(unsigned int const* _data){
    while(wait());
     data(_data[0]);
     wr();
}
void AXI4LITE_NOC::send(Address const* address, unsigned int const* _data){
    send_header(address);
    send(_data);
}
void AXI4LITE_NOC::receive_header(Address* address){
    address->local = header_src_local_addr();
    address->router_x = header_src_router_x_addr();
    address->router_y = header_src_router_y_addr();
}
void AXI4LITE_NOC::receive(unsigned int* _data){
    while(!nd());
    _data[0] = data();
    rd();
}
void AXI4LITE_NOC::receive(Address* address, unsigned int* _data){
    receive_header(address);
    receive(_data);
}
void AXI4LITE_NOC::receive_int(IC::Interrupt_Handler h){
    CPU::int_disable();
    IC::disable(IC::IRQ_NOC);
    IC::int_vector(IC::IRQ_NOC, h);
    IC::enable(IC::IRQ_NOC);
    CPU::int_enable();
}

__END_SYS
