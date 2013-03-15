/******************************************************************************
 * Simple socket client example 
 *
 * Compile command: gcc level2.c -o main -lpthread
 *
 * ihsan Kehribar - 2013
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>	
#include <string.h>	
#include <sys/socket.h>	
#include <arpa/inet.h>
#include <stdint.h>
#include <pthread.h>

volatile char buff[128];	
volatile int newMessageFlag;

void *threadFunction(void *arg)
{	
	int i;
	char temp;

	while(1)
	{
		i = 0;
		temp = 0;
		while((temp!='\n')&(i<(sizeof(buff)-1)))
		{
			temp = getchar();
			buff[i] = temp;
			i++;
		}
		newMessageFlag = 1;
	}		
}

int main(int argc , char *argv[])
{
	int sock;
	int16_t value;
	int errCount = 0;
	FILE* loglogfile;
	int16_t value_bef;
	pthread_t thread_1;
	int packetCount = 0;
	struct sockaddr_in server;
	char message[1000] , server_reply[2000];

	pthread_create(&thread_1,NULL,threadFunction,NULL);	

	if(argc==1)
	{
		printf("Please write the server IP\n");
		return 0;
	}

	char* ip_address = argv[1];
	printf("Server ip address: %s\n",ip_address);

	loglogfile = fopen("logfile.txt","w");
	if(loglogfile == NULL)
	{
		printf("Couldn't create the log file!\n");
		return -1;
	}
	fprintf(loglogfile, "Hi!\n");
	
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr(ip_address);
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}
	
	puts("Connected\n");
	
	newMessageFlag = 0;

	while(1)
	{
		if(newMessageFlag)
		{
			newMessageFlag = 0;
					
			//Send some data
			if( send(sock , buff , strlen(buff) , 0) < 0)
			{
				puts("Send failed");
				return 1;
			}
		}
		
		memset(server_reply,0x00,sizeof(server_reply));

		//Receive a reply from the server
		if( recv(sock , server_reply , sizeof(server_reply) , 0) < 0)
		{
			puts("recv failed");
			break;
		}
		
		packetCount++;;
		value_bef = value;
		sscanf(server_reply,"%d",&value);		
		if((value - value_bef) != 0x01)
		{
			errCount++;
			fprintf(loglogfile,"Value: %d Value_bef: %d Server reply: %s\n",value,value_bef,server_reply);		
			printf("Value: %d Value_bef: %d Server reply: %s\n",value,value_bef,server_reply);		
			fflush(loglogfile);		
		}
		printf("total: %d loss: %f\n",packetCount,100*(float)errCount/packetCount);

	}
	
	fclose(loglogfile);
	close(sock);

	return 0;
}