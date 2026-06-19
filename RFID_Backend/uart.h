#ifndef _UART_H_
#define _UART_H_

int   serialOpen      (const char *device, const int baud) ;
void  serialClose     (const int fd) ;
void  serialFlush     (const int fd) ;
void  serialPutchar   (const int fd, const unsigned char c) ;
void serialPuts(int fd,const char *str);
int   serialGetchar   (const int fd) ;
int serialGetString(int fd, char *buffer);

#endif
