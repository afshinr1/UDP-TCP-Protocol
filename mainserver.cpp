
/* Usage: cc -o wordlen-TCPserver wordlen-TCPserver.c */
/*        ./wordlen-TCPserver                         */
/*                                                    */
/* Written by Carey Williamson       January 13, 2012 */

/* Include files for C socket programming and stuff */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <iostream>

using namespace std;
/* Global manifest constants */
#define MAX_MESSAGE_LENGTH 100
#define MYPORTNUM 12345
#define IDENTITYPORT 12346
#define REVERSEPORT 12347
#define UPPERPORT 12348
#define LOWERPORT 12349
#define CAESARPORT 12350
#define YOURPORT 12351

/* Optional verbose debugging output */
#define DEBUG 1

/* Global variable */
int childsockfd;

/* This is a signal handler to do graceful exit if needed */
void catcher( int sig )
  {
    close(childsockfd);
    exit(0);
  }
  
 string transform(char* messageWord, int identifier){
	struct sockaddr_in si_server;
    struct sockaddr *server;
    int s, i, len = sizeof(si_server);
    char buf[MAX_MESSAGE_LENGTH];
    int readBytes;
	cout << "Sending message to server!======  " << messageWord << endl;
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
      {
	printf("Could not set up a socket!\n");
		exit(1);
      }
      
    memset((char *) &si_server, 0, sizeof(si_server));
    si_server.sin_family = AF_INET;
	
		if(identifier == 1){
    si_server.sin_port = htons(IDENTITYPORT);
	cout << "Recieved message from identity server : ";
	}
		if(identifier == 2){
    si_server.sin_port = htons(REVERSEPORT);
	cout << "Recieved message from reverse server : ";
	}	if(identifier == 3){
    si_server.sin_port = htons(UPPERPORT);
	cout << "Recieved message from upper server : ";
	}	if(identifier == 4){
    si_server.sin_port = htons(LOWERPORT);
	cout << "Recieved message from lower server : ";
	}	if(identifier == 5){
    si_server.sin_port = htons(CAESARPORT);
	cout << "Recieved message from caesar server : ";
	}	if(identifier == 6){
    si_server.sin_port = htons(YOURPORT);
	cout << "Recieved message from your server : ";
	}
	
    server = (struct sockaddr *) &si_server;

    if (inet_pton(AF_INET, "127.0.0.1", &si_server.sin_addr)==0)
      {
	printf("inet_pton() failed\n");
		exit(1);
      }

    fprintf(stderr, "Welcome! I am the UDP version of the client!!\n");

  
	bzero(buf, MAX_MESSAGE_LENGTH);

	strcpy(buf, messageWord);

		
	if (sendto(s, buf, strlen(buf), 0, server, sizeof(si_server)) == -1)
	  {
	    printf("sendto failed\n");
		exit(1);
	  }
	  
struct timeval tv;
tv.tv_sec = 0;
tv.tv_usec = 100000;
setsockopt(s, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv));

	if ((readBytes=recvfrom(s, buf, MAX_MESSAGE_LENGTH, 0, server, &len)) < 0)
	  {
	    printf("Timeout!\n");
		exit(1);
	  }
	  
	buf[readBytes] = '\0';    // proper null-termination of string

	cout << buf << endl;
	
	string returnVal = (string) buf;
    close(s);
	cout << "Sending back " << returnVal << endl;
	return returnVal;
 }
/* Main program for server */
int main()
  {
    struct sockaddr_in server;
    static struct sigaction act;
	string buffer;
    char messagein[MAX_MESSAGE_LENGTH];
    char messageout[MAX_MESSAGE_LENGTH];
	char messageWord[MAX_MESSAGE_LENGTH];
	char messagelength[MAX_MESSAGE_LENGTH];
    int parentsockfd;
    int i, j;
    int pid;
    char c;

    /* Set up a signal handler to catch some weird termination conditions. */
    act.sa_handler = catcher;
    sigfillset(&(act.sa_mask));
    sigaction(SIGPIPE, &act, NULL);

    /* Initialize server sockaddr structure */
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(MYPORTNUM);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    /* set up the transport-level end point to use TCP */
    if( (parentsockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1 )
      {
	fprintf(stderr, "wordlen-TCPserver: socket() call failed!\n");
	exit(1);
      }

    /* bind a specific address and port to the end point */
    if( bind(parentsockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in) ) == -1 )
      {
	fprintf(stderr, "wordlen-TCPserver: bind() call failed!\n");
	exit(1);
      }

    /* start listening for incoming connections from clients */
    if( listen(parentsockfd, 5) == -1 )
      {
	fprintf(stderr, "wordlen-TCPserver: listen() call failed!\n");
	exit(1);
      }

    /* initialize message strings just to be safe (null-terminated) */
    bzero(messagein, MAX_MESSAGE_LENGTH);
    bzero(messageout, MAX_MESSAGE_LENGTH);

    fprintf(stderr, "Welcome! I am the TCP version of the transform server!!\n");
    fprintf(stderr, "server listening on TCP port %d...\n\n", MYPORTNUM);

    /* Main loop: server loops forever listening for requests */
    for( ; ; )
      {
	/* accept a connection */
	if( (childsockfd = accept(parentsockfd, NULL, NULL)) == -1 )
	  {
	    fprintf(stderr, "wordlen-TCPserver: accept() call failed!\n");
	    exit(1);
	  }

	/* try to create a child process to deal with this new client */
	pid = fork();

	/* use process id (pid) returned by fork to decide what to do next */
	if( pid < 0 )
	  {
	    fprintf(stderr, "wordlen-TCPserver: fork() call failed!\n");
	    exit(1);
	  }
	else if( pid == 0 )
	  {
	    /* the child process is the one doing the "then" part */

	    /* don't need the parent listener socket that was inherited */
	    close(parentsockfd);
	if( recv(childsockfd, messageWord, MAX_MESSAGE_LENGTH, 0) > 0)
	{
	cout << "--------Word recieved -----------" << messageWord << endl;	
	}
	    /* obtain the message from this client */
	    while( recv(childsockfd, messagein, MAX_MESSAGE_LENGTH, 0) > 0 )
	      {
		/* print out the received message */
		printf("Child process received transformation: %s\n", messagein);
		printf("That word has %d characters!\n", strlen(messagein));
		string fullMessage = "";
		int choice = atoi(messagein);
	string str = (string) messagein;
	for(char &ch : str){
	choice = (int) ch - '0';
		cout << "CHOICE IN THE TRANSFORMATION STRING " << choice << endl;

		if(choice == 1){
			cout << "Choice 1, identity!" << endl;
			buffer = transform(messageWord, 1);
			strcpy(messageout, buffer.c_str());
		}
	
		if(choice == 2){
			cout << "Choice 2, reverse!" << endl;
			buffer = transform(messageWord, 2);
			strcpy(messageout, buffer.c_str());
		}
	
		if(choice == 3){
			cout << "Choice 3, upper!" << endl;
			buffer = transform(messageWord, 3);
			strcpy(messageout, buffer.c_str());
		}
	
		if(choice == 4){
			cout << "Choice 4, lower!" << endl;
			buffer = transform(messageWord, 4);
			strcpy(messageout, buffer.c_str());
		}
	
		if(choice == 5){
			cout << "Choice 5, caesar!" << endl;
			buffer = transform(messageWord, 5);
			strcpy(messageout, buffer.c_str());
		}
	
		if(choice == 6){
			cout << "Choice 6, yours!" << endl;
			buffer = transform(messageWord, 6);
			strcpy(messageout, buffer.c_str());
		}
		fullMessage = fullMessage + buffer;
		fullMessage = fullMessage + "\n";
		
	}
	cout << "FULL MESSAGE : \n" << fullMessage << endl << endl;
				strcpy(messageout, fullMessage.c_str());
	cout << "LENGTH OF FULL MESSAGE : " << fullMessage.length() << endl;			
	int lenth = fullMessage.length();
	string len = to_string(lenth);
	
	strcpy(messagelength, len.c_str());

		/* create the outgoing message (as an ASCII string) */
	//	sprintf(messageout, "%d\n", strlen(messagein));

		send(childsockfd, messagelength, strlen(messagelength), 0);


		printf("Child about to send message\n: %s\n", messageout);

		/* send the result message back to the client */
		send(childsockfd, messageout, strlen(messageout), 0);

		/* clear out message strings again to be safe */
		bzero(messagein, MAX_MESSAGE_LENGTH);
		bzero(messageout, MAX_MESSAGE_LENGTH);
	      }

	    /* when client is no longer sending information to us, */
	    /* the socket can be closed and the child process terminated */
	    close(childsockfd);
	    exit(0);
	  } /* end of then part for child */
	else
	  {
	    /* the parent process is the one doing the "else" part */
	    fprintf(stderr, "Created child process %d to handle that client\n", pid);
	    fprintf(stderr, "Parent going back to job of listening...\n\n");

	    /* parent doesn't need the childsockfd */
	    close(childsockfd);
	  }
      }
  }