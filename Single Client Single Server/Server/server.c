#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SIZE 1024

void error_terminate(const char *message)
{

	perror ( message );
	exit(1);

}


void download(int newsocketfd, struct sockaddr_in client_address)
{
	// Defining variables
	int check;
  	char buffer[SIZE];
  	bzero(buffer, SIZE);
  	char *filename = "clientfile_upload_copy.txt" ;


  	//File Oparation
  	FILE* fp = fopen(filename, "w");

  	//Receiving the data and writing it into the file.
  	while (1)
  	{
    		check = recv(newsocketfd, buffer, SIZE, 0);

    		if (strcmp(buffer, "END") == 0)
    		{
      			break;
    		}

    		fprintf(stdout,"\nRECEVING DATA> %s", buffer);
    		fflush(stdout);
    		fprintf(fp, "%s", buffer);
    		bzero(buffer, SIZE);
  	}
  	
  	fprintf(stdout,"\nFile succesfully uploaded from Client!!!!!!!!!!!!!!\nSee the uploaded file 'clientfile_upload_copy.txt' in server folder.\n");
    	fflush(stdout);


  	fclose(fp);


}


void upload(int newsocketfd, struct sockaddr_in client_address)
{
	// Defining variables
	int check;
  	char buffer[SIZE];
  	bzero(buffer, SIZE);
  	char *filename = "serverfile.txt" ;

  	//File Oparation
  	FILE* fp = fopen(filename, "r");

  	// Reading the text file and Sending it
	if (fp == NULL)
	{
		error_terminate("ERROR: Can't read the file");
	}

    //fgets(buffer, SIZE, fp);
    //rewind(fp);

	while (fgets(buffer, SIZE, fp) != NULL)
  	{
    		fprintf(stdout,"\nSENDING DATA> %s", buffer);
    		fflush(stdout);

    		check = send(newsocketfd, buffer, SIZE, 0);
    		if (check < 1)
    		{
      			error_terminate("ERROR : Can't send data to the server.");

    		}
    	bzero(buffer, SIZE);
  	}

  	// Sending the 'END'
  	strcpy(buffer, "END");
  	send(newsocketfd, buffer, SIZE, 0);
  	
  	fprintf(stdout,"\nDownload completed!!!!!!!!!!!!!!");
    	fflush(stdout);

  	fclose(fp);


}


void delete(int newsocketfd, struct sockaddr_in client_address)
{
	// Defining variables
  	char buffer[SIZE];
  	bzero(buffer, SIZE);

  	//Receiving Filename to Delete
  	recv(newsocketfd, buffer, SIZE, 0);

  	fprintf(stdout, "DELETING THE FILE NAMED 'serverfile_delete.txt' FROM SERVER\n");
  	fflush(stdout);

  	if(remove(buffer) == 0)
  	{
  		fprintf(stdout, "Successfully Deleted 'serverfile_delete.txt' file\n");
  		fflush(stdout);
  	}
  	else
  	{
  		fprintf(stdout, "Couldn't Delete  'serverfile_delete.txt' filen\n");
  		fflush(stdout);
  	}

}


void rename_file(int newsocketfd, struct sockaddr_in server_address)
{
	// Defining variables
  	char buffer[SIZE];
  	bzero(buffer, SIZE);
  	//Renaming The File

  	recv(newsocketfd, buffer, SIZE, 0);

  	fprintf(stdout, "Renaming THE FILE NAMED 'serverfile_rename.txt' FROM SERVER\n");
  	fflush(stdout);

  	if(rename("serverfile_rename.txt",buffer) == 0)
  	{
  		fprintf(stdout, "Successfully renamed the 'serverfile_delete.txt' file to %s\n",buffer);
  		fflush(stdout);
  	}
  	else
  	{
  		fprintf(stdout, "Couldn't rename 'serverfile_delete.txt' file\n");
  		fflush(stdout);
  	}



}

int main(int argc, char *argv[])
{
	// Defining the IP and Port
	char *ip = "127.0.0.1";
	const int port_no = 8080;

	// Defining variables
	int socketfd, check, newsocketfd;
	struct sockaddr_in server_address, client_address;
	char buff[SIZE];
	bzero(buff, SIZE);
	socklen_t client_address_size;


	// Creating a TCP socket
	socketfd = socket(AF_INET, SOCK_STREAM, 0);

	if (socketfd<0)
	{
		error_terminate(" Error : can't open Socket.");
	}

	bzero((char *) &server_address, sizeof(server_address));

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(ip);
	server_address.sin_port = port_no;

	//Binding the socket

	if(bind(socketfd, (struct sockaddr *) &server_address , sizeof(server_address)) < 0 )
	{
		error_terminate("Error : Binding has failed{");
	}
	else
	{
		fprintf(stdout, "Server has started!!!!!!!!!!!!!!!!!!!!\n");
		fflush(stdout);
	}

	//Accepting client request

	listen(socketfd, 5);

	client_address_size = sizeof(client_address);

	newsocketfd = accept(socketfd, (struct sockaddr*) &client_address, &client_address_size);

	if(newsocketfd < 0)
	{
		error_terminate("Error : No accept");
	}
	else
	{
		fprintf(stdout,"Connection Stablishedd!!!!!!!!!!!!!!!!!!!!\n");
		fflush(stdout);
	}

	while(1)
	{
		check = recv(newsocketfd, buff, SIZE, 0);


		if( strcmp(buff,"download")==0)
		{
			fprintf(stdout,"\n<<<<<Client want to download file>>>>>\n\n");
			fflush(stdout);
			upload(newsocketfd,client_address);
			//printf(" Client want to download file");
		}
		else if( strcmp(buff,"upload")==0)
		{
			fprintf(stdout,"\n<<<<<Client want to upload file>>>>>\n\n");
			fflush(stdout);
			download(newsocketfd,client_address);
		}
		else if( strcmp(buff,"delete")==0)
		{
			fprintf(stdout,"\n<<<<<Client want to delete file>>>>>\n\n");
			fflush(stdout);
			delete(newsocketfd,client_address);
		}
		else if( strcmp(buff,"rename")==0)
		{
			fprintf(stdout,"\n<<<<<Client want to rename file>>>>>\n\n");
			fflush(stdout);
			rename_file(newsocketfd,client_address);
		}

		bzero(buff, SIZE);
		//fprintf(stdout,"\n");
    		//fflush(stdout);
    		
    		
	}
		
	close(newsocketfd);
	close(socketfd);

	return 0;



}

