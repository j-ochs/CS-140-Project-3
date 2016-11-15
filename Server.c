/*
	This is a simple echo server.  This demonstrates the steps to set up
	a streaming server.
 
 //Run using gcc Project3Server and run a.out
 //Telnet is telnet localhost 28900


 C socket server example
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>    
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <unistd.h>    


 #define WESTMONT_ID "ahumphrey\n"

 /* function declaration */
void addip(char* name);

int main(int argc , char *argv[])
{
    char *westmont_id = "ahumphrey\n";
    char *whois = "Who are you?\n";
    char *invalid = "Sorry! You have entered an invalid request. Please Try again!\n";
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];

    //Array can hold a maximum of 20 different ip addresses to refuse connections in 15 minute intervals
    char refuseArray[20][16];
    int refuseInt = 0;
    
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 28900 );
    
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
    
    //Listen
    listen(socket_desc , 3);
    
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    
    puts("Connection accepted");
    printf("The ip address of the person connected is: %s\n", inet_ntoa(client.sin_addr));
    printf("The port is: %d\n", (int) ntohs(client.sin_port));

    //Once this connection is made add their ip address to refusal array, after 15 mins remove it
    char str[16];
    sprintf(str, "%d", client.sin_addr);
    addip(str);
    
    //Receive a message from client

    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
        if (strcmp(client_message,whois) == 0){
            //system("arp -a"); //This is where we execute the terminal command arp -a
            //Send username jochs back to client
            write(client_sock, westmont_id, strlen(westmont_id)); //send username
        }
        else{
            //Send back invalid
            write(client_sock , invalid , strlen(invalid));
        }

        //Flush the buffer
        fflush(stdout);

        
    }
    
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
    
    return 0;
}

/* function adding the given ip address to the array of ip address that can not currently connect to server */
void addip(char* name) {

   printf("This is ip address that was just passed to the function%s\n", name);
}
