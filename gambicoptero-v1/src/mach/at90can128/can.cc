// EPOS AT90CAN128_CAN Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.


#include <machine.h>
#include <mach/at90can128/can.h>
#include <mutex.h>

__BEGIN_SYS

Mutex AT90CAN128_CAN::AT90CAN128_CAN_MOB::Page_Mutex;
AT90CAN128_CAN::AT90CAN128_CAN_MOB AT90CAN128_CAN::mobs[_Traits::MAX_MOBS];

void AT90CAN128_CAN::AT90CAN128_CAN_MOB::page_release()
{
    Page_Mutex.unlock();
}

int AT90CAN128_CAN::AT90CAN128_CAN_MOB::status()
{
    unsigned char stt, scd;
    int ret;
    page(_mob_index);
    stt = canstmob();
    scd = cancdmob();
    page_release();

    if(stt & (1<<CANSTMOB_TXOK))
        ret = MOB_TX_COMPLETED;
    else if(stt & (1<<CANSTMOB_RXOK))
        ret = MOB_RX_COMPLETED;
    else
    {
        if( ( scd & CANCDMOB_CONMOBMASK ) == 0)
            ret = MOB_DISABLED;
        else
            ret = MOB_BUSY;

    }

    return ret;
}

int AT90CAN128_CAN::AT90CAN128_CAN_MOB::error()
{
    unsigned char stt;
    int ret = MOB_NO_ERROR;
    page(_mob_index);
    stt = canstmob();
    page_release();

    if(stt & CANSTMOB_DLCW)
        ret |= MOB_ERROR_DLC;
    if(stt & CANSTMOB_BERR)
        ret |= MOB_ERROR_BIT;
    if(stt & CANSTMOB_SERR)
        ret |= MOB_ERROR_STUFF;
    if(stt & CANSTMOB_CERR)
        ret |= MOB_ERROR_CRC;
    if(stt & CANSTMOB_FERR)
        ret |= MOB_ERROR_FORM;
    if(stt & CANSTMOB_AERR)
        ret |= MOB_ERROR_ACK;

    return ret;
}

void AT90CAN128_CAN::AT90CAN128_CAN_MOB::clear()
{
    page(_mob_index);
    canstmob(0); 
    cancdmob(0); 
    canidt4(0);
    canidt3(0);
    canidt2(0);
    canidt1(0);
    canidm4(0); 
    canidm3(0); 
    canidm2(0); 
    canidm1(0); 
    for(int i = 0; i < _Traits::MAX_DATASIZE; i++)
        canmsg(0);    
    page_release();
}

unsigned long AT90CAN128_CAN::AT90CAN128_CAN_MOB::id() 
{ 
    page(_mob_index);
    if( cancdmob() & (1<<CANCDMOB_IDE) )
    {
        _extended = true;
        _mob_id.b[3] = canidt1() >> 3;
        _mob_id.b[2] = (canidt1() << 5) | (canidt2() >> 3);
        _mob_id.b[1] = (canidt2() << 5) | (canidt3() >> 3);
        _mob_id.b[0] = (canidt3() << 5) | (canidt4() >> 3);
    }
    else
    {
        _mob_id.b[2] = _mob_id.b[3] = 0;
        _mob_id.b[1] = canidt1() >> 3;
        _mob_id.b[0] = (canidt1() << 5) | (canidt2() >> 3);
        _extended = false;
    }
    page_release();

    return _mob_id.id; 
}

void AT90CAN128_CAN::AT90CAN128_CAN_MOB::rx_mode(bool extended, int max_size, bool compare_remote_bit, bool compare_extended_bit)
{
    page(_mob_index);

    prepare_rx(extended, max_size, compare_remote_bit, compare_extended_bit);
            
    cancdmob(cancdmob() | (ENABLE_RX << CANCDMOB_CONMOB)); 
    page_release();                
}

void AT90CAN128_CAN::AT90CAN128_CAN_MOB::buffer_mode(bool extended, int max_size, bool compare_remote_bit, bool compare_extended_bit)
{
    page(_mob_index);

    prepare_rx(extended, max_size, compare_remote_bit, compare_extended_bit);
            
    cancdmob(cancdmob() | (ENABLE_BUFFER << CANCDMOB_CONMOB)); 
    page_release();
}

void AT90CAN128_CAN::AT90CAN128_CAN_MOB::disable()
{
    page(_mob_index);
    cancdmob(cancdmob() & (~CANCDMOB_CONMOBMASK));
    page_release();
}

void AT90CAN128_CAN::AT90CAN128_CAN_MOB::tx_request(bool extended)
{
    page(_mob_index);

    prepare_id(extended);
    
    canidt4(canidt4() | (1<<CANIDT4_RTRTAG)); // set rtr -> data request frame
    
    cancdmob(cancdmob() | (ENABLE_TX << CANCDMOB_CONMOB)); 
    page_release();
}

void AT90CAN128_CAN::AT90CAN128_CAN_MOB::tx_data(unsigned char *data, unsigned char size, bool extended)
{
    page(_mob_index);

    prepare_id(extended);
    
    size = (size & CANCDMOB_DLCMASK);
    cancdmob((cancdmob() & ~CANCDMOB_DLCMASK ) | size);
    
    // using autoincrement pointer feature of controller
    for(int i=0; i < size; i++)
        canmsg(data[i]);
        
    cancdmob(cancdmob() | (ENABLE_TX << CANCDMOB_CONMOB)); 
    page_release();
}

int AT90CAN128_CAN::AT90CAN128_CAN_MOB::retrieve_data(unsigned char *data)
{
    int size;
    
    page(_mob_index);
    size = cancdmob() & CANCDMOB_DLCMASK;
    if(size != 0)
    {
        // using autoincrement pointer feature of controller
        for(unsigned char i=0; i < size; i++)
            data[i] = canmsg();
    }
    page_release();
    return size;
}

void AT90CAN128_CAN::AT90CAN128_CAN_MOB::clear_all()
{
    for(int i = 0; i < _Traits::MAX_MOBS; i++)
    {
        page(i);
        canstmob(0); 
        cancdmob(0); 
        canidt4(0);
        canidt3(0);
        canidt2(0);
        canidt1(0);
        canidm4(0); 
        canidm3(0); 
        canidm2(0); 
        canidm1(0); 
        for(int i = 0; i < _Traits::MAX_DATASIZE; i++)
            canmsg(0);
        page_release();
    }
}

void AT90CAN128_CAN::AT90CAN128_CAN_MOB::prepare_rx(bool extended, int max_size, bool compare_rtr, bool compare_ide)
{
    unsigned char size;
    prepare_id(extended);
    
    if(extended)
    {
        canidm1((_mob_mask.b[3] << 3) | (_mob_mask.b[2] >> 5));
        canidm2((_mob_mask.b[2] << 3) | (_mob_mask.b[1] >> 5));
        canidm3((_mob_mask.b[1] << 3) | (_mob_mask.b[0] >> 5));
        canidm4(_mob_mask.b[0] << 3);
    }
    else
    {
        canidm1((_mob_mask.b[1] << 5) | (_mob_mask.b[0] >> 3));
        canidm2(_mob_mask.b[0] << 5);
        canidm3(0);
        canidm4(0);
    }
    if(compare_ide)
        canidm4(canidm4() | (1<<CANIDM4_IDEMASK));
    if(compare_rtr)
        canidm4(canidm4() | (1<<CANIDM4_RTRMASK));
        
    size = (max_size & CANCDMOB_DLCMASK);
    cancdmob(cancdmob() | size);        
}

void AT90CAN128_CAN::AT90CAN128_CAN_MOB::prepare_id(bool extended)
{
    canstmob(0);
    cancdmob(0); 

    if(extended)
    {
        canidt1((_mob_id.b[3] << 3) | (_mob_id.b[2] >> 5));
        canidt2((_mob_id.b[2] << 3) | (_mob_id.b[2] >> 5));
        canidt3((_mob_id.b[1] << 3) | (_mob_id.b[0] >> 5));
        canidt4(_mob_id.b[0] << 3);
        cancdmob(cancdmob() | (1<<CANCDMOB_IDE)); // use extended 2.0B ID (29bits)
        _extended = true;
    }
    else
    {
        canidt1((_mob_id.b[1] << 5) | (_mob_id.b[0] >> 3));
        canidt2(_mob_id.b[0] << 5);
        canidt3(0);
        canidt4(0);
        _extended = false;
    }   
}

void AT90CAN128_CAN::AT90CAN128_CAN_MOB::page(int mob) // Caution! static mutex locking function, call release_page to unlock
{
    Page_Mutex.lock();
    if(mob < _Traits::MAX_MOBS)
        canpage( mob << CANPAGE_MOB );
}

//-------------

int AT90CAN128_CAN::mob_select(int index)
{
    if(index > _Traits::MAX_MOBS || index < 0)
        return INVALID_INDEX;
        
    selected_mob = index;
    return OK;
}

bool AT90CAN128_CAN::mob_is_free()
{
    if(mob_is_free(selected_mob) )//&& mobs[selected_mob].status() == MOB_DISABLED) necessary?
        return true;   
    else
        return false;
}

int AT90CAN128_CAN::mob_rx_mode(unsigned long ID, unsigned long mask, int max_data_size, bool extended, int type) 
{
    bool compare_rtr = false, compare_ide = false;
    if(!check_id(extended, ID))
        return INVALID_ID;
    if(!mob_is_free())
        return NOT_FREE;
    if(max_data_size > _Traits::MAX_DATASIZE)
        return INVALID_SIZE;
        
    if(type & MOB_COMPARE_RTR_BIT )
        compare_rtr = true;
    if(type & MOB_COMPARE_IDE_BIT )
        compare_ide = true;
        
    mobs[selected_mob].id(ID);
    mobs[selected_mob].mask(mask);
    mobs[selected_mob].rx_mode(extended, max_data_size, compare_rtr, compare_ide); 
    
    return OK;
}

int AT90CAN128_CAN::mob_buffer_mode(unsigned long ID, unsigned long mask, int max_data_size, bool extended, int type) 
{ 
    bool compare_rtr = false, compare_ide = false;
    if(!check_id(extended, ID))
        return INVALID_ID;
    if(!mob_is_free())
        return NOT_FREE;
    if(max_data_size > _Traits::MAX_DATASIZE)
        return INVALID_SIZE;
        
    if(type & MOB_COMPARE_RTR_BIT )
        compare_rtr = true;
    if(type & MOB_COMPARE_IDE_BIT )
        compare_ide = true;
        
    mobs[selected_mob].id(ID);
    mobs[selected_mob].mask(mask);
    mobs[selected_mob].buffer_mode(extended, max_data_size, compare_rtr, compare_ide); 
    
    return OK;    
}

int AT90CAN128_CAN::mob_send_data(unsigned char *data, unsigned long ID, unsigned char size, bool extended)
{
    if(!check_id(extended, ID))
        return INVALID_ID;
    if(size > _Traits::MAX_DATASIZE)
        return INVALID_SIZE;
    if(!mob_is_free())
        return NOT_FREE;

    
    mobs[selected_mob].id(ID);
    mobs[selected_mob].tx_data(data, size, extended);
    
    return OK;
}

int AT90CAN128_CAN::mob_retrieve_data(unsigned char *data, unsigned long *ID, bool *extended)
{
    int size;
    if(!(mobs[selected_mob].status() & MOB_RX_COMPLETED))
        return NO_DATA;
        
    size = mobs[selected_mob].retrieve_data(data);
    *ID = mobs[selected_mob].id();
    *extended = mobs[selected_mob].is_extended();
    
    return size;
}

int AT90CAN128_CAN::mob_request_data(unsigned long ID, bool extended)
{
    if(!check_id(extended, ID))
        return INVALID_ID;
    if(!mob_is_free())
        return NOT_FREE;
    
    mobs[selected_mob].id(ID);
    mobs[selected_mob].tx_request(extended);
    
    return OK;
}

int AT90CAN128_CAN::high_prio_mob()
{
    int mob = (canhpmob() >> CANHPMOB_HPMOB);
    
    if(mob < _Traits::MAX_MOBS) 
        return mob;
    else
        return NO_PRIORITY_MOB;
}

int AT90CAN128_CAN::status()
{
    unsigned char stt;
    int ret = 0;
    
    stt = cangsta();
    if(stt & CANGSTA_OVFG)
        ret |= CAN_OVERLOAD_TX;
    if(stt & CANGSTA_TXBSY)
        ret |= CAN_TXING;
    if(stt & CANGSTA_RXBSY)
        ret |= CAN_RXING;
    if(stt & CANGSTA_BOFF)
        ret |= CAN_BUSOFF;
    if(stt & CANGSTA_ENFG)
        ret |= CAN_ENABLED;
    if(stt & CANGSTA_ERRP)
        ret |= CAN_ERROR_PASV;
        
    return ret;            
}

long AT90CAN128_CAN::baud_rate(long baudrate)
{
    unsigned char BRP, PRS, PHS1, PHS2, SJW, SMP, TimeQuanta;
    long BRPCalc;
    /*
    Tsyns = (BRP + 1)/CLKio  = 1Tq, CAN TimeQuantum
    Tsjw = Tq * (SJW + 1) 
    Tprs = Tq * (PRS + 1),  Tprs >= (2*Delay), Delay = max(OutPutDelay(node 1) + BusDelay + InputDelay(node 2))
                            Compensates physical delays
    Tphs1 = Tq * (PHS1 + 1)
    Tphs2 = Tq * (PHS2 + 1)
    Here we will use fixed 8 time quanta (8 to 25 can be used)
    So with 1Tq for Tsyns, 1 + PSR, 1 + PHS1 and 1 + PHS2, 1 + SJW we are left with 4Tq
    At this time PHS2 will be set to 0 and PHS1 = 1 and PRS = 3
    As Tsjw can't be bigger than any of PHS, so SJW will be 0
    This will give one of the combinations that give exactly 87.5% sample point
    (calculated by PSH1+PRS+Tsyns = 5Tq, 5Tq/8Tq = 87.5%, default of CANOpen and DeviceNet)
    SMP will be set to 0 because if it is activated, Tprs is increased in 1Tq
    Now we have to calculate the prescaler...
    */
    PRS = 2;   // 2 
    PHS1 = 1;  // 1 for 75% sample point like atmel manual
    PHS2 = 1;  // 1
    SJW = 0;   // 0
    SMP = 0;
    // Below code can be manteined as is if the above parameter calculation changes
    TimeQuanta = 4 + PRS + PHS1 + PHS2 + SJW;
    
    BRPCalc = baudrate;
    BRP = (Traits<Machine>::CLOCK / (BRPCalc * TimeQuanta)) - 1;
    
    BRP = (BRP << CANBT1_BRP) & CANBT1_BRPMASK;
    canbt1(BRP);

    SJW = (SJW << CANBT2_SJW) & CANBT2_SJWMASK;
    PRS = (PRS << CANBT2_PRS) & CANBT2_PRSMASK;
    canbt2(SJW | PRS);

    PHS1 = (PHS1 << CANBT3_PHS1) & CANBT3_PHS1MASK;
    PHS2 = (PHS2 << CANBT3_PHS2) & CANBT3_PHS2MASK;
    SMP = (SMP << CANBT3_SMP) & CANBT3_SMPMASK;
    canbt3(PHS2 | PHS1 | SMP);

    _baud_rate = Traits<Machine>::CLOCK / (int)(TimeQuanta * (BRP + 1));// return the real baudrate...
    
    return _baud_rate;
}

long AT90CAN128_CAN::baud_rate(int brp, int prop_seg, int phase_seg1, int phase_seg2, int sjw)
{
    unsigned char BRP, PRS, PHS1, PHS2, SJW, SMP, TimeQuanta;
    long BRPCalc;

    //Considering the received time quanta we adjust to avr register setting just subtracting one

    BRP = brp - 1;
    PRS = prop_seg - 1;
    PHS1 = phase_seg1 - 1;
    PHS2 = phase_seg2 - 1;
    SJW =  sjw - 1;
    SMP = 0;

    TimeQuanta = prop_seg + phase_seg1 + phase_seg2 + sjw;
        
    BRP = (BRP << CANBT1_BRP) & CANBT1_BRPMASK;
    canbt1(BRP);

    SJW = (SJW << CANBT2_SJW) & CANBT2_SJWMASK;
    PRS = (PRS << CANBT2_PRS) & CANBT2_PRSMASK;
    canbt2(SJW | PRS);

    PHS1 = (PHS1 << CANBT3_PHS1) & CANBT3_PHS1MASK;
    PHS2 = (PHS2 << CANBT3_PHS2) & CANBT3_PHS2MASK;
    SMP = (SMP << CANBT3_SMP) & CANBT3_SMPMASK;
    canbt3(PHS2 | PHS1 | SMP);

    _baud_rate = Traits<Machine>::CLOCK / (int)(TimeQuanta * brp);// return the real baudrate...
    
    return _baud_rate;
}

__END_SYS
