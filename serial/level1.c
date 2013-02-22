/******************************************************************************
 * Linux serial port example
 *
 * compile with: gcc level1.c serialib.c -o main
 *****************************************************************************/
#include <stdio.h>
#include "serialib.h"

int main(void)
{
	serial *s;
	if (serial_open(&s, "/dev/ttyUSB0", 115200) == 0){
		printf("Port opened.\n");

	} else {
		printf("Problem with port opening\n");
		return -1;
	}
	
	char buffer[128];
	while(1)
	{
		serial_read(s, buffer, '\n', 128);
		if(strlen(buffer) > 0)
			printf("%s", buffer);
	}

}
