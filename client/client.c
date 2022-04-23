#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#define HTML_TYPE 0
#define GIF_TYPE 1
#define JPEG_TYPE 2
#define MP3_TYPE 3
#define PDF_TYPE 4
#define UNKNOWN_TYPE -1

#define MAX_BUFF 2048

// the Content-Type function (choosing the content type and parsing the object name)
char* parse_html(char *buff, int* obj_type);

int main(int argc, char *argv[])
{
  // valid usage check
  if (argc < 2){
    fprintf(stderr, "usage: http_server [port_num]\n");
    return 0;
  }

  // HTTP header info.
  char html_header[] = "HTTP/1.1 200 Ok\r\nContent-Type: text/html\r\n\r\n";
  char gif_header[] = "HTTP/1.1 200 Ok\r\nContent-Type: image/gif\r\n\r\n";
  char jpg_header[] = "HTTP/1.1 200 Ok\r\nContent-Type: image/jpeg\r\n\r\n";
  char mp3_header[] = "HTTP/1.1 200 Ok\r\nContent-Type: audio/mp3\r\n\r\n";
  char pdf_header[] = "HTTP/1.1 200 Ok\r\nContent-Type: application/pdf\r\n\r\n";
  char _404_header[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n";

  // create a socket
  int server_socket;
  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  // define the address
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(atoi(argv[1]));
  server_address.sin_addr.s_addr = INADDR_ANY;

  // receive ready
  bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));
  listen(server_socket, 5);

  // # read bytes
  int n;

  char cbuff[MAX_BUFF]; // client msg buff
  char fbuff[MAX_BUFF]; // file buffer

  // open a file to serve
  int fd;

  int object_type;
  int client_socket;

  while(1) {  // process keep alive until it receives SIGKILL
    bzero(cbuff, MAX_BUFF);
    bzero(fbuff, MAX_BUFF);

    // get the client msg.
    client_socket = accept(server_socket, NULL, NULL);
    read(client_socket, cbuff, MAX_BUFF);
    printf("%s\n", cbuff);

    // parse and open the object file
    if ((fd = open(parse_html(cbuff, &object_type), O_RDONLY)) != -1) {
      if (object_type == HTML_TYPE)
        write(client_socket, html_header, sizeof(html_header)-1);
      else if (object_type == GIF_TYPE)
        write(client_socket, gif_header, sizeof(gif_header)-1);
      else if (object_type == JPEG_TYPE)
        write(client_socket, jpg_header, sizeof(jpg_header)-1);
      else if (object_type == MP3_TYPE)
        write(client_socket, mp3_header, sizeof(mp3_header)-1);
      else if (object_type == PDF_TYPE)
        write(client_socket, pdf_header, sizeof(pdf_header)-1);
    }
    else {  // could not open a file
      fd = open("404.html", O_RDONLY);
      write(client_socket, _404_header, sizeof(_404_header)-1);
    }

    // response to client
    while((n=read(fd, fbuff, MAX_BUFF))>0)
      write(client_socket, fbuff, n);

    close(client_socket);
  }

  return 0;
}

char* parse_html(char *buff, int* obj_type)
{
  // parsed object name
  char *html_object;

  // type of the object
  char type[5];

  if((html_object = (char*)malloc(sizeof(char)*25)) == NULL)
  {
    printf("error on creating html obj. name...\n");
    exit(1);
  }

  // parse the object name
  char* runner = buff+5;  // object name comes from buff+5 location
  int obj_len = 0;  // length of the object file name
  while(*runner != ' ') {
    strncat(html_object, runner, 1);
    obj_len++;
    runner++;
  }

  if (*runner == ' ' && obj_len == 0) { // quit right away with default html
    *obj_type = HTML_TYPE;
    strcpy(html_object, "index.html");
    return html_object;
  }

  // choose the type
  int i;
  runner = html_object;
  while(*runner != '.') runner++;
  runner++; // we don't need '.'

  for(i=0; i<4; i++) {
    type[i] = *runner;
    runner++;
  }
  type[4] = '\0'; // null at the end

  if(strcmp(type, "html") == 0) *obj_type = HTML_TYPE;
  else if(strcmp(type, "jpg") == 0) *obj_type = JPEG_TYPE;
  else if(strcmp(type, "gif") == 0) *obj_type = GIF_TYPE;
  else if(strcmp(type, "mp3") == 0) *obj_type = MP3_TYPE;
  else if(strcmp(type, "pdf") == 0) *obj_type = PDF_TYPE;
  else *obj_type = UNKNOWN_TYPE;

  return html_object;
}