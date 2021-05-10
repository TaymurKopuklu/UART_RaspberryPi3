#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#define uzunlukHaber 5


int main(int argc, char** argv) {
 system("sudo systemctl stop serial-getty@ttyAMA0.service");
 int sfd = open("/dev/serial0", O_RDWR | O_NOCTTY); 

 if (sfd == -1) {
  printf("Error no is : %d\n", errno);
  printf("Error description is : %s\n", strerror(errno));
  return (-1);
 };
 struct termios options;
 tcgetattr(sfd, &options);
 cfsetspeed(&options, B9600);
 cfmakeraw(&options);
 options.c_cflag &= ~CSTOPB;
 options.c_cflag |= CLOCAL;
 options.c_cflag |= CREAD;
 options.c_cc[VTIME]=100;
 options.c_cc[VMIN]=10;
 tcsetattr(sfd, TCSANOW, &options);
 
 u_int8_t buf[uzunlukHaber];
 u_int8_t buf2[uzunlukHaber];
 //u_int8_t c;
 buf[0]=0x00;
 buf[1]=0x16;
 buf[2]=0x17;
 buf[3]=0x18;
 buf[4]=0xFF & 0xff;
 u_int8_t count = write(sfd, buf,uzunlukHaber);
 

 //u_int8_t i=0;
 while(1){
  count = read(sfd, buf2, uzunlukHaber);
  if(count!=0){
   // buf2[i]=c;
    //i++;
    if(buf2[uzunlukHaber-1]==0xFF)
    {
  printf("%x ", buf2[0]);
  printf("%x ", buf2[1]);
  printf("%x ", buf2[2]);
  printf("%x ", buf2[3]);
  printf("%x \n", buf2[4]);
    }
     if(buf2[uzunlukHaber-1]==0xF1)
     {
		 buf[3]=buf2[3]+1;
		 write(sfd, buf,uzunlukHaber);
		  printf("%x ", buf2[0]);
  printf("%x ", buf2[1]);
  printf("%x ", buf2[2]);
  printf("%x ", buf2[3]);
  printf("%x \n", buf2[4]);
	}
	if(buf2[uzunlukHaber-3]==0x13)
     {
		buf[2]=buf2[2]+1;
		write(sfd, buf,uzunlukHaber);
	}
	/*if(buf2[0]==buf[0])
     {
		buf[0]=buf2[0]+1;
		write(sfd, buf,uzunlukHaber);
	}*/
  }
 }
  
 close(sfd);
 return (EXIT_SUCCESS);
}

