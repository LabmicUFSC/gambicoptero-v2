// EPOS AT90CAN128_CAN Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __at90can128_can_h
#define __at90can128_can_h

#include <cpu.h>
#include <mach/at90can128/memory_map.h>
#include <can.h>

__BEGIN_SYS

class AT90CAN128_CAN: public CAN_Common
{
private:
    typedef Traits<AT90CAN128_CAN> _Traits;

    static const unsigned long CAN_A_BITS_ID    = 0x7FF;
    static const unsigned long CAN_B_BITS_ID    = 0x1FFFFFFF;

    enum
    {
        CANHPMOB = 0xEC - 0x20, // high priority mob 
        CANGCON = 0xD8 - 0x20,
        CANGSTA = 0xD9 - 0x20,
        CANGIT  = 0xDA - 0x20,
        CANGIE  = 0xDB - 0x20,
        CANEN2  = 0xDC - 0x20,
        CANEN1  = 0xDD - 0x20,
        CANIE2  = 0xDE - 0x20,
        CANIE1  = 0xDF - 0x20,
        CANSIT2 = 0xE0 - 0x20,
        CANSIT1 = 0xE1 - 0x20,
        CANBT1  = 0xE2 - 0x20,
        CANBT2  = 0xE3 - 0x20,
        CANBT3  = 0xE4 - 0x20,
        CANTCON = 0xE5 - 0x20,
        CANTIM  = 0xE6 - 0x20,
        CANTTC  = 0xE8 - 0x20,
        CANTEC  = 0xEA - 0x20,
        CANREC  = 0xEB - 0x20 
    };

    enum 
    {
        CANGCON_ABRQ     = 7,
        CANGCON_OVRQ     = 6,
        CANGCON_TTC      = 5,
        CANGCON_SYNCTTC  = 4,
        CANGCON_LISTEN   = 3,
        CANGCON_ENA      = 1,
        CANGCON_SWRES    = 0,

        CANGSTA_OVFG     = 6,
        CANGSTA_TXBSY    = 4,
        CANGSTA_RXBSY    = 3,
        CANGSTA_ENFG     = 2,
        CANGSTA_BOFF     = 1,
        CANGSTA_ERRP     = 0,

        CANGIT_CANIT     = 7,
        CANGIT_BOFFIT    = 6,
        CANGIT_OVRTIM    = 5,
        CANGIT_BXOK      = 4,
        CANGIT_SERG      = 3,
        CANGIT_CERG      = 2,
        CANGIT_FERG      = 1,
        CANGIT_AERG      = 0,
        CANGIT_ERRMASK   = 0x0F,
        CANGIT_INTMASK   = 0x5F,

        CANGIE_ENIT      = 7,
        CANGIE_ENBOFF    = 6,
        CANGIE_ENRX      = 5,
        CANGIE_ENTX      = 4,
        CANGIE_ENERMOB   = 3,
        CANGIE_ENBUF     = 2,
        CANGIE_ENERG     = 1,
        CANGIE_ENOVRT    = 0,

        CANBT1_BRP       = 1,
        CANBT1_BRPMASK   = 0x7E,

        CANBT2_SJW       = 5,
        CANBT2_SJWMASK   = 0x60,
        CANBT2_PRS       = 1,
        CANBT2_PRSMASK   = 0x0E,

        CANBT3_PHS2      = 4,
        CANBT3_PHS2MASK  = 0x70,
        CANBT3_PHS1      = 1,
        CANBT3_PHS1MASK  = 0x0E,
        CANBT3_SMP       = 0,
        CANBT3_SMPMASK   = 0x01,

        CANHPMOB_HPMOB      = 4,
        CANHPMOB_HPMOBMASK  = 0xF0
    };

	class AT90CAN128_CAN_MOB
	{
	private:
		typedef Traits<AT90CAN128_CAN> _Traits;

		union object_id
		{
			unsigned long id;
			unsigned char b[4];
		};

		enum 
		{
			CANPAGE  = 0xED - 0x20, // page(mob selector) - default set to autoincrement pointer on write/read op
			CANSTMOB = 0xEE - 0x20, // mob status
			CANCDMOB = 0xEF - 0x20, // mob control and dlc
			CANIDT4  = 0xF0 - 0x20, // message id tags
			CANIDT3  = 0xF1 - 0x20,  
			CANIDT2  = 0xF2 - 0x20,  
			CANIDT1  = 0xF3 - 0x20, 
			CANIDM4  = 0xF4 - 0x20, // message masks 
			CANIDM3  = 0xF5 - 0x20, 
			CANIDM2  = 0xF6 - 0x20, 
			CANIDM1  = 0xF7 - 0x20, 
			CANSTML  = 0xF8 - 0x20, // time stamp low
			CANSTMH  = 0xF9 - 0x20, // time stamp high  
			CANMSG   = 0xFA - 0x20  // data
		};


		enum 
		{
			CANSTMOB_DLCW       = 7,
			CANSTMOB_TXOK       = 6,
			CANSTMOB_RXOK       = 5,
			CANSTMOB_BERR       = 4,
			CANSTMOB_SERR       = 3,
			CANSTMOB_CERR       = 2,
			CANSTMOB_FERR       = 1,
			CANSTMOB_AERR       = 0,
			CANSTMOB_ERRMASK    = 0x1F,
			CANSTMOB_INTMASK    = 0x7F,

			CANCDMOB_CONMOB     = 6,
			CANCDMOB_RPLY       = 5,
			CANCDMOB_IDE        = 4,
			CANCDMOB_CONMOBMASK = 0xC0,
			CANCDMOB_DLCMASK    = 0x0F,
			ENABLE_TX           = 1, // 0b01
			ENABLE_RX           = 2, // 0b10
			ENABLE_BUFFER       = 3, // 0b11
        
			CANPAGE_MOB			= 4,
			CANPAGE_NOINCREMENT = 8,

			CANIDT4_RTRTAG      = 2,
			CANIDT4_RB1TAG      = 1,
			CANIDT4_RB0TAG      = 0,
        
			CANIDM4_RTRMASK     = 2,
			CANIDM4_IDEMASK     = 0
		};

	public:  
		AT90CAN128_CAN_MOB() : _mob_index(0), _extended(true) 
		{
			_mob_id.id = 0; _mob_mask.id = 0;
			clear(); 
		}
		~AT90CAN128_CAN_MOB() {}

		int status();
		int error();
		void clear();
		
		void mob_index( int index ) { _mob_index = index; }

		// setting ID only takes effect when mob is set to tx/rx
		void id(unsigned long ID) { _mob_id.id = ID; }
		unsigned long id();
		bool is_extended() { return _extended; }
    
		// setting mask only takes effect when the mode is set to rx
		void mask(unsigned long mask) { _mob_mask.id = mask; }
    
		// autoreply mode is activated by rtr + reply valid flag... not implemented!
		void rx_mode(bool extended, int max_size, bool compare_remote_bit, bool compare_extended_bit);
		void buffer_mode(bool extended, int max_size, bool compare_remote_bit, bool compare_extended_bit);
		void disable();

		void tx_request(bool extended);
		void tx_data(unsigned char *data, unsigned char size, bool extended);

		int retrieve_data(unsigned char *data);
    
		static void clear_all();

	private:

		typedef AVR8::Reg8 Reg8;

		void prepare_rx(bool extended, int max_size, bool compare_rtr, bool compare_ide);
		void prepare_id(bool extended);

		static void page(int mob); // Caution! static mutex locking function, call release_page to unlock!
		inline static void page_release(); /*{ Page_Mutex.unlock(); }*/
		static int page() { return canpage() >> CANPAGE_MOB; }

		static void canmsg(Reg8 Value) { AVR8::out8(CANMSG, Value); }
		static Reg8 canmsg() { return AVR8::in8(CANMSG); }
		static void canpage(Reg8 Value) { AVR8::out8(CANPAGE, Value); }
		static Reg8 canpage() { return AVR8::in8(CANPAGE); }
		static void canstmob(Reg8 Value) { AVR8::out8(CANSTMOB, Value); }
		static Reg8 canstmob() { return AVR8::in8(CANSTMOB); }
		static void cancdmob(Reg8 Value) { AVR8::out8(CANCDMOB, Value); }
		static Reg8 cancdmob() { return AVR8::in8(CANCDMOB); }
		static void canidm1(Reg8 Value) { AVR8::out8(CANIDM1, Value); }
		static Reg8 canidm1() { return AVR8::in8(CANIDM1); }
		static void canidm2(Reg8 Value) { AVR8::out8(CANIDM2, Value); }
		static Reg8 canidm2() { return AVR8::in8(CANIDM2); }
		static void canidm3(Reg8 Value) { AVR8::out8(CANIDM3, Value); }
		static Reg8 canidm3() { return AVR8::in8(CANIDM3); }
		static void canidm4(Reg8 Value) { AVR8::out8(CANIDM4, Value); }
		static Reg8 canidm4() { return AVR8::in8(CANIDM4); }
		static void canidt1(Reg8 Value) { AVR8::out8(CANIDT1, Value); }
		static Reg8 canidt1() { return AVR8::in8(CANIDT1); }
		static void canidt2(Reg8 Value) { AVR8::out8(CANIDT2, Value); }
		static Reg8 canidt2() { return AVR8::in8(CANIDT2); }
		static void canidt3(Reg8 Value) { AVR8::out8(CANIDT3, Value); }
		static Reg8 canidt3() { return AVR8::in8(CANIDT3); }
		static void canidt4(Reg8 Value) { AVR8::out8(CANIDT4, Value); }
		static Reg8 canidt4() { return AVR8::in8(CANIDT4); }
    
	private:
		int _mob_index;
		object_id _mob_id;
		object_id _mob_mask;
		bool _extended;
    
		static Mutex Page_Mutex;
	};

public:  
    AT90CAN128_CAN() { configure(125000, true); }
    
    AT90CAN128_CAN(long br, bool autostart = true) { configure(br, autostart); }

    // values int time quantum, min..max: 
    //             1..32        1..8              1..8                1..8                1..4

    ~AT90CAN128_CAN() { stop(); }

    inline int mob_select() { return selected_mob; }
    int mob_select(int index);

    bool mob_is_free();

    inline int mob_status() { return mobs[selected_mob].status(); }
    inline int mob_error() { return mobs[selected_mob].error(); }
    inline void mob_clear() { mobs[selected_mob].clear(); }
    inline void mob_disable() { mobs[selected_mob].disable(); }
   
    int mob_rx_mode(unsigned long ID, unsigned long mask = 0,
                    int max_data_size = _Traits::MAX_DATASIZE,
                    bool extended = true, int type = MOB_COMPARE_ALL_BITS);
    int mob_buffer_mode(unsigned long ID, unsigned long mask = 0,
                        int max_data_size = _Traits::MAX_DATASIZE,
                        bool extended = true, int type = MOB_COMPARE_ALL_BITS);
    int mob_send_data(unsigned char *data, unsigned long ID, unsigned char size, bool extended = true);
    // use: buffer size must be at least equal than mob data size(max = MAX_DATASIZE)
    int mob_retrieve_data(unsigned char *data, unsigned long *ID, bool *extended);
	int mob_request_data(unsigned long ID, bool extended);

// -- below goes MOB independent code

    int high_prio_mob();//return a mob with the interrupt flag set and of highest priority

    void put_overload_request() //the overload frame will be sent upon next rx
    {
        cangcon(cangcon() | CANGCON_OVRQ);
    }

    inline int rx_error_count() { return canrec(); }
    inline int tx_error_count() { return cantec(); }

    int status();

    long baud_rate(long baudrate);
    long baud_rate(int brp, int prop_seg, int phase_seg1, int phase_seg2, int sjw);
    inline long baud_rate() { return _baud_rate; }

    void start(bool wait = true, bool listenonly = false) 
    {
        unsigned char conval = 1<<CANGCON_ENA;
        if(listenonly)
            conval |= 1<<CANGCON_LISTEN;
        cangcon(cangcon() | conval); 
        if(wait)
            while(!(cangsta() & (1<<CANGSTA_ENFG))) {}
    }
    void stop(bool wait = true)  
    { 
        cangcon(cangcon() & (~(1<<CANGCON_ENA)));
        if(wait)
            while(cangsta() & (1<<CANGSTA_ENFG)) {}
    }

    static int init(); 

    void configure(long baudrate, bool autostart,
                   int brp = 0, int prop_seg = 0, int phase_seg1 = 0, int phase_seg2 = 0, int sjw = 0)
    {
        if(autostart)
            stop(); //force stop if already running...

        if( baudrate != -1 )
            baud_rate(baudrate);
        else
            baud_rate(brp, prop_seg, phase_seg1, phase_seg2, sjw);

            AT90CAN128_CAN_MOB::clear_all();
            mob_select(0);
        if(autostart)
            start();
    }


private:

    typedef AVR8::Reg8 Reg8;

    inline bool check_id(bool extended_id, unsigned long ID)
    {
        if((!extended_id && (ID > CAN_A_BITS_ID)) || (extended_id && (ID > CAN_B_BITS_ID)))
            return false;
        
        return true;
    }

    inline static bool mob_is_free(int mob)
    {
        if(mob < _Traits::REGISTER_MOBS) 
            return !(canen2() & (1<<mob));
        else
            return !(canen1() & (1<<(mob-_Traits::REGISTER_MOBS)));
    }

    static void canhpmob(Reg8 Value) { AVR8::out8(CANHPMOB, Value); }
    static Reg8 canhpmob() { return AVR8::in8(CANHPMOB); }
    static void cangcon(Reg8 Value) { AVR8::out8(CANGCON, Value); }
    static Reg8 cangcon() { return AVR8::in8(CANGCON); }
    static void cangsta(Reg8 Value) { AVR8::out8(CANGSTA, Value); }
    static Reg8 cangsta() { return AVR8::in8(CANGSTA); }
    static Reg8 cantec() { return AVR8::in8(CANTEC); }
    static Reg8 canrec() { return AVR8::in8(CANREC); }
    static void canen2(Reg8 Value) { AVR8::out8(CANEN2, Value); }
    static Reg8 canen2() { return AVR8::in8(CANEN2); }
    static void canen1(Reg8 Value) { AVR8::out8(CANEN1, Value); }
    static Reg8 canen1() { return AVR8::in8(CANEN1); }
    static void canbt1(Reg8 Value) { AVR8::out8(CANBT1, Value); }
    static Reg8 canbt1() { return AVR8::in8(CANBT1); }
    static void canbt2(Reg8 Value) { AVR8::out8(CANBT2, Value); }
    static Reg8 canbt2() { return AVR8::in8(CANBT2); }
    static void canbt3(Reg8 Value) { AVR8::out8(CANBT3, Value); }
    static Reg8 canbt3() { return AVR8::in8(CANBT3); }

private:
    int selected_mob;
    long _baud_rate;

    static AT90CAN128_CAN_MOB mobs[_Traits::MAX_MOBS];
};


__END_SYS

#endif
