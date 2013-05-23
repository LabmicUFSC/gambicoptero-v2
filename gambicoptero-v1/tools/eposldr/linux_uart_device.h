#include <sys/types.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <sys/signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BAUDRATE        B57600

class LinuxSerial 
{
protected:
  int serial;
  char nome_porta[255];
  struct termios original_serial_settings, app_serial_settings;

public:
  LinuxSerial(unsigned char uart_no, bool usb) {
    if(usb){
      sprintf( nome_porta, "/dev/ttyUSB%d", uart_no );
    } else {
      sprintf( nome_porta, "/dev/ttyS%d", uart_no );
    }

    serial = open(nome_porta, O_RDWR | O_NOCTTY );
    if (serial < 0){
      perror(nome_porta);
      exit(-1);
    }

    tcgetattr(serial, &original_serial_settings); /* save current serial port settings */
    bzero(&app_serial_settings, sizeof(original_serial_settings)); /* clear struct for new port settings */

    app_serial_settings.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    app_serial_settings.c_iflag = IGNPAR;
    app_serial_settings.c_oflag = 0;
    app_serial_settings.c_lflag = 0;
    app_serial_settings.c_cc[VINTR]    = 0;     /* Ctrl-c */
    app_serial_settings.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
    app_serial_settings.c_cc[VERASE]   = 0;     /* del */
    app_serial_settings.c_cc[VKILL]    = 0;     /* @ */
    app_serial_settings.c_cc[VEOF]     = 4;     /* Ctrl-d */ 
    app_serial_settings.c_cc[VTIME]    = 0;     /* inter-character timer unused */
    app_serial_settings.c_cc[VMIN]     = 0;     /* blocking read until 1 character arrives */
    app_serial_settings.c_cc[VSWTC]    = 0;     /* '\0' */
    app_serial_settings.c_cc[VSTART]   = 0;     /* Ctrl-q */
    app_serial_settings.c_cc[VSTOP]    = 0;     /* Ctrl-s */
    app_serial_settings.c_cc[VSUSP]    = 0;     /* Ctrl-z */
    app_serial_settings.c_cc[VEOL]     = 0;     /* '\0' */
    app_serial_settings.c_cc[VREPRINT] = 0;     /* Ctrl-r */
    app_serial_settings.c_cc[VDISCARD] = 0;     /* Ctrl-u */
    app_serial_settings.c_cc[VWERASE]  = 0;     /* Ctrl-w */
    app_serial_settings.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
    app_serial_settings.c_cc[VEOL2]    = 0;     /* '\0' */

    tcflush(serial, TCIFLUSH);
    tcsetattr(serial, TCSANOW, &app_serial_settings);

  }

  ~LinuxSerial() {
    tcsetattr(serial, TCSANOW, &original_serial_settings);
  }

  int send( unsigned char *buffer, int size ) {
     //usleep(10000);
    return write( serial, buffer, size );
  }

  int receive( unsigned char *buffer, int size ) {
    return read( serial, buffer, size );
  }

};
