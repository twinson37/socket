#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h>
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/wait.h>
#include <unistd.h>

#define PORT 3490 
#define BACKLOG 10
// #define Server_IP_address 127.0.0.1

int main(){
    int sockfd, new_fd;
    struct sockaddr_in their_addr; /* connector addr */ 

    if ((sockfd = socket (PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit (1); 
        }
    their_addr.sin_family = AF_INET; 
    their_addr.sin_port = htons(PORT); 
    // their_addr.sin_addr = htonl(Server_IP_address);
    their_addr.sin_addr.s_addr = INADDR_ANY;
    if (connect (sockfd, (struct sockaddr*)&their_addr, sizeof (struct sockaddr)) == -1) {
        perror ("connect");
        exit (1); 
    }

    return 0;
}
// int main(){
//     //create a socket
//     int network_socket;
//     network_socket = socket(PF_INET, SOCK_STREAM,0);

//     //speciify an address for the socket
//     struct sockaddr_in server_address;
//     server_address.sin_family = PF_INET;
//     server_address.sin_port = htons(PORT);
//     server_address.sin_addr.s_addr = INADDR_ANY;

//     int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
//     //check for error with the connection
//     if(connection_status == -1){
//        printf("There was an erro making a connection to the remote socket \n\n");
//     }

//     //recieve data from the server
//     char server_response[256];
//     recv(network_socket, &server_address, sizeof(server_response),0);

//     //print out the server's response
//     printf("The server sent the data: %s\n", server_response);

//     //and the close the socket
//     close(network_socket);

//     return 0;
// }