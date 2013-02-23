/******************************************************************************
 * Linux serial library
 * 
 * Mostly based on: https://github.com/mrshu/serialib
 ******************************************************************************/
#include "serialib.h"

int serial_open(serial **s, char* port, const unsigned int baud)
{
	*s = (serial *) malloc(sizeof(serial));

	(*s)->port = (char *) malloc((strlen(port) + 1) * sizeof(char));
	strcpy((*s)->port, port);
	
	struct termios options;
	
	(*s)->fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
	
	if ((*s)->fd == -1)
		return -1;
	
	fcntl((*s)->fd, F_SETFL, FNDELAY);	
	
	tcgetattr((*s)->fd, &options);
	bzero(&options, sizeof(options));
	speed_t speed;
	switch (baud){
		case 110  :     speed=B110; break;
		case 300  :     speed=B300; break;   
		case 600  :     speed=B600; break;
		case 1200 :     speed=B1200; break;
		case 2400 :     speed=B2400; break;
		case 4800 :     speed=B4800; break;
		case 9600 :     speed=B9600; break;
		case 19200 :    speed=B19200; break;
		case 38400 :    speed=B38400; break;
		case 57600 :    speed=B57600; break;
		case 115200 :   speed=B115200; break;
	}
	
	cfsetispeed(&options, speed);
   cfsetospeed(&options, speed);

	options.c_cflag |= ( CLOCAL | CREAD |  CS8);
	options.c_cc[VTIME]=0;			
	options.c_cc[VMIN]=0;			
	tcsetattr((*s)->fd, TCSANOW, &options);
	return 0;
}

int serial_read_char(serial *s, char *p)
{
	return (read(s->fd, p, 1) == 1) ? 0 : -1; 
}

int serial_read(serial *s, char *buf, char eol, unsigned int len)
{
	int i = 0;
	while (i < len-1) {
		int j = serial_read_char(s, &buf[i++]);
		
		if (j != 0){
			buf[i-1] = '\0';
			return -1;
		}

		if (buf[i-1] == eol){
			buf[i] = '\0';
			return 1;
		}
	}

	buf[i] = '\0';
	return 1;
}

int serial_write_char(serial *s, char p)
{
	return (write(s->fd, &p, 1) == 1) ? 0: -1;
}

int serial_write(serial *s, char* str)
{
	int len = strlen(str);
	if (write(s->fd, str, len) != len)
		return -1;
	return 0;
}
