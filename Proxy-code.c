//Name: Ana Bindiu
//Built a proxy that blocks pages based on their URL/HTTP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <ctype.h>
#include <time.h>
#include <sys/socket.h>

//Functions called from below
int server_for_proxy(struct sockaddr_in server);
int check_for_blocked_word(int i);
void exit_function();

//GLOBAl VAIRABLES
int num2 = 0;
int array_for_blocked_words[4][20];
int socket2_byte;
int byte;
int tcp_socket;
int num3;
int byte2;
char client_message[2000];
char address_web[2000];
char web_url[2000];
char web_host[2000]; 
char http_request[2000];
char web_path[2000];
char web_header[2000];
char server_response[2000];	
int port_number;
int socket3;
int socket2;
int bindStatus;
int socket_desc;
int socket_desc1;
int listeningServer;




int main(int argc, char* argv[]){
    //Check for the port number from the client 
    //If the port number matches the requirements, proceed onward
    //Otherwise exit
    if(argc > 1 && (port_number >= 1025 || port_number <= 65535)){
        port_number = 0;
        for (int i = 0; argv[1][i] != '\0'; i++){
			port_number = port_number*10 + (argv[1][i] - '0');
        }
    }
    else if(argc > 1 && (port_number < 1025 || port_number > 65535)){
		exit(0);
    }

    else{
        printf("\nPlease enter a port number between 1025-65535 into the second argument!\n");
		exit(0);
    }


    //CREATE A SERVER FOR PROXY
    struct sockaddr_in server;
    socket_desc = server_for_proxy(server);



    //Code given to us in tutorial for A0, specifically: lines 91-212
    memset(&server, 0, sizeof(server));

    // Socket Creation
    socket_desc1 = socket(PF_INET, SOCK_STREAM, 0);
    if(socket_desc1 == -1){
        //print the error client_message
        printf("Could not create socket\n");
        exit(-1);
    }
    puts("Socket created\n");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_port = htons(port_number+1);
    server.sin_addr.s_addr = htonl(INADDR_ANY );


    // Bind
    bindStatus = bind(socket_desc1, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
    if(bindStatus == -1){
        printf("Binding failed\n");
        exit(-1);
    }

    struct timeval clock;
    clock.tv_sec = 5;
    clock.tv_usec = 0;

    if(setsockopt(socket_desc1, SOL_SOCKET, SO_RCVTIMEO, (char*)&clock, sizeof(clock))<0){
        printf("Couldn't make a socket for time\n");
    }

    //Listen
    //Create a timeout
    listeningServer = listen(socket_desc1, 100);
    if(listeningServer != -1){
        printf("You have 5 seconds to connect.\n ");
        socket3 = accept(socket_desc1,NULL,NULL);
        if(socket3 != -1){
            printf("\nYOU ARE NOW CONNECTED\n");
        }
        else{  
            printf("Time ran out!\n Try connecting again!\n");
            exit(-1);       
        }
    }
    else{
        printf("Listening failed\n");
        exit(-1);
    }

		

    char array_for_blocked_words[4][20];
    printf("Reload the page\n") ;
    while(1){
        if((listeningServer != -1) && (socket3 != -1))    
        {
            if((byte = recv(socket3, client_message, 2000, 0)) <= 0){
                printf("\nPlease type : BLOCK + [word] to block something or UNBLOCK to unblock everything\n\n");
            }
            else{
		        int counterForLoop = 0;
                //Loop through the word BLOCK to make sure it matches the client's request
                char block_array[5] = { 'B',  'L',  'O',  'C',  'K'};
                int flag_block = 0;
                for(int i = 0; i < 5; i++){
                    if(client_message[i] == block_array[i]){
			            counterForLoop++;
			            if(counterForLoop == 5){
                        	flag_block = 1;
                        }
                    }
                    else{
                        flag_block = 0;
                    }
                }

                //Loop through the word UNBLOCK to make sure it matches the client's request
                char unblock_array[7] = { 'U',  'N',  'B',  'L',  'O', 'C', 'K'};
                int flag_unblock = 0;
		        counterForLoop = 0;
                for(int i = 0; i < 7; i++){
                    if(client_message[i] == unblock_array[i]){
			            counterForLoop++;
			            if(counterForLoop == 5){
                        	flag_unblock = 1;
                        }
                    }
                    else{
                        flag_unblock = 0;
                    }
                }

                if(flag_block == 1){
                    num3 = 0;
                    if(client_message[num3+6]!='\n'){
                        if(client_message[num3+6]!='\r'){	
                            array_for_blocked_words[num2][num3] = client_message[6+num3]; 
                            num3++;
                        }
                    }

                    array_for_blocked_words[num2][num3] = '\0';	
                    num2++;
                    
                continue;
                }
                else if(flag_unblock == 1){
                    if(num2>0)
                    {
                        num2--;	
                    }
                }

                //if the client doesnt type BLOCK or UNBLOCK but types something else, display a message
                else if((flag_block != 1) && (flag_unblock != 1)){
                    printf("\nYou typed something invalid!.\nPlease type : BLOCK + [word] to block something or UNBLOCK to unblock everything\n\n");
                }
            }
		}

        //Accept the socket
        socket2 = accept(socket_desc, NULL, NULL);
        if(socket2 == -1)
        {
            printf("Could not accept socket\n");
            exit(0);
        }
        printf("Socket accepted");


        //Multi-threading
        int thread = fork();
        if(thread == 0){
            close(socket_desc);         
            if((socket2_byte = recv(socket2, address_web, 2000, 0))>0)
            {
                //check for Link/ image web_url
                char *link_of_web = strtok(address_web, "\r\n");  
                sscanf(link_of_web, "\nGET http://%s\n", web_url)==1;

                int first = 0;
                while(first<strlen(web_url)){
                    if(web_url[first] == '/')
                    {                               
                        strncpy(web_host, web_url, first);      
                        web_host[first] = '\0';           
                        break;
                    }     
                    first++;
                }
                bzero(web_path, 500); 
                
                int second = strlen(web_host);
                while(second<strlen(web_url)){
                    strcat(web_path, &web_url[second]);
                    break;
                    second++;
                }

                //add it all together

                //Call function check_for_blocked_word
                //if the function returns 1, desplay the error message
                if(check_for_blocked_word(strlen(web_host)) == 1)
                {
                    printf("\nYou cannot access this page, because this page has a bad word!\n");
                    strcpy(web_path, "/~carey/CPSC441/ass1/error.html");
                }


                //Create a TCP socket   
                //Code taken/inspired from tutorial notes 
                struct sockaddr_in serverAddr;
                struct hostent *server;

                int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
                if(tcp_socket >= 0){
                    printf("Socket creation was successfull! \n");
                }

                // get server info
                server = gethostbyname(web_host);
                if(server != NULL){
                    printf("Web Server = %s\n", server->h_name);
                }
                
                //Initialize the socket structure
                memset(&serverAddr, 0, sizeof(serverAddr));
                serverAddr.sin_family = AF_INET;

                bcopy((char *) server->h_addr, (char *) &serverAddr.sin_addr.s_addr, server->h_length);
                serverAddr.sin_port = htons(80);

                if(connect(tcp_socket, (struct sockaddr *) &serverAddr, sizeof(serverAddr))<0)
                    printf("Couldn't connect the TCP");


                //Create HTTP http_request
                //Code taken/inspired from tutorial notes
                if(tcp_socket>0){
	                memset(&http_request, 0, sizeof(http_request));
                    //bzero(http_request, 2000);
                    sprintf(http_request, "GET %s HTTP/1.1\r\nHOST: %s\r\n\r\n", web_path, web_host);

                    if(send(tcp_socket, http_request, strlen(http_request), 0)<0)
                        printf("Couldn't send a http_request");
                }
                else printf("Failed TCP socket");

                bzero(web_header, 2000);
                byte2 = 0;

                // Finding the information of the HTTP
                while((byte2 = read(tcp_socket, server_response, 2000))) 
                {
                    send(socket2, server_response, byte2, 0);
                    bzero(address_web, 2000);
                    bzero(server_response, 2000);
                }
            }

            //Close everything
            close(socket2);
            exit(0);
        } 

        //If thread is > 0, close the socket
        else if(thread>0){
            close(socket2);
        }

        //Exit if thread <0
        else{
            printf("\nCouldn't create a thread\n");
            exit(0);
        }
    }
    return 0;
}

//Function: server_for_proxy
//
//input: struct sockaddr_in server
//output: socket_desc
//
//Description: this function deals with the server for the proxy.
///* Creating the socket so that proxy acts as a server for the client */
//
//This code was given to us in tutorial for A0
int server_for_proxy(struct sockaddr_in server){
    memset(&server, 0, sizeof(server));

    //Create socket
    socket_desc = socket(PF_INET, SOCK_STREAM, 0);
    if(socket_desc == -1){
        //print the error client_message
        printf("Could not create socket\n");
        exit(-1);
    }
    puts("Socket created\n");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_port = htons(port_number);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    //Bind 
    bindStatus = bind(socket_desc, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
    if(bindStatus == -1){
        //print the error client_message
        printf("Binding failed\n");
        exit(-1);
        
    }

    //Listen
    listeningServer = listen(socket_desc, 5);
    if(listeningServer == -1){
        printf("Listening failed\n");
        exit(-1);
    }
    printf("Listening done.\n");
    //Accept and incoming connection
	printf("Waiting for clients...\n");
    return socket_desc;
}


//Function: check_for_blocked_word
//
//input: int i, char web_url
//output: int that's either 1 or 0
//
//Description: this function checks to see if the word we want to block is in the url link; if it is it outputs a 1
int check_for_blocked_word(int i){
    int inappropriate_word, j, z, x, y = 0; 
    printf("There are %d blocked words right now\n", num2);
    while(z<num2){
        j = i;
        while(j<strlen(web_url)){
            if(web_url[j] == array_for_blocked_words[z][y]){
                x++;
                y++;
            }
            j++;
        }

        if( x == sizeof(array_for_blocked_words[z][0])){
            inappropriate_word = 1;
            return inappropriate_word;
        }
        else{
            return 0;
        } 
        z++;
    } 
}
