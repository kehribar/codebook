/******************************************************************************
 * Linux serial port example
 *
 * compile with: gcc level1.c serialib.c -o main
 *****************************************************************************/
#include <stdio.h>
#include "serialib.h"

int main(void)
{
	char buffer[128];
	serial *s;

	if (serial_open(&s, "/dev/ttyUSB0", 115200) == 0)
	{
		printf("Port opened.\n");

	} 
	else 
	{
		printf("Problem with port opening\n");
		return -1;
	}
	
	while(1)
	{
		serial_read(s, buffer, '\n', sizeof(buffer));
		if(strlen(buffer) > 0)
			printf("%s", buffer);
	}

	return 0;
}
