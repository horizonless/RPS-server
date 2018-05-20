#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <dns_sd.h>
#include <time.h>
#include <errno.h>
#include <math.h>
#include <ctype.h>
#include <fcntl.h> // for open
// #include <unistd.h> // for close

// extern void HandleEvents;
static volatile int stopNow = 0;
#define STDIN 0

void gamelogic(char namea[], char inputa[],char nameb[],char inputb[],int asd,int bsd){

        if(strcmp(inputa,"PAPER") == 10 &&strcmp(inputb,"ROCK") == 10) {
                char r[255];
                strcpy(r, "PAPER covers ROCK! ");

                strcat(r,namea);
                strcat(r, " defeats ");
                strcat(r, nameb);
                strcat(r, "\n");
                write(asd,r,strlen(r));
                write(bsd,r,strlen(r));
                // return 0;
        }
        else if(strcmp(inputb,"PAPER") == 10 &&strcmp(inputa,"ROCK") == 10 ) {
                        char r[255];
                        strcpy(r, "PAPER covers ROCK! ");

                        strcat(r,nameb);
                        strcat(r, " defeats ");
                        strcat(r, namea);
                        strcat(r, "\n");
                        write(asd,r,strlen(r));
                        write(bsd,r,strlen(r));
                        // return 0;
        }
        else if(strcmp(inputa,"ROCK") == 10 &&strcmp(inputb,"SCISSORS") == 10 ) {
                        char r[255];
                        strcpy(r, "ROCK smashes SCISSORS! ");

                        strcat(r,namea);
                        strcat(r, " defeats ");
                        strcat(r, nameb);
                        strcat(r, "\n");
                        write(asd,r,strlen(r));
                        write(bsd,r,strlen(r));
                        // return 0;
              }
        else if(strcmp(inputb,"ROCK")== 10 &&strcmp(inputa,"SCISSORS")==10) {
                                char r[255];
                                strcpy(r, "ROCK smashes SCISSORS! ");

                                strcat(r,nameb);
                                strcat(r, " defeats ");
                                strcat(r, namea);
                                strcat(r, "\n");
                                write(asd,r,strlen(r));
                                write(bsd,r,strlen(r));
                                // return 0;
        }
        else if(strcmp(inputa,"SCISSORS") == 10&&strcmp(inputb,"PAPER") == 10) {
                        char r[255];
                        strcpy(r, "SCISSORS cut PAPER! ");

                        strcat(r,namea);
                        strcat(r, " defeats ");
                        strcat(r, nameb);
                        strcat(r, "\n");
                        write(asd,r,strlen(r));
                        write(bsd,r,strlen(r));
                        // return 0;
                }
        else if(strcmp(inputb,"SCISSORS") == 10 &&strcmp(inputa,"PAPER") == 10) {
                                char r[255];
                                strcpy(r, "SCISSORS cut PAPER! ");

                                strcat(r,nameb);
                                strcat(r, " defeats ");
                                strcat(r, namea);
                                strcat(r, "\n");
                                write(asd,r,strlen(r));
                                write(bsd,r,strlen(r));
                                // return 0;
                        }
        else {
              write(asd,"They are tied\n",14);
              write(bsd,"They are tied\n",14);
        }
}

// extern void HandleEvents();
int validinput( char buffer[]){
        int i=0;
        while (buffer[i]) {
                buffer[i]= (toupper(buffer[i]));
                i++;
        }

        if(strcmp(buffer,"PAPER")== 10 || strcmp(buffer,"ROCK") == 10 || strcmp(buffer,"SCISSORS")== 10) {
                // printf("valid!!!\n");
                return 1;
        }
        return 0;

}
void
HandleEvents(DNSServiceRef serviceRef,int sd)
{
        int dns_sd_fd = DNSServiceRefSockFD(serviceRef);
        // printf("dns_sd_fd %d \n",dns_sd_fd );
        // int nfds = dns_sd_fd + 1;
        fd_set master;    // master file descriptor list
        fd_set readfds;  // temp file descriptor list for select()
        int fdmax;        // maximum file descriptor number
        int i;


        FD_ZERO(&master); // clear the master and temp sets
        FD_SET(sd,&master);
        FD_SET(dns_sd_fd,&master);
        //keep track of the biggest file descriptors
        fdmax = sd;
        int result, client_fd[2];
        struct sockaddr_in client;
        // int fromlen = sizeof( client );
        int inadd, listensock,input_size;
        // int n;
        char inputs[255];
        //two clients
        char clientA[255];
        char clientB[255];
        char clientA_input[255];
        char clientB_input[255];
        int receiveA = 0;
        int receiveB = 0;
        int correct_nameA = 1;
        int correct_nameB = 1;
        int sockfd;
        for (i = 0; i < 2; i++)
                client_fd[i] = -1;
        while (1)
        {

                readfds = master;
                // printf("stop1\n");
                if((receiveA == 1) && (receiveB == 1)) {
                        //have both valid input and compare it
                        gamelogic(clientA,clientA_input,clientB,clientB_input,client_fd[0],client_fd[1]);
                        close(client_fd[0]);
                        close(client_fd[1]);
              					FD_CLR(client_fd[0], &master);
                        FD_CLR(client_fd[1], &master);
                        client_fd[0] = -1;
                        client_fd[1] = -1;
                        fdmax = fdmax -2;
                        receiveA = 0;
                        receiveB = 0;
                        correct_nameA = 1;
                        correct_nameB = 1;

                }
                result = select(fdmax+1, &readfds, (fd_set*)NULL, (fd_set*)NULL, NULL);
                // printf("stop2\n");
                if (result > 0)
                {

                        DNSServiceErrorType err = kDNSServiceErr_NoError;
                        if (FD_ISSET(dns_sd_fd, &readfds))
                                err = DNSServiceProcessResult(serviceRef);
                        if (err) {
                                fprintf(stderr,"DNSServiceProcessResult returned %d\n", err);
                                break;
                        }
                        // printf("goes here\n");
                        if (FD_ISSET(sd, &readfds)) {
                                //setting up the connections
                                printf("setting up the connections\n" );
                                inadd = sizeof(struct sockaddr_in);
                                listensock = accept(sd, (struct sockaddr *) &client, (socklen_t*)&inadd);
                                // printf("listensock:%d\n",listensock );
                                if (listensock < 0) {
                                        perror("accept failed\n");
                                        break;
                                }
                                for (i = 0; i < 2; i++)
                                        if (client_fd[i] < 0) {
                                                client_fd[i] = listensock; /* save descriptor */
                                                break;
                                        }
                                write(listensock,"What is your name?\n",20);
                                // printf("What is your name?\n");
                                FD_SET(listensock, &master);
                                // FD_SET(listensock, &readfds);
                        }
                        if (fdmax < listensock) {
                                fdmax = listensock;
                                // printf("update fdmax\n" );
                        }
                        // printf("reading inputs\n");
                        for(i = 0; i < 2; i++) {
                                if ( (sockfd = client_fd[i]) < 0)
                                        continue;
                                if (FD_ISSET(sockfd, &readfds)) {

                                        if ((input_size = recv(sockfd, inputs, 20, 0)) > 0) {
                                                inputs[input_size] = '\0';
                                                // printf("buffer:%s\n",inputs );
                                                // printf("sockfd:%d\n",sockfd );

                                                if(correct_nameA == 1) {
                                                        // printf("buffer:%s\n",inputs );
                                                        if(sockfd == client_fd[0]) {
                                                                // strcpy(clientA,inputs);
                                                                strcpy(clientA,inputs);
                                                                printf("ClientA:%s\n",clientA );
                                                                correct_nameA = 0;
                                                        }
                                                        // correct_nameA = 0;
                                                }
                                                else if(correct_nameB == 1) {
                                                        // printf("=====%s\n",inputs );

                                                        if(sockfd == client_fd[1]) {
                                                                // strcpy(clientB,inputs);
                                                                strcpy(clientB,inputs);
                                                                printf("ClientB:%s\n",clientB );
                                                                correct_nameB = 0;
                                                        }
                                                        // correct_nameB = 0;
                                                }
                                                if(validinput(inputs) == 1) {
                                                        // printf("test\n" );

                                                        if((receiveA == 1) && (receiveB == 1)) {
                                                                //have both valid input and compare it
                                                                printf("inside\n");
                                                                // gamelogic(clientA,clientA_input,clientB,clientB_input,client_fd[0],client_fd[1]);
                                                        }else{
                                                                if(sockfd == client_fd[0]) {//clientA put valid input
                                                                        strcpy(clientA_input,inputs);
                                                                        receiveA = 1;
                                                                }
                                                                if(sockfd == client_fd[1]) {//clientA put valid input
                                                                        strcpy(clientB_input,inputs);
                                                                        receiveB = 1;
                                                                }

                                                        }

                                                        if(sockfd == client_fd[0]) {//clientA put valid input
                                                                strcpy(clientA_input,inputs);
                                                                receiveA = 1;
                                                        }
                                                        if(sockfd == client_fd[1]) {//clientA put valid input
                                                                strcpy(clientB_input,inputs);
                                                                receiveB = 1;
                                                        }

                                                }else{
                                                        write(sockfd, "Rock, paper, or scissors?\n", 26);
                                                }
                                                //valid input check



                                        }
                                }
                        }

                }
                else//error
                {
                        printf("select() returned %d errno %d %s\n",
                               result, errno, strerror(errno));
                        if (errno != EINTR) stopNow = 1;
                }
                // printf("end of while\n");
        }
}

// int validinput( char buffer[]){
//         int i=0;
//         while (buffer[i]) {
//                 putchar(toupper(buffer[i]));
//                 i++;
//         }
//
//         if(strcmp(buffer,"PAPER")!=0&&strcmp(buffer,"ROCK")!=0&&strcmp(buffer,"SCISSORS")!=0) {
//
//                 return 0;
//         }
//         return 1;
//         // if(strcmp(inputb,"PAPER")!=0&&strcmp(inputb,"ROCK")!=0&&strcmp(inputb,"SCISSORS")!=0) {
//         //         return 2;
//         // }
// }

static void MyRegisterCallBack(DNSServiceRef service,
                               DNSServiceFlags flags,
                               DNSServiceErrorType errorCode,
                               const char * name,
                               const char * type,
                               const char * domain,
                               void * context)
{
    #pragma unused(flags)
    #pragma unused(context)

        if (errorCode != kDNSServiceErr_NoError)
                fprintf(stderr, "MyRegisterCallBack returned %d\n", errorCode);
        else
                printf("%-15s %s.%s%s\n","REGISTER", name, type, domain);
}

static DNSServiceErrorType MyDNSServiceRegister(uint16_t port_num, int sd)
{
        DNSServiceErrorType error;
        DNSServiceRef serviceRef;
        printf("%d\n",port_num );
        error = DNSServiceRegister(&serviceRef,
                                   0,               // no flags
                                   0,               // all network interfaces
                                   "zhangs18", // name
                                   "_rps._tcp",    // service type
                                   "",              // register in default domain(s)
                                   NULL,            // use default host name
                                   port_num,     // port number
                                   0,               // length of TXT record
                                   NULL,            // no TXT record
                                   MyRegisterCallBack, // call back function
                                   NULL);           // no context

        if (error == kDNSServiceErr_NoError)
        {
                HandleEvents(serviceRef,sd);
                DNSServiceRefDeallocate(serviceRef);
        }

        return error;
}

int main(int argc, char const *argv[]) {
        /* Create the listener socket as TCP socket */
        int sd = socket( PF_INET, SOCK_STREAM, 0 );
        uint16_t port_num = 0;
        /* here, the sd is a socket descriptor (part of the fd table) */
        if ( sd == -1 )
        {
                perror( "socket() failed" );
                return EXIT_FAILURE;
        }
        /* socket structures */
        struct sockaddr_in server;
        server.sin_family = PF_INET;
        server.sin_addr.s_addr = INADDR_ANY; /* allow any IP address to connect */
        /* htons() is host-to-network short for data marshalling */
        server.sin_port = htons( 0 );
        socklen_t len = sizeof( server );
        if ( bind( sd, (struct sockaddr *)&server, len ) == -1 )
        {
                perror( "bind() failed" );
                return EXIT_FAILURE;
        }
        /* identify this port as a listener port */
        if ( listen( sd, 5 ) == -1 )
        {
                perror( "listen() failed" );
                return EXIT_FAILURE;
        }

        if (getsockname(sd, (struct sockaddr *)&server, &len) == -1)
                perror("getsockname");
        else
        {
                printf("Started server\n");
                fflush(stdout);
                port_num = ntohs(server.sin_port);
                printf( "Listening for connections on port: %d\n", port_num );
        }


        DNSServiceErrorType error = MyDNSServiceRegister(port_num, sd);
        fprintf(stderr, "DNSServiceDiscovery returned %d\n", error);
        return 0;
}
