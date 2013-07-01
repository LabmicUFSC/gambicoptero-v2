// EPOS ML310 PCI Mediator

#ifndef __axi4lite_noc_h
#define __axi4lite_noc_h

#include <noc.h>

__BEGIN_SYS

class AXI4LITE_NOC: public NOC_Common
{
public:
    typedef NOC_Common::Address Address;

    typedef NOC_Common::Info Info;

    AXI4LITE_NOC():NOC_Common(){

        _info.local_addr = local_addr();
        _info.router_x_addr = router_x_addr();
        _info.router_y_addr = router_y_addr();
        _info.net_x_size = net_x_size();
        _info.net_y_size = net_y_size();
        _info.data_width = net_data_width();

        header_src_router_x_addr(_info.router_x_addr);
        header_src_router_y_addr(_info.router_y_addr);
        header_src_local_addr(_info.local_addr);

    }

    void send_header(Address const* address);
    void send(Address const* address, unsigned int const* data);
    void send(unsigned int const* data);

    void receive_header(Address* address);
    void receive(Address* address, unsigned int* data);
    void receive(unsigned int* data);

    void receive_int(IC::Interrupt_Handler h);

private: //HW registers
    enum{
        NOC_BASE = Traits<AXI4LITE_NOC>::BASE_ADDRESS,
    };

    enum{
        REG_HEADER_DST_LOCAL_ADDR = 0,
        REG_HEADER_DST_ROUTER_Y_ADDR,
        REG_HEADER_DST_ROUTER_X_ADDR,
        REG_HEADER_SRC_LOCAL_ADDR,
        REG_HEADER_SRC_ROUTER_Y_ADDR,
        REG_HEADER_SRC_ROUTER_X_ADDR,
        REG_DATA,
        REG_STATUS,
        REG_LOCAL_ADDR,
        REG_ROUTER_X_ADDR,
        REG_ROUTER_Y_ADDR,
        REG_NET_X_SIZE,
        REG_NET_Y_SIZE,
        REG_NET_DATA_WIDTH
    };


    static inline unsigned int reg(unsigned int offset){
        volatile unsigned int* aux = reinterpret_cast<volatile unsigned int*>(NOC_BASE);
        return aux[offset];
    }

    static inline void reg(unsigned int offset, unsigned int val){
        volatile unsigned int* aux = reinterpret_cast<volatile unsigned int*>(NOC_BASE);
        aux[offset] = val;
    }

    static inline unsigned int header_src_router_x_addr(){return reg(REG_HEADER_SRC_ROUTER_X_ADDR);}
    static inline unsigned int header_src_router_y_addr(){return reg(REG_HEADER_SRC_ROUTER_Y_ADDR);}
    static inline unsigned int header_src_local_addr(){return reg(REG_HEADER_SRC_LOCAL_ADDR);}
    static inline unsigned int header_dst_router_x_addr(){return reg(REG_HEADER_DST_ROUTER_X_ADDR);}
    static inline unsigned int header_dst_router_y_addr(){return reg(REG_HEADER_DST_ROUTER_Y_ADDR);}
    static inline unsigned int header_dst_local_addr(){return reg(REG_HEADER_DST_LOCAL_ADDR);}

    static inline void header_src_router_x_addr(unsigned int val){reg(REG_HEADER_SRC_ROUTER_X_ADDR, val);}
    static inline void header_src_router_y_addr(unsigned int val){reg(REG_HEADER_SRC_ROUTER_Y_ADDR, val);}
    static inline void header_src_local_addr(unsigned int val){reg(REG_HEADER_SRC_LOCAL_ADDR, val);}
    static inline void header_dst_router_x_addr(unsigned int val){reg(REG_HEADER_DST_ROUTER_X_ADDR, val);}
    static inline void header_dst_router_y_addr(unsigned int val){reg(REG_HEADER_DST_ROUTER_Y_ADDR, val);}
    static inline void header_dst_local_addr(unsigned int val){reg(REG_HEADER_DST_LOCAL_ADDR, val);}

    static inline unsigned int data(){ return reg(REG_DATA);}
    static inline void data(unsigned int val){ reg(REG_DATA, val);}

    static inline void wr(){ reg(REG_STATUS, 0x1); }
    static inline void rd(){ reg(REG_STATUS, 0x2); }
    static inline bool wait(){return reg(REG_STATUS) & 0x4; }
    static inline bool nd(){return reg(REG_STATUS) & 0x8; }

    static inline unsigned int local_addr(){return reg(REG_LOCAL_ADDR);}
    static inline unsigned int router_x_addr(){return reg(REG_ROUTER_X_ADDR);}
    static inline unsigned int router_y_addr(){return reg(REG_ROUTER_Y_ADDR);}
    static inline unsigned int net_x_size(){return reg(REG_NET_X_SIZE);}
    static inline unsigned int net_y_size(){return reg(REG_NET_Y_SIZE);}
    static inline unsigned int net_data_width(){return reg(REG_NET_DATA_WIDTH);}


};

__END_SYS

#endif
