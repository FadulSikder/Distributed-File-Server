#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>
#define SIZE 1024




void error_terminate(const char *message)
{

	perror ( message );
	exit(1);

}


void download(int socketfd, struct sockaddr_in server_address)
{
	// Defining variables
	int check, i=0;
  	char buffer[SIZE];
  	bzero(buffer, SIZE);

  	//Creating and openiing file
  	char *filename = "servfile_download_copy.txt" ;
  	FILE* fp = fopen(filename, "w");
  	
  	
  	//Receiving the data and writing it into the file.
  	while (1)
  	{
    		check = recv(socketfd, buffer, SIZE, 0);

    		if (strcmp(buffer, "END") == 0)
    		{
      			break;
    		}

    		fprintf(stdout,"\nRECEVING DATA> %s", buffer);
    		fflush(stdout);
    		fprintf(fp, "%s", buffer);
    		bzero(buffer, SIZE);
  	}

  	fprintf(stdout,"\nFile succesfully downloaded from Server!!!!!!!!!!!!!!\nSee the downloaded file 'servfile_download_copy.txt' in client folder.\n");
    	fflush(stdout);

  	fclose(fp);
	
}


void upload(int socketfd, struct sockaddr_in server_address)
{
	// Defining variables
	int check, i=0;
  	char buffer[SIZE];
  	bzero(buffer, SIZE);
  	char *filename = "clientfile.txt" ;
  	
  	//File Oparation
  	FILE* fp = fopen(filename, "r");
  	
  	// Reading the text file and Sending it
	if (fp == NULL)
	{
		error_terminate(" ERROR: Can't read the file");
	}
	
	while (fgets(buffer, SIZE, fp) != NULL)
  	{
    		fprintf(stdout,"\nSENDING DATA> %s", buffer);
    		fflush(stdout);

    		check = send(socketfd, buffer, SIZE, 0);
    		if (check < 1)
    		{
      			error_terminate("ERROR : Can't send data to the server.");
      			
    		}
    	bzero(buffer, SIZE);
  	}

  	// Sending the 'END'
  	strcpy(buffer, "END");
  	send(socketfd, buffer, SIZE, 0);

  	fprintf(stdout,"\nUpload completed!!!!!!!!!!!!!!");
    	fflush(stdout);


  	fclose(fp);

  	
}


void delete(int socketfd, struct sockaddr_in server_address)
{
	// Defining variables
	int check, i=0;
  	char buffer[SIZE];
  	bzero(buffer, SIZE);
  	char *filename = "serverfile_delete.txt" ;

  	//Sending Filename to Delete
  	strcpy(buffer, filename);
  	fprintf(stdout,"SENDING INSTRACTION TO DELETE FILE > %s\n", buffer);
    	fflush(stdout);
  	send(socketfd, buffer, SIZE, 0);
  	
}


void rename_file(int socketfd, struct sockaddr_in server_address)
{
	// Defining variables
	int check, i=0;
  	char buffer[SIZE];
  	bzero(buffer, SIZE);
  	
  	//Sending Filename to Delete
  	fprintf(stdout,"Please provide the name: ");
  	fflush(stdout);
  	scanf("%s", buffer);
  	strcat(buffer, ".txt");
  	fprintf(stdout,"SENDING INSTRACTION TO RENAME FILE TO > %s\n", buffer);
    	fflush(stdout);
  	send(socketfd, buffer, SIZE, 0);
  	
}


int main(int argc, char *argv[])
{
	
	// Defining the IP and Port
	char *ip = "127.0.0.1";
	const int port_no = 8080;
	
	// Defining variables
	int socketfd, n;
	struct sockaddr_in server_address;
	char buff[SIZE];
	bzero(buff, SIZE);
	
	// Creating a Client TCP socket
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd<0)
	{
		error_terminate(" Error : can't open Socket.");
	}
	bzero((char *) &server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
  	server_address.sin_port = port_no;
  	server_address.sin_addr.s_addr = inet_addr(ip);

  	fprintf(stdout,"Client has started!!!!!!!!!!!!!!\n");
    	fflush(stdout);

  	if(connect(socketfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
	{
		error_terminate("Error : Can't Connect with Server");
	}
	else
	{
	    	fprintf(stdout,"Connection Stablished!!!!!!!!!!!!!!\n");
	    	fflush(stdout);
	}
	
	//Creating UI and choosing which function to perform
	while(1)
	{
		fprintf(stdout,"\nWhat operation do you want to do?\n1. Uplado a file to Server\n2. Download a file from Server\n3. Delete a file from Server\n4. Rename a file from server\n5. Terminate the program.\nPlease type the number of the option you want to perform(1/2/3/4/5): ");
		fflush(stdout);
		scanf("%d", &n);
		
		fprintf(stdout,"\n");
    		fflush(stdout);
		
		switch(n)
		{
			case 1:
				strcpy(buff, "upload");
	  			send(socketfd, buff, SIZE, 0);
				upload(socketfd,server_address);
				break;
				
			case 2: 
				strcpy(buff, "download");
	  			send(socketfd, buff, SIZE, 0);
				download(socketfd,server_address);
				break;
				
			case 3:
				strcpy(buff, "delete");
	  			send(socketfd, buff, SIZE, 0);
				delete(socketfd,server_address);
				break;
				
			case 4:
				strcpy(buff, "rename");
	  			send(socketfd, buff, SIZE, 0);
				rename_file(socketfd,server_address);
				break;

			/*case 5:
				strcpy(buff, "terminate");
	  			send(socketfd, buff, SIZE, 0);
				rename_file(socketfd,server_address);
				break;*/
		
		}

		if(n == 5)
		{
			fprintf(stdout,"Terminating the client program!!!!!!!!!!!!!!\n");
    			fflush(stdout);
    		break;
		}
		fprintf(stdout,"\n");
    		fflush(stdout);
		bzero(buff, SIZE);
    
    }
    
    close(socketfd);
    
    return 0;
}
