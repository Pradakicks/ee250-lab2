/* A simple server in the internet domain using TCP
 * Answer the questions below in your writeup
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    /* 1. What is argc and *argv[]?
     * In C, this is how the program gets inputs from the cmdl. Argc tells us how many arguments there are and argv is the char array that holds the arguments.
     */
    int sockfd, newsockfd, portno;
    /* 2. What is a UNIX file descriptor and file descriptor table?
     * The file descriptor is a process-unique identifier for a file (wiki)
     * the fd table maps each fs to a open file description that contains data on the file
     */
    socklen_t clilen;

    struct sockaddr_in serv_addr, cli_addr;
    /* 3. What is a struct? What's the structure of sockaddr_in?
     * Its a type in C that allows the user to group other variables under one variable. 
  	* struct sockaddr_in {
        *   sa_family_t     sin_family;       
        *   in_port_t       sin_port;          
        *   struct in_addr  sin_addr;          
      * };
     * 
     */
    
    int n;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    /* 4. What are the input parameters and return value of socket()
     * domain, type, protocol and returns the socket file descriptor or -1 for an error.
     */
    
    if (sockfd < 0) 
       error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0) 
             error("ERROR on binding");
    /* 5. What are the input parameters of bind() and listen()?
     * bind accepts sockfd, addr, addrlen
* int bind(int sockfd,
         const struct sockaddr *addr,
         socklen_t addrlen);
     * listen accepts sockfd and backlog
int listen(int sockfd, int backlog);
     */ 
    
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    
    while(1) {
        /* 6.  Why use while(1)? Based on the code below, what problems might occur if there are multiple simultaneous connections to handle?
        * So we can keep on accepting connections even after one was closed. If two clients connect only one will get the connection. The other client will hand until the first connection is closed.
        */
        
	char buffer[256];
        newsockfd = accept(sockfd, 
                    (struct sockaddr *) &cli_addr, 
                    &clilen);
	/* 7. Research how the command fork() works. How can it be applied here to better handle multiple connections?
         * It will create a new process at this point, and allow us to get another open socket so a different client may connect 
         */
        
	if (newsockfd < 0) 
             error("ERROR on accept");
	bzero(buffer,256);
        
	n = read(newsockfd,buffer,255);
        if (n < 0) 
            error("ERROR reading from socket");
        //printf("Here is the message: %s\n",buffer);
        n = write(newsockfd,"I got your message",18);
        if (n < 0) 
            error("ERROR writing to socket");
        close(newsockfd);
    }
    close(sockfd);
    return 0; 
}
  
/* This program makes several system calls such as 'bind', and 'listen.' What exactly is a system call?
 * A system call is the api that the kernel provides so that user-space programs can "ask" the kernel to perform some task like IO.
 */
