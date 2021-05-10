#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#define uzunlukHaber 50
#define haberlesme_start 0xab
 

int check_sum_hesapla(u_int8_t *veri)
{
  u_int8_t check_sum_hesapla_sonucu=0x00; 
  for(int i=0;i<uzunlukHaber-1;i++)
  {
  check_sum_hesapla_sonucu = check_sum_hesapla_sonucu+veri[i];
  printf("%x",veri[i]);
  }
  printf("%x\n",check_sum_hesapla_sonucu);
  return check_sum_hesapla_sonucu;
}
/*
void veri_gonder(u_int8_t data_gonder)
{
  data_gonder[uzunluk-1]=check_sum_hesapla(data_gonder);
  u_int8_t count = write(sfd, data_gonder,uzunlukHaber);
  printf("\nveri gönderildi\n");
  
}*/
void veri_al(u_int8_t *data_al)
{
  u_int8_t genel_veri_check_sum;
  genel_veri_check_sum=check_sum_hesapla(data_al);
  if (genel_veri_check_sum==data_al[uzunlukHaber-1])
  {
    printf("veri alındı\n");
    for(int i=0;i<uzunlukHaber-1;i++)
    {
      printf("%x\t",data_al[i]);
    }
  }
  //else {printf("\nveri almak başarısız\n");}

}

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
 //u_int8_t kontrol_bit=0x00;
  buf[0]=haberlesme_start;
 buf[1]=0x00;
 buf[2]=0x00;
 buf[3]=0x01;
  buf[uzunlukHaber-6]=0x02;//4
  buf[uzunlukHaber-5]=0x02;//5
  buf[uzunlukHaber-4]=0x02;//6
  buf[uzunlukHaber-3]=0x01;//7 
 //buf[uzunlukHaber-1]=check_sum_hesapla(buf); 
 // kontrol_bit=check_sum_hesapla(buf);
 // buf[uzunlukHaber-1]=kontrol_bit;
 buf[uzunlukHaber-1]=check_sum_hesapla(buf);
 u_int8_t count = write(sfd, buf,uzunlukHaber);
 // u_int8_t notasyon_kontrol=0;
 while(1){
  count = read(sfd, buf2, uzunlukHaber);
  
  if(count!=0)
  {
    if(buf2[0]==haberlesme_start)
     {
       printf("STM ile haberleşme başladı\n");
       veri_al(buf2);
       
       }
    count=0;
  }
  count = write(sfd, buf,uzunlukHaber);
   // printf("000\n");
    
    
  }
 close(sfd);
 return (EXIT_SUCCESS);
}
