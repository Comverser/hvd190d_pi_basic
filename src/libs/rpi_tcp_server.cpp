#include <stdio.h> // printf(), sprintf(), perror(), etc
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h> // struct sockaddr_in
#include <netdb.h>
#include <string.h> // strlen(), etc
#include <stdlib.h> // atoi(), etc
#include <unistd.h> // POSIX operating system API
#include <errno.h>

void error( char *msg ) 
{
      perror(  msg );
      exit(1);
}

int func( int a ) 
{
    return 2 * a;
}

void sendData( int sockfd, int x ) 
{
    int n;

    char buffer[32];
    sprintf( buffer, "%d\n", x );
    if ( (n = write( sockfd, buffer, strlen(buffer) ) ) < 0 )
        error( const_cast<char *>( "ERROR writing to socket") );
    buffer[n] = '\0';
}

int getData( int sockfd ) 
{
    char buffer[32];
    int n;
  
    if ( (n = read(sockfd,buffer,31) ) < 0 )
        error( const_cast<char *>( "ERROR reading from socket") );
    buffer[n] = '\0';
    return atoi( buffer ); // number only
}

void tcp_server()
{
}

int main(int argc, char *argv[]) 
{
    int sockfd, newsockfd, portno = 58262, clilen; // sockfd is socket descriptor
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    /*
		struct sockaddr_in 
		{
			short            sin_family;   // e.g. AF_INET
			unsigned short   sin_port;     // e.g. htons(3490)
			struct in_addr   sin_addr;     // see struct in_addr, below
			char             sin_zero[8];  // zero this if you want to
		};
		struct in_addr 
		{
			unsigned long s_addr;  // load with inet_aton()
		};
     */
    int n;
    int data;

    printf( "using port #%d\n", portno );

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET => IPv4 protocol, SOCK_STREAM => TCP
    if (sockfd < 0) 
        error( const_cast<char *>("ERROR opening socket") );
    bzero((char *) &serv_addr, sizeof(serv_addr)); // bzero() : memset to zero, legacy

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY : to work without knowing the IP address of the machine it is running on
    serv_addr.sin_port = htons( portno ); // htons() : most significant byte first
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error( const_cast<char *>( "ERROR on binding" ) );
    listen(sockfd,1); // the backlog means no. of connecion
    clilen = sizeof(cli_addr);

    //--- infinite wait on a connection ---
    while ( 1 ) 
    {
        printf( "waiting for new client...\n" );
        if ( ( newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, (socklen_t*) &clilen) ) < 0 )
            error( const_cast<char *>("ERROR on accept") );
        printf( "opened new communication with client\n" );
        while ( 1 ) 
        {
            //---- wait for a number from client ---
            data = getData( newsockfd );
            printf( "got %d\n", data );
            if ( data < 0 ) 
                break;

            data = func( data );

            //--- send new data back --- 
            printf( "sending back %d\n", data );
            sendData( newsockfd, data );
        }
        close( newsockfd );

        //--- if -2 sent by client, we can quit ---
        if ( data == -2 )
            break;
    }
    return 0; 
}
