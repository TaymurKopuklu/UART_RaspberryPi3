#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>


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
 options.c_cc[VTIME]=0;
 options.c_cc[VMIN]=0;
 tcsetattr(sfd, TCSANOW, &options);
 
 char buf[5];
 char buf2[5];

 
 


 buf[0]=0xAB;
 buf[1]=0x16;
 buf[2]=0x17;
 buf[3]=0x18;
 buf[4]=0xFF;
write(sfd, buf,5);
 while(1){
 read(sfd,buf2, 5);
  printf("%s\n\r", buf2);
 };
 printf("%s\n\r", buf2);
 close(sfd);
 return (EXIT_SUCCESS);
}
