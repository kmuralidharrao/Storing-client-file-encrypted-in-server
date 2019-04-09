
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno = 9999;
     socklen_t clilen;
     char buffer[512],fname[10];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 1) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
     if (newsockfd < 0)
          error("ERROR on accept");

    read(newsockfd,fname,sizeof(fname));

    FILE *fp;
    int ch = 0;
    fp = fopen(fname,"w");
    int words;
		read(newsockfd, &words, sizeof(int));

    while(ch != words)
    {
       read(newsockfd , buffer , 512);
	   	 fprintf(fp , " %s" , buffer);
		 ch++;
	  }
     printf("The file was received successfully\n");
	   printf("The new file uploaded is %s",fname);
     close(newsockfd);
     close(sockfd);
     return 0;
}
