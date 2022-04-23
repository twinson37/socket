#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <arpa/inet.h>
#include <unistd.h>


#define PORT 3490 
#define BACKLOG 10

// int main() {
//     char server_message[256] = "a";

//     //create the server socket
//     int server_socket;
//     server_socket = socket(PF_INET, SOCK_STREAM, 0);

//     //define the server address
//     struct sockaddr_in server_address;
//     server_address.sin_family = PF_INET;
//     server_address.sin_port = htons(PORT);
//     server_address.sin_addr.s_addr = INADDR_ANY;

//     //bind the socket to our specified IP and port
//     bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

//     listen(server_socket,5);

//     int client_socket;
//     client_socket = accept(server_socket,NULL,NULL);

//     //send the message
//     send(client_socket,server_message,sizeof(server_message),0);

//     //close the socket
//     close(server_socket);
    
//     return 0;
// }

int main() {
    int sockfd, new_fd;
    char sendBuff[1025],*message;
    /* 
    listen on sock_fd, new connection on new_fd
    */
    struct sockaddr_in my_addr; /* my address */ 
    struct sockaddr_in their_addr; /* connector addr */ 
    int sin_size;
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0))==-1){
        perror("socket");
        exit(1);
    }
    my_addr.sin_family = PF_INET;
    my_addr.sin_port = htons(PORT); 
    /* short, network byte order */ 
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    /* INADDR_ANY allows clients to connect to any one of the host’s IP address */
    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1); 
    }

    if (listen(sockfd, BACKLOG) == -1) { perror("listen");
    exit(1);
    }
    while(1) { /* main accept() loop */
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr*)&their_addr,&sin_size)) == -1) {
            perror("accept");
            continue; 
        }
        // printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));
        recv(new_fd, sendBuff, sizeof(sendBuff),0);
		printf("Response %s\n",sendBuff);

        message = "hello";
        send(new_fd, sendBuff, strlen(sendBuff), 0);
        // printf("Hello message sent\n");
    }
    return 0;
}