#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>
#include <fcntl.h>

char dfn[10];
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void encrypt(char fn[10])
{
  char message[10],ch;
  int key,i;
  int sfd=open(fn,O_RDONLY|O_EXCL,0);
  printf("Destination file name : \n");
  scanf("%s",dfn);
  printf("Encryption Key : \n");
  scanf("%d",&key);
	int dfd=open(dfn,O_WRONLY|O_CREAT,S_IRWXU|S_IRGRP|S_IWGRP|S_IROTH);
  while(read(sfd,message,sizeof(message))>0)
	{
	for(i = 0; message[i] != '\0'; ++i)
  {
		ch = message[i];
    if(ch >= 'a' && ch <= 'z'){
			ch = ch + key;

			if(ch > 'z'){
				ch = ch - 'z' + 'a' - 1;
			}

			message[i] = ch;
		}
		else if(ch >= 'A' && ch <= 'Z'){
			ch = ch + key;

			if(ch > 'Z'){
				ch = ch - 'Z' + 'A' - 1;
			}

			message[i] = ch;
		}
	 }
	 write(dfd,message,sizeof(message));
	}
  printf("File has been encrypted.... \n sending to server ....\n");
	close(sfd);
	close(dfd);
	}


int main(int argc, char *argv[])
{
    int sockfd, portno = 9999, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[512];
    if (argc < 3)
    {
       error("Not valid\n");
       exit(0);
    }
    encrypt(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    bzero(buffer,512);
    write(sockfd,dfn,sizeof(dfn));
    FILE *f;
    int words = 0;
    char c;
    f=fopen(dfn,"r");
    while((c=getc(f))!=EOF)			//Counting No of words in the file
	  {
		  fscanf(f , "%s" , buffer);
		  if(isspace(c)||c=='\t')
		    words++;
	  }


  	write(sockfd, &words, sizeof(int));
     	rewind(f);

    char ch ;
    while(ch != EOF)
    {
    fscanf(f , "%s" , buffer);
		write(sockfd,buffer,512);
		ch = fgetc(f);
    }
	  printf("The file was sent successfully \n");

    close(sockfd);
    return 0;
}
