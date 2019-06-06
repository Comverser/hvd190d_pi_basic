#include "rpi_tcp_server.h" 

#include <stdio.h> // printf(), sprintf(), perror(), etc
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h> // struct sockaddr_in
#include <netdb.h>
#include <string.h> // strlen(), strncmp(), etc 
#include <stdlib.h> // atoi(), etc
#include <unistd.h> // POSIX operating system API
#include <errno.h>

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

namespace rpi
{
    //private
    int sockfd; // sockfd is socket descriptor
    int newsockfd;

    void error( char *msg ) 
    {
          perror(  msg );
          exit(1);
    }

    //public
    unsigned char data_tcp[size_buffer];

    void init_tcp_server() // socket creation
    {
        const int portno = 58262;
        struct sockaddr_in serv_addr;

        printf( "using port #%d\n", portno );

        sockfd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET => IPv4 protocol, SOCK_STREAM => TCP

        if (sockfd < 0) 
            error( const_cast<char *>("ERROR opening socket") );

        bzero((char *) &serv_addr, sizeof(serv_addr)); // bzero() : memset to zero, legacy
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY : to work without knowing the IP address of the machine
        serv_addr.sin_port = htons( portno ); // htons() : most significant byte first
        if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
            error( const_cast<char *>( "ERROR on binding" ) );
        listen(sockfd,1); // the backlog means no. of connecion
    }

    void wait_connection_tcp()
    {
        struct sockaddr_in cli_addr;
        int clilen = sizeof(cli_addr);

        printf( "waiting for new client...\n" );
        if ( ( newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, (socklen_t*) &clilen) ) < 0 )
        {
            error( const_cast<char *>("ERROR on accept") );
        }
        printf( "opened new communication with client\n" );
    }

    void data_reset_tcp()
    {
        memset(data_tcp, 0, sizeof(data_tcp));
        data_tcp[0] = '\0';
    }

    void run_tcp_server()
    {
        wait_connection_tcp();
        data_reset_tcp();
    }

    void get_data_tcp_server() 
    {
        unsigned char buffer[size_buffer] = {0};
        int n = 0;
      
        if ( (n = read(newsockfd, buffer, size_buffer-1) ) < 0 )
        {
            error( const_cast<char *>( "ERROR reading from socket") );
        }
        else
        {
            buffer[n] = '\0'; // c-string
            memcpy(data_tcp, buffer, size_buffer);
            printf( "got <%s>\n", buffer);
        }
    }

    void send_data_tcp_server(unsigned char p_data_tcp[]) 
    {
        int n = 0;
        unsigned char buffer[size_buffer] = {0};

        memcpy(buffer, p_data_tcp, size_buffer);
        if ( (n = write( newsockfd, buffer, strlen((char*)buffer) ) ) < 0 )
        {
            error( const_cast<char *>( "ERROR writing to socket") );
        }
        else
        {
            buffer[n] = '\0'; // c-string
            printf( "sending back <%s>\n", buffer);
        }
    }

    void terminate_tcp_server()
    {
        close( newsockfd );
    }

}
