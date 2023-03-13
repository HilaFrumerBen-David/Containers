//
// Created by omer_siton on 14/06/2022.
//


#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <cstdio>
#include <strings.h>
#include <netdb.h>
#include <unistd.h>


#define MAX_CLIENTS 5
#define MAXHOSTNAME 256
#define SIZEBUFF 256
#define ERR_READ "system error: read is failed"
#define ERR_ECCEPT "system error: accept is failed"
#define ERR_WRITE "system error: write is failed"
#define ERR_HOSTNAME "system error: get host by name is failed"
#define ERR_SOCKET "system error: create socket is failed"
#define ERR_BIND "system error: Binding an Address is failed"
#define ERR_LISTEN "system error: listen is failed"
#define ERR_SYSTEM "system error: system is failed"
#define ERR_CONNECT "system error: connect is failed"


void error_and_exit(const char* msg_error)
{
    fprintf(stderr, "%s\n", msg_error);
    exit(1);
}

int get_connection(int s){ // s is a  socket created with establish()
    int t; // t is a socket of connection of server

    if ((t = accept(s, nullptr, nullptr)) < 0){
        error_and_exit(ERR_ECCEPT);
    }
    return t;
}


int write_data(int s, char *buf, unsigned long long n){
    int bcount; //counts bytes read
    int br; // bytes read this pass
    bcount= 0; br= 0;

    while (bcount < n) {  // loop until full buffer
        br = write(s, buf, n-bcount);
        if (br > 0) {
            bcount += br;  // increment byte counter
            buf += br; // move buffer ptr for next read */
        }
        if (br < 0) {
            error_and_exit(ERR_WRITE);
        }
    }
    return bcount;
}


int read_data(int s, char *buf, int n) {
    int bcount; // counts bytes read
    int br; //bytes read this pass
    bcount= 0; br= 0;

    while (bcount < n) { // loop until full buffer
        br = read(s, buf, n-bcount);
        if (br > 0) {
            bcount += br;
            buf += br;
        }
        if (br == 0){
          break;
        }
        if (br < 0) {
            error_and_exit(ERR_READ);
        }
    }
    return bcount;
}


int main(int argc, char* argv[]) {

    // argument initlization
    unsigned short portnum = std::strtoul(argv[2], nullptr,10);
    char buffer[SIZEBUFF];
    char myname[MAXHOSTNAME+1]; //my hostname
    int s; //welcome socket
    struct sockaddr_in sa{}; // ip, port
    struct hostent *hp; // return from get host by name

    // hostnet initialization
    gethostname(myname, MAXHOSTNAME);
    if ((hp= gethostbyname (myname)) == nullptr) {
        error_and_exit( ERR_HOSTNAME);
    }

    // sockaddrr_in initlization
    memset(&sa, 0, sizeof(struct sockaddr_in)); //fill all zero
    sa.sin_family = hp->h_addrtype; // AF_INET
    memcpy(&sa.sin_addr, hp->h_addr, hp->h_length);  //this is our host address
    sa.sin_port= htons(portnum); // this is our port number

    // First step - create socket
    if ((s= socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error_and_exit(ERR_SOCKET);
    }

    // ******* the server ******* //
    if (argc == 3) {

        // Second step - Binding an Address
        if (bind(s , (struct sockaddr *)&sa , sizeof(struct sockaddr_in)) < 0) {
            close(s);
            error_and_exit(ERR_BIND);
        }

        // Third Step – Listening , only 5 client can connect
        if (listen(s, MAX_CLIENTS) < 0){
            close(s);
            error_and_exit(ERR_LISTEN);
        }

        //Fourth Step – Waiting for Calls
        while (true){
            int t = get_connection(s); // t is a socket of connection of server
            read_data(t, buffer,SIZEBUFF);
            if (system(buffer) == -1){
                error_and_exit(ERR_SYSTEM);
            }
            close(t);
            bzero(buffer, strlen(buffer));
        }
     }

    // ******* the client ******* //
    if (argc == 4) {

        // Second step - connect to the server
        if (connect(s, (struct sockaddr *)&sa , sizeof(sa)) < 0) {
            close(s);
            error_and_exit(ERR_CONNECT);
        }

        // Third Step - writing data
        if(write_data(s,argv[3], SIZEBUFF) == -1){
            error_and_exit(ERR_WRITE);
        }
    }
}
