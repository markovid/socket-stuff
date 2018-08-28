/***********************************
* Name: Dylan Markovic
* Class: CS372
* Assignment: Project 1
* Due Date: 2.11.2018
************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#define MESSAGE_LENGTH 501
/*
 * createSocket takes a struct addrinfo* as an argument and
 * returns the socket field descriptor integer
 * it creates a socket, and prints an error message if
 * the socket could not be created
*/
int createSocket(struct addrinfo* results){
	int sockfd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
	if(sockfd < 0)
		printf("Could Not Open Socket\n");
	return sockfd;
}

/*
 * initializeAddrinfo takes strings for serverName and portNumber as arguments
 * it creates an addrinfo (hints) and sets it up for a TCP connection
 * it then uses getaddrinfo to pass the info from args and hints to
 * the results addrinfo pointer.  results* is the return value.
*/
struct addrinfo* initializeAddrinfo(char* serverName, char* portNum){
	struct addrinfo hints, *results;
	
	bzero(&hints, sizeof(hints));  //clean out hints of junk data
	
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM; //we're using TCP
	if(getaddrinfo(serverName, portNum, &hints, &results) != 0){
		printf("Invalid IP address or Port Number!\n");
		exit(0);
	}
	return results;
}

/*
 * createConnection takes the socket field descriptor int and 
 * the addrinfo* set up for the TCP connection as args.  It then
 * creates the connection and prints an error if the connection
 * was not successful
*/
void createConnection(int sockfd, struct addrinfo* results){
	if(connect(sockfd, results->ai_addr, results->ai_addrlen) < 0){
		printf("Failed to Connect!\n");
		exit(0);
	}
}

/*
 *getClientName takes a string as a parameter
 * it prompts the user to enter a name, and then stores the 
 * first 10 chars into the string it was passed, making sure the last
 * element is a '\0'
*/
void getClientName(char* input){
		bzero(input, sizeof(input)); //clean input string
		printf("Enter your name, it must be less than 10 characters long\n");
		printf("Anymore than 10 characters will be cut off of the name >>");
		scanf("%s", input);  //put input into string
		input[10] = '\0';
}

/*
 * nameExchange takes the socket fd integer, the client's name string
 * and the server's name string as parameters.  It sends the client's name
 * to the connected host, and then saves the received message as the server's name
 * Finally, it informs the user who the chat has begun with
*/
void nameExchange(int sockfd, char* clientName, char* serverName){
	bzero(serverName, sizeof(serverName));
	send(sockfd, clientName, strlen(clientName), 0);
	recv(sockfd, serverName, 10, 0);
	printf("Chat with %s has begun\n", serverName);
}

/*
 * chat takes the socket fd integer, the client's name string,
 * the server's name string as parameters.  it sends client's message, 
 * checking for errors.  It also receives the server's messages and checks
 * this for erros as well
*/
void chat(int sockfd, char* clientName, char* serverName){
	int sendCheck, recvCheck;
	char inBuffer[501], outBuffer[501];
	fgets(inBuffer, 501, stdin);
	//inBuffer[strcspn(inBuffer, "\n")] = 0;  //remove trailing new lines
	//begin chat loop
	while(1){
		//buffers clean up
		bzero(inBuffer, sizeof(inBuffer));
		bzero(outBuffer, sizeof(outBuffer));
		//sending
		printf("%s> ", clientName);
		fgets(inBuffer, 500, stdin);//get the message
		
		//client wants to quit chatting
		if (strcmp(inBuffer,"\\quit\n") == 0)
			break;
		sendCheck = send(sockfd, inBuffer, strlen(inBuffer), 0);
		
		//if sending error quit
		if (sendCheck == -1){
			printf("Error in Sending Message\n");
			exit(0);
		}
		

		
		//printf("%s> %s", clientName, inBuffer);
		//fgets(outBuffer, 501, stdin);  //clean out buffer
		
		//receiving
		recvCheck = recv(sockfd, outBuffer, 500, 0);  //get message
		//check received message
		if (recvCheck == 0){//server closed chat
			printf("The connection has been closed\n");
			close(sockfd);
			return;
		} else if (recvCheck < 0){//problem with message
			printf("There was an error in receiving the message\n");
			return;
		}else{	printf("%s\n", outBuffer); }  //its all good, print the message
	}
	close(sockfd);//close connection
	printf("The connection has been terminated\n");  //let chatter know
	return;
}

/*
 * main drives the whole program.  it uses the functions to create the
 *client-side chat in its entirety
*/
int main( int argc, char *argv[]){
	int hostPortNumber, sockfd;
	struct hostent* server;
	char buffer[MESSAGE_LENGTH];
	struct sockaddr_in *serverAddr, *clientAddr;
	
	//check arg number
	if (argc != 3){
		printf("Invalid Argument Count\n");
		printf("Format: %s <host name> <port number>\n", argv[0]);
		exit(0);
	}
	
	hostPortNumber = atoi(argv[2]); // convert string port num to int
	
	//make sure valid port number
	if (hostPortNumber < 1024 || hostPortNumber > 65535){
		printf("Invalid port number value. Choose from range [1024, 65535]\n");
		exit(0);
	}
	
	//ready to get the chat going
	char clientName[11], serverName[11];
	getClientName(clientName);  //name collected
	struct addrinfo* results = initializeAddrinfo(argv[1], argv[2]);  //addrinfo set
	sockfd = createSocket(results);  //socket made
	createConnection(sockfd, results);  // connection attempted
	nameExchange(sockfd, clientName, serverName);  //names traded
	chat(sockfd, clientName, serverName);  //time to chat
	

	
	freeaddrinfo(results); //free linked list

}

/***********************
References used in created the program:
Beej's Guide to Network Programming
reference for removing trailing \n's\nhttps://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
************************/