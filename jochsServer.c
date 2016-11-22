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
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <sys/timerfd.h>
#include <pthread.h>
#include <curl/curl.h>

#define PORT 28900



/**
* this function is an abstraction of our libcurl http get request
* which will report to pilot when we have found a victim with:
* @their
**/
void *libcurlVictim(char *their_username, char *their_location){
    char part1[128]; //eg: "http://pilot.westmont.edu:28900?i=jochs&u="
    char part1a[128];
    char part2[128]; //eg: "&where="
    char part2a[128];
    char part3[128];
    strcat(part1, "http://pilot.westmont.edu:28900?i=jochs&u=");
    strcat(part1,their_username);
    strcat(part2, "&where=");
    strcat(part1, part2);
    strcat(part2a, their_location);
    strcat(part1, part2a);
    printf("part1:%s",part1);


    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
/* send the libcurl http get request to pilot with: our username, our victim, and their location*/
        // a sample: http://pilot.westmont.edu:28900?i=jochs&u=rwilder&where=vl-1a-wap3
        curl_easy_setopt(curl, CURLOPT_URL, part3);
        #ifdef SKIP_PEER_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        #endif

        #ifdef SKIP_HOSTNAME_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        #endif

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

        /* always cleanup */
      curl_easy_cleanup(curl);

    }
}


/**
* this function runs a continual 60 sec timer 
* which will then execute a libcurl http get
* request using i=jochs (our username) and 
* uptime=60 (60 sec program uptime)
* @params x is a void pointer
*/
void *myTime(void *x)
{
    while (1){
        sleep(60); //60 sec timer sleep in the infinite loop
        CURL *curl;
        CURLcode res;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if(curl) {
/* send the libcurl http get request to pilot's webpage, with jochs + 60 sec uptime*/
            curl_easy_setopt(curl, CURLOPT_URL, "http://pilot.westmont.edu:28900/?i=jochs&uptime=60");
            #ifdef SKIP_PEER_VERIFICATION
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            #endif

            #ifdef SKIP_HOSTNAME_VERIFICATION
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
            #endif

        /* Perform the request, res will get the return code */
            res = curl_easy_perform(curl);
        /* Check for errors */
            if(res != CURLE_OK)
              fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));

        /* always cleanup */
          curl_easy_cleanup(curl);

      }
    }//the end of our infinite while loop   
  return NULL; //should never get here
}


/**
* calls find (located in chatclient2.c) to search for 
* a server with port 28900 running on westmont encrypted
* @params x is a void pointer
*/
    void *callFind(void *x){
        while(1){
            sleep(60);
            //find();
        }
        return NULL; //should never get here (outside infinite loop)
    }





int main(int argc , char *argv[])
{
    /* declare variables */
    char *whois = "Who is this?";
    int socket_desc , client_sock , c , read_size, timerfd;
    struct sockaddr_in server , client;
    char client_message[2000];
    fd_set clientfds;

    /* create a thread to run our timer */
    pthread_t threads[2];
    int thread_args[2];
    int rc, i;
    /* spawn the thread which will run the 60 sec timer recursively*/
    for (i=0; i<2; ++i)
    {
        thread_args[i] = i;
        rc = pthread_create(&threads[0], NULL, myTime, (void *) &thread_args[0]);
       // rc = pthread_create(&threads[1], NULL, callFind, (void *) &thread_args[1]);

    }

    
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
    server.sin_port = htons(PORT);
    
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




    // a test call for when we have found a victim to report to pilot
     libcurlVictim("bminer","00:18:0a:79:d5:10");


    while(1){


//select(0, NULL, NULL, NULL, &sleeptime);
//printf("select sleep\n");


        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
        if (client_sock < 0){
            perror("accept failed");
            return 1;
        }
        puts("Connection accepted");

    //your username with a space at the end -- don't delete the ending space
        char id_space[30];
        strcat(id_space, "jochs ");

    //Receive a message from client   
        while((read_size = recv(client_sock , client_message , 2000 , 0)) > 0)
        {
            if (strcmp(client_message,whois) != 0){
            //run terminal command to get your network location (access point)
                FILE *cmd = popen("arp -a|grep 128|awk '{print $4}'", "r");
            //send username + " " to the connected client
                write(client_sock, id_space, strlen(id_space));
                printf("Username and network location have been sent\n");
                //buf = allocated buffer for our network location
                char buf[18];
                while (fgets(buf, sizeof(buf), cmd) != NULL)
                {
                    //send your network location to the connected client
                    write(client_sock, buf, strlen(buf));
                }
                pclose(cmd);
            }
        *id_space = '\0'; //clear the username string for the next connection
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
   // FD_ZERO(client_sock);
    //return 0;
}
}