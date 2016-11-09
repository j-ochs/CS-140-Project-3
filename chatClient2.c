
#include <pthread.h>
#include <curses.h>
#include <termios.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
 
#define MSG_SIZE 1024 //Size of the max number of characters that can be sent

#define CLIENT_SERVER_BUFFER 1024
#define PORT 49153

int i = 0;
int port;
int server_sockfd, clients_sockfd;
struct sockaddr_in server_address;
int addresslen = sizeof(struct sockaddr_in);
int fd;
fd_set readfds, testfds, clientfds;
char msg[MSG_SIZE +1];	
char kb_msg[MSG_SIZE + 10];

//Client Variables
int sockfd;
int result;
char hostname[MSG_SIZE];
struct hostent *hostinfo;
struct sockaddr_in address;
char alias[MSG_SIZE];
int clientid;

void connectToServer();

int main(){

	//Client, connect to the server
	printf("\n*** Client program starting (enter \"quit\" to stop): \n");
	printf("Please enter the name that you would like to enter the chat room as :\n");
	fflush(stdout);

	//Create a socket for the client
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	//attr
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("10.115.20.250");
	address.sin_port = htons(49153);

	//connect the socket to the server's socket
	if(connect(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0){
		perror("connecting");
		exit(1); //If the program exits herre just change it
	}	

	fflush(stdout);

	FD_ZERO(&clientfds);
	FD_SET(sockfd, &clientfds);
	FD_SET(0, &clientfds); //stdin

	while(1){
		
		testfds = clientfds;
		int sel;
		select(FD_SETSIZE, &testfds, NULL, NULL, NULL);
		
		for(fd=0; fd < FD_SETSIZE; fd++){
			if(FD_ISSET(fd, &testfds)){
				if(fd == sockfd){
					//test when something comes in, grab what is on the keyboard and print it before waht can it
					fgets(kb_msg, MSG_SIZE + 1, stdin);

					//When data comes in from the server first take what is being written in buffer and save somewhere
					//while ((getchar()) != '\n');

					//Clear the buffer

					//Print the result to the console
					result = read(sockfd, msg, MSG_SIZE);
					msg[result] = '\0';
					

					//If the user was typing something then put it back in the buffer as if they are still typing
				}
				else if(fd == 0){
					//grab the data from the keyboard and send it to the serve
					//printf("client - send\n");
					fgets(kb_msg, MSG_SIZE +1, stdin);

					if (strcmp(kb_msg, "quit\n")==0) {
						sprintf(msg, "XClient is shutting down.\n");
						write(sockfd, msg, strlen(msg));
						close(sockfd); //close the current client
						exit(0); //end program
					}
					else{
						//scanf("%*[^\n]%*1[\n]");
						sprintf(msg, "%s", kb_msg);
						write(sockfd, msg, strlen(msg));

					}
						}
				}
			}
			
		}
}

void connectToServer(){


}
