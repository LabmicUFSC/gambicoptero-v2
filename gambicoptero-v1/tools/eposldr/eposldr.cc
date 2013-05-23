/*=======================================================================*/

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
/* LDR.C                                                                 */
/*                                                                       */
/* Desc: Tool to trasmit the EPOS Boot image through Serial Port	 */
/*                                                                       */
/* Parm: <boot image> <-file epos_img> <-uart device>                    */
/* Both parameter are optional, if ommited -file = $EPOS/img/epos.img    */
/*                                         -uart = /dev/ttyS0            */
/*                                                                       */
/* Auth: Hugo                                                            */
/*=======================================================================*/
#include "linux_uart_device.h"
#include <unistd.h>

int main( int argc, char *argv[] ) {

  unsigned char START_CMD = (unsigned char)0xA5;
  int linux_uart_no = 0;
  bool usb = false;
  char * filename;
  filename = getenv("EPOS");
  strcat(filename, "/img/epos.img");

  //Processa argumentos.
  for(int i = 1; i < argc; i++){
    if(strcmp("-uart", argv[i]) == 0){
      i++;
      if(strcmp("/dev/ttyS0", argv[i]) == 0){
	linux_uart_no = 0;
        usb = false;
      } else if(strcmp("/dev/ttyS1", argv[i]) == 0){
        linux_uart_no = 1;
        usb = false;
      } else if(strcmp("/dev/ttyUSB0", argv[i]) == 0){
        linux_uart_no = 0;
        usb = true;
      } else if(strcmp("/dev/ttyUSB1", argv[i]) == 0){
        linux_uart_no = 1;
        usb = true;
      } else {
	printf("Invalid device %s\n", argv[i]);
      }
    } else if (strcmp("-file", argv[i]) == 0){
      i++;
      strcpy(filename, argv[i]);
    }
  }

  LinuxSerial uart(linux_uart_no, usb);

  FILE *file;
  struct stat file_attributes;
  unsigned long file_size;

  file = fopen(filename, "r");
  if (file == NULL) {
    perror(filename);
    exit(-1);
  }
  printf("Opening image file: %s\n", filename);
  fseek ( file , 0 , SEEK_SET );
  fflush( file );
  fstat( fileno( file ), & file_attributes );
  file_size = file_attributes.st_size;

  unsigned char sendBuffer[file_size];

  // le o arquivo todo;
  if( fread( sendBuffer, 1, file_size, file ) != file_size ) {
    printf("\nError reading image file...\n");
    fclose( file );
    exit(-1);
  }
  fclose( file );

  //Envia o arquivo pela serial...
  printf("\nSending %d bytes...\n", file_size);
  //Start protocol
  uart.send(&START_CMD,1);
  uart.send(&START_CMD,1);

  unsigned char *size_ptr = reinterpret_cast<unsigned char *>(&file_size);
  //uart.send(&size_ptr[1],1);
  //uart.send(&size_ptr[3],1);
  //uart.send(&size_ptr[2],1);
  //uart.send(&size_ptr[0],1);
  uart.send(size_ptr,4);

  for(unsigned int count=0; count < file_size ; count++){
      printf("                  \r%d bytes sent!", count+1);
      if(uart.send(&sendBuffer[count], 1) != 1){
	printf("\nError sending the file...\n");
	exit(-1);
      }
  }

  printf("\nOk, image sent!\n");

  unsigned char a;
  while(true)
  {
    if(uart.receive(&a, 1))
      write(STDOUT_FILENO, &a, 1);
  }

  printf("\nfim...\n");

};
