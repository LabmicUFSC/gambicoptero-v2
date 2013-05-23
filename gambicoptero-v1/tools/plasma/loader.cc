#include "plasma.h"

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include "elf.h"

#define ENABLE                  0xA5
#define COPY_ADDR		0x100E0000 //Last 128K
#define MEM_BASE		0x10000000
#define MEM_TOP 		0x10100000
#define TOUT			0x000FFFFF

#define MemoryRead(A) (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)

inline void leds(unsigned int val){
    //*leds_clear = 0x0;
    unsigned int aux = gpio_base->output;
    aux &= 0xFFFFFF00;
    val &= 0x0000000FF;
    gpio_base->output = aux | val;
}

void print_string(char * p);
void printhex(int i);
void printhex(short s);
void printhex(char c);

void print_string(char * p)
{
   while(*p){
     if(*p == '\n') hal_uart_putc('\r');
     hal_uart_putc(*p);
     p++;
   }
}

void printhex(char c)
{
   hal_uart_putc("0123456789ABCDEF"[(c >> 4) & 15]);
   hal_uart_putc("0123456789ABCDEF"[c & 15]);
}

void printhex(short s){
   printhex((char)(s >> 8));
   printhex((char)(s & 255));
}

void printhex(unsigned int i){
   printhex((short)(i >> 16));
   printhex((short)(i & 0x0000FFFF));		   
}


void timer_handler(unsigned irq){
    print_string("$TIMER_INT$");
}
void peripheral_test(){

    print_string("Plasma peripheral test\n");

    unsigned int i = 0;

    print_string("Test external RAM - Write\n");
    for(i = EXT_RAM_BASE; i < EXT_RAM_TOP; i += 4){
        MemoryWrite(i, i);
    }
    print_string("Test external RAM - Read\n");
    for(i = EXT_RAM_BASE; i < EXT_RAM_TOP; i += 4){
        if(MemoryRead(i) != i){
            print_string("Error\n");
            while(1);
        }
    }
    print_string("Test external RAM - Read after write\n");
    for(i = EXT_RAM_BASE; i < EXT_RAM_TOP; i += 4){
        MemoryWrite(i, i);
        if(MemoryRead(i) != i){
            print_string("Error\n");
            while(1);
        }
    }
    print_string("OK\n");


    print_string("Test Timer\n");
    unsigned int time = timer_regs->time;
    print_string("Time: "); printhex(time); print_string("\n");
    for (i = 0; i < 256; ++i);
    time = timer_regs->time - time;
    print_string("Elapsed: "); printhex(time); print_string("\n");
    print_string("OK\n");

    print_string("Test Timer interrupt / IC\n");
    pic_init();
    pic_register_handler(IRQ_TIMER, timer_handler);
    int_enable();
    unsigned int wait_time = timer_regs->time + time;
    timer_regs->time = wait_time;
    while(timer_regs->time < wait_time);
    print_string("OK\n");

    print_string("Peripheral test end\n\n");
}

int main( void ) {

  hal_uart_init();
  
  //while(1) hal_uart_putc(hal_uart_getc());

  volatile unsigned char * elf_image = (unsigned char *)COPY_ADDR;
  leds(0x01);

  print_string("----------------------------------------------------------------\n");
  print_string("                        Plasma ELF Loader\n");
  print_string("          Software and Hardware Integration Lab - UFSC\n");
  print_string("              Hugo Marcondes (hugom@lisha.ufsc.br)\n");
  print_string("                        v0.1 - Sep. 2006\n");
  print_string("\n");
  print_string("               Set switches to 0xFF to enable test mode\n");
  print_string("----------------------------------------------------------------\n");
  print_string("\n\n");

  if((gpio_base->input & 0xFF) == 0xFF){
      print_string("Test mode enabled\n");
      peripheral_test();
  }

  print_string("---------------------- Waiting OS image ---4343432---------------\n");
  
  leds(0x02);
  //Synchronize with sender  
  while(hal_uart_getc() != ENABLE);
  while(hal_uart_getc() != ENABLE);

  leds(0x03);

  //receive image size
  unsigned int image_size = 0;
  reinterpret_cast<unsigned char*>(&image_size)[3] = hal_uart_getc();
  reinterpret_cast<unsigned char*>(&image_size)[2] = hal_uart_getc();
  reinterpret_cast<unsigned char*>(&image_size)[1] = hal_uart_getc();
  reinterpret_cast<unsigned char*>(&image_size)[0] = hal_uart_getc();

  print_string("Image size: 0x");
  printhex(image_size);
  print_string(" bytes\n");

  leds(0x04);

  if((COPY_ADDR + image_size) >= MEM_TOP){
      print_string("File too long! Aborting receiving!\n");
      leds(0xAA);
      while(1);
  }

  //receives
  unsigned int bytes_received = 0;

  for (bytes_received = 0; bytes_received < image_size; ++bytes_received) {
      elf_image[bytes_received] = hal_uart_getc();
      leds(bytes_received);
  }

  leds(0x05);

  print_string("Expected 0x");
  printhex(image_size);
  print_string(" bytes\n");
  print_string("Received 0x");
  printhex(bytes_received);
  print_string(" bytes\n");
  

  print_string("\n\n");
  print_string("------------------------ Image Received ------------------------\n");

  print_string("-- File received! Now the ELF Segments will be loaded.\n");

  ELF * elf_file = reinterpret_cast<ELF *>(COPY_ADDR);
  if(!elf_file->valid()) {
    print_string("-- Sorry, the ELF image was corrupted!\n");
    while(1);
  }

  print_string("Number of segments: ");
  printhex((unsigned int)elf_file->segments());
  print_string("\n");

  /*
  print_string("Segment 0 size: ");
  printhex((unsigned int)elf_file->segment_size(0));
  print_string("\n");

  print_string("Segment 0 Addr: ");
  printhex((unsigned int)elf_file->segment_address(0));
  print_string("\n");*/

  for(int i = 0; i < elf_file->segments(); ++i){
      print_string("Segment ");
      printhex((unsigned int)i);
      print_string(" size:  ");
      printhex((unsigned int)elf_file->segment_size(i));
      print_string(" addr: ");
      printhex((unsigned int)elf_file->segment_address(i));
      print_string("\n");
  }

  print_string("Elf Entry Point: ");
  printhex((unsigned int)elf_file->entry());
  print_string("\n");

      

  if(elf_file->load_segment(0) < 0) {
    print_string("-- Sorry, the Code Segment was corrupted!\n");
  }

  for(int i = 1; i < elf_file->segments(); i++) {
      if(elf_file->load_segment(i) < 0) {
        print_string("-- Sorry, the Data Segment was corrupted!\n");
      }
  }

  print_string("-- Job Done! Now the ELF will take control. Bye-Bye :P\n");
  print_string("----------------------------------------------------------------\n");
  print_string("\n\n");

  leds(0x00);
  
  // Call setup
  void (*entry)() = reinterpret_cast<void (*)()>(elf_file->entry());
  entry();

  //This point should never be reached ... but ??
  while(1);

  return 0;

};

/*

print_string("`````````````````````````````````````.................`````````````````````\n");
print_string("`````````````````.:/+osyyhdddddddhhhhhhhhhhhyhhhhhhhhhhdmdo-```````````````\n");
print_string("````````````-+yddhhhhyo//:::::::::::::----:::::::.``````.:smh:`````````````\n");
print_string("`````````:smds///::::-------------::::---:::::::-:/:-``````.oNy.```````````\n");
print_string("```````/ddo-``````--:::--..``````````/::---:::::/:::-::-`````-md-``````````\n");
print_string("`````oM+``````:/:.````..::`````````/:``````````.://:/:.:-````.dm-`````````\n");
print_string("``````dm``````/-`````````-/`````````:````.----..```./:-/```````.dm:````````\n");
print_string("`````oMo````````.-:::--```:``````````-+ydhyNNNNNmds/`.``````````.hNo.``````\n");
print_string("``.odd/.....--`ymNmmNNNmh+-````````:hds/-:/mMMMMMNhmm:`.:-````.-:/hNm+.````\n");
print_string("`-md/:/:......`/ossyhhdmNMNdh+`````yMy+ydhysoys+ossyy:`::--::///::/+sdm+```\n");
print_string("`dm:/-+/:://:-`````````.-oMh-```````:ss/-.```+dh/-.`.-:oyddysssydh:`.oomd-`\n");
print_string("`Myo`+.ohysosdd+:+s.`````+M/``````````````````./yhyyyyys/-.-h:``.sNo`.+-dd`\n");
print_string("`Myo`o`..``o--yyys/```.+hdy`````````:/:-`-`````````````.:+hmNm:.``sN-`+`/M`\n");
print_string("`dms--/.``oMs```````:ymMo.```````.../osms::::::.```-:ohdhs/-hMmhs-oM-.+`oM`\n");
print_string("`:Nd+/:-`/MMN/```:::/hymh-`````.syyhh`:Ny````-:/oyddMd/-``.+Nd...-Ny`/--Nd`\n");
print_string("``:Nh...`yMmMmh+:-`````.odhoy/`````.-`os++shhhhs+:-oM/`-/ymMm/```.+:///mh-.\n");
print_string(".``/Mo```mMMohMhddhs+/:--/ooo/:/+osyyhhhdMh:-.```-oNMdddhdMy.`````-:odmo.`.\n");
print_string("..``mh```NMN-dm`.sMhosyydNmhyssdMh/:-..`.md.-/shdNNMMs--ymo```````-hNs-``..\n");
print_string("....dd```mMMmMMo:dM:-...+Mo....sM:...-:/oNMmNNmdy+-yM:/dd:```````:md:```...\n");
print_string("....dd```dMMMMMMMMMMNmmmNMNmmmmNMNmmNNMNmdNN+:.````-Nmd+````````:Nh.``.....\n");
print_string("....dd```sMmMMNMMMMMMMMMMMMMMMMNNMMdyo/-.`yM-````-odd+.````````+Nh.``......\n");
print_string("....md```-mmhMohMhshMNyyyNMs///:-Nh```````.mm--ohds:`````````:hmo.````.....\n");
print_string("-...mh````-dmm+-dN-`dN-``/M/`````md````..-/hMmds:.`.--`.--.:hms-``````.....\n");
print_string("....Nh.````./shddNdshMm+/+Md////+NNsssyhhhs+:-..::::-:::/oddo.```````......\n");
print_string("....Ny``````-.``.-:://+++ooooo+++///:--.``.-::::-::::/sdds:``````````......\n");
print_string("`..-Mo``:-``.//-.```````.......``````.-::::::::::/shdh+-`````````````......\n");
print_string("`../M/``.//:.``--------::::::::::::/++::-----+yddy+:.`````````````````.....\n");
print_string("`..:My.````-:::-------------------.````.-+ydds/-``````````````````````.....\n");
print_string("`...+Nh-``````````````````````````.-/ohdds/.``````````````````````````.....\n");
print_string("`...`-ymy+-.`````````````..:/oyhdmdhyo:.``````````````````````````````.....\n");
print_string("`...```./yddhysssssssyyhddhyo+:-.````````````````````````````````````......\n");
print_string("`..``````````..........``````````````````````````````````````````\n");
*/
