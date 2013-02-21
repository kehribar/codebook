/******************************************************************************
 * Learning socket programming ...
 * 
 * This example creates a server 
 * To connect this server use: telnet <server_ip> 8888
 *
 * Compile command: gcc level1.c -o main -lpthread
 *
 * This code is written thanks to the following tutorial:
 * http://www.binarytides.com/socket-programming-c-linux-tutorial/2/
 *
 * ihsan Kehribar - 2013
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

void *connectionHandler(void *arg)
{
	int sock = *(int*)arg;
	int read_size;
	char* message;
	char client_message[2000];

	printf("This is the thread talking!\n");

	message = "I'm going to repeat what you say to me ...\n";
	write(sock,message,strlen(message));

	while(1)
	{
		/* First off, clear the old messages */
		memset(client_message,0x00,sizeof(client_message));
		
		/* Wait until a message receives. */
		read_size = recv(sock,client_message,2000,0);
		
		/* If the size is OK, send back. If not, exit the thread */
		if(read_size > 0)
			write(sock,client_message,strlen(client_message));
		else
			break;
	}
	
	if(read_size == 0)
		printf("Client is disconneted ...\n");
	else if(read_size == -1)
		printf("Receive is failed ...\n");

	free(arg);
	
	printf("Thread exits ...\n");
	printf("------------------------------------\n");
	printf("Waiting for client again ...\n");
	return 0;
}

int main()
{
	/* Thread related definitions and functions */
	pthread_t client_thread;

	/* Socket related definitions and functions */
	int socket_desc;
	struct sockaddr_in server;
	struct sockaddr_in client;

	/* Create the socket */
	socket_desc = socket(AF_INET,SOCK_STREAM,0);
	if(socket_desc == -1)
	{
		printf("Could not create socket!\n");
		return -1;
	}

	/* Socket details ... */
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(8888); /* Spesific port number ... */

	/* Bind ... */
	if(bind(socket_desc,(struct sockaddr*)&server,sizeof(server)) < 0)
	{
		printf("Binding error!\n"); 
		return -1;
	}
	printf("------------------------------------\n");
	printf("Binding OK!\n");
	printf("Waiting for client ...\n");

	/* Start listening to the socket - Maximum 1 pending connection is allowed */
	listen(socket_desc,1);
	
	/* Accept the incoming connections */
	int new_socket;
	int c;
	c = sizeof(struct sockaddr_in);

	while(1)
	{
		new_socket = accept(socket_desc,(struct sockaddr*)&client,(socklen_t*)&c);
		if(new_socket < 0)
		{
			printf("Client accept failed ...\n");
			return -1;
		}
		printf("------------------------------------\n");
		printf("Client is accepted!\r\n");
	
		/* Let's see who is it? */
		char* client_ip = inet_ntoa(client.sin_addr);
		int client_port = ntohs(client.sin_port);
		printf("Client IP: %s\n",client_ip);
		printf("Client port: %d\n",client_port);

		/* If you don't create new pointer -> Segmentation Fault! */
		int* new_sock;
		new_sock = malloc(1);
		*new_sock = new_socket;

		/* Assign the client to its thread */
		if(pthread_create(&client_thread,NULL,connectionHandler,(void*)new_sock) < 0)
		{
			printf("Thread creation error ...\n");
		}
		printf("Client handler is assigned!\n");

	}

	return 0;
}

