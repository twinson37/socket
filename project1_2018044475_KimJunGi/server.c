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
#include <fcntl.h>


#define _CRT_SECURE_NO_WARNINGS
#define PORT 3490 
#define BACKLOG 10

int main() {
    int sock,new_fd;
    char sendBuff[1025],fileBuffer[2048],*message, *result, *result1, *filename, *filetype;
    const char * html_packet = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    const char * jpeg_packet = "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\n\r\n";
    const char * gif_packet = "HTTP/1.1 200 Ok\r\nContent-Type: image/gif\r\n\r\n";
    const char * mp3_packet = "HTTP/1.1 200 Ok\r\nContent-Type: audio/mp3\r\n\r\n";
    const char * pdf_packet = "HTTP/1.1 200 Ok\r\nContent-Type: application/pdf\r\n\r\n";
    // FILE *fp = fopen("index.html", "r");
    int fp;
    int n;
    /* 
    listen on sock_fd, new connection on new_fd
    */
    struct sockaddr_in server_addr; /* my address */ 
    struct sockaddr_in client_addr; /* connector addr */ 
    int sin_size;
    if ((sock = socket(PF_INET, SOCK_STREAM, 0))==-1){
        perror("socket");
        exit(1);
    }
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(PORT); 
    /* short, network byte order */ 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    /* INADDR_ANY allows clients to connect to any one of the host’s IP address */
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1); 
    }

    if (listen(sock, BACKLOG) == -1) { perror("listen");
    exit(1);
    }
    while(1) { /* main accept() loop */
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sock, (struct sockaddr*)&client_addr,&sin_size)) == -1) {
            perror("accept");
            continue; 
        }
        // printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));
        read(new_fd, sendBuff, sizeof(sendBuff));
		printf("%s",sendBuff);
        result = "";
        result = sendBuff;
        filename = strtok(result,"/");
        filename = strtok(NULL," ");
        strcpy(result1,filename);
        filetype = strtok(result1,".");
        filetype = strtok(NULL," ");



        fp = open(filename,O_RDONLY);
        if(strcmp(filetype, "html") == 0){

            write(new_fd, html_packet,strlen(html_packet));
        }else if(strcmp(filetype, "jpeg") == 0){
            write(new_fd, jpeg_packet,strlen(jpeg_packet));
        }else if(strcmp(filetype, "gif") == 0){
            write(new_fd, gif_packet,strlen(gif_packet));
        }else if(strcmp(filetype, "mp3") == 0){
            write(new_fd, mp3_packet,strlen(mp3_packet));
        }else if(strcmp(filetype, "pdf") == 0){
            write(new_fd, pdf_packet,strlen(pdf_packet));
        }else{
            write(new_fd, html_packet,strlen(html_packet));
        }
        // printf("!!!!!! %s!!!!!\n",filename);
        // printf("!!!!!! %s!!!!!\n",filetype);
        while((n=read(fp, fileBuffer, 2048))>0)
            write(new_fd, fileBuffer, n);
        close(new_fd);
    }
    // fclose(fp);
    return 0;
}