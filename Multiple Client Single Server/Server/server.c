#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>

#define SIZE 1024
int client_number = 0 ;



void error_terminate(const char *message)
{

	perror ( message );
	exit(1);

}


void download(int thread_socketfd, int client_number)
{
	// Defining variables
	int check;
  	char buffer[SIZE],str[20],filename[20] = "uploaded_file_client";
  	bzero(buffer, SIZE);
  	bzero(str, 20);

  	sprintf(str, "%d", client_number);

  	strcat(filename,str);
  	strcat(filename, ".txt");   


  	//File Oparation
  	FILE* fp = fopen(filename, "w");

  	//Receiving the data and writing it into the file.
  	while (1)
  	{
    		check = recv(thread_socketfd, buffer, SIZE, 0);

    		if (strcmp(buffer, "END") == 0)
    		{
      			break;
    		}

    		fprintf(stdout,"\nRECEVING DATA> %s", buffer);
    		fflush(stdout);
    		fprintf(fp, "%s", buffer);
    		bzero(buffer, SIZE);
  	}
  	
  	fprintf(stdout,"\nFile succesfully uploaded from Client!!!!!!!!!!!!!!\nSee the uploaded file '%s' in server folder.\n",filename);
    	fflush(stdout);


  	fclose(fp);


}


void upload(int thread_socketfd, int client_number)
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

    		check = send(thread_socketfd, buffer, SIZE, 0);
    		if (check < 1)
    		{
      			error_terminate("ERROR : Can't send data to the server.");

    		}
    	bzero(buffer, SIZE);
  	}

  	// Sending the 'END'
  	strcpy(buffer, "END");
  	send(thread_socketfd, buffer, SIZE, 0);
  	bzero(buffer, SIZE);
  	sprintf(buffer, "%d", client_number);
  	send(thread_socketfd, buffer, SIZE, 0);
  	
  	fprintf(stdout,"\nDownload completed!!!!!!!!!!!!!!");
    	fflush(stdout);

  	fclose(fp);


}


void delete(int thread_socketfd, int client_number)
{
	// Defining variables
  	char str[20],filename[50] = "delete_serverfile";
  	sprintf(str, "%d", client_number);

  	strcat(filename,str);
  	strcat(filename, ".txt");   


  	//Receiving Filename to Delete

  	fprintf(stdout, "DELETING THE FILE NAMED '%s' FROM SERVER\n",filename);
  	fflush(stdout);

  	if(remove(filename) == 0)
  	{
  		fprintf(stdout, "Successfully Deleted '%s' file\n",filename);
  		fflush(stdout);
  	}
  	else
  	{
  		fprintf(stdout, "Couldn't Delete  '%s' filen\n", filename);
  		fflush(stdout);
  	}

}




void rename_file(int thread_socketfd, int client_number)
{
	// Defining variables
  	char buffer[SIZE],str[20],filename[50] = "rename_serverfile";
  	bzero(buffer, SIZE);
  	bzero(str, 20);

  	sprintf(str, "%d", client_number);

  	strcat(filename,str);
  	strcat(filename, ".txt"); 
  	//Renaming The File

  	recv(thread_socketfd, buffer, SIZE, 0);

  	fprintf(stdout, "Renaming THE FILE NAMED '%s' FROM SERVER\n", filename);
  	fflush(stdout);

  	if(rename(filename,buffer) == 0)
  	{
  		fprintf(stdout, "Successfully renamed the '%s' file to '%s'\n",filename,buffer);
  		fflush(stdout);
  	}
  	else
  	{
  		fprintf(stdout, "Couldn't rename '%s' file\n", filename);
  		fflush(stdout);
  	}



}


void *client_connection_handler(void *newsocketfd)
{
	
	int i = client_number, check;
	char buff[SIZE];
	bzero(buff, SIZE);


	while(1)
	{
		int thread_socketfd = *((int *) newsocketfd) ;
		check = recv(thread_socketfd, buff, SIZE, 0);


		if( strcmp(buff,"download")==0)
		{
			fprintf(stdout,"\n<<<<<Client %i want to download file>>>>>\n\n", i);
			fflush(stdout);
			upload(thread_socketfd,i);
			//printf(" Client want to download file");
		}
		else if( strcmp(buff,"upload")==0)
		{
			fprintf(stdout,"\n<<<<<Client %i want to upload file>>>>>\n\n", i);
			fflush(stdout);
			download(thread_socketfd,i);
		}
		else if( strcmp(buff,"delete")==0)
		{
			fprintf(stdout,"\n<<<<<Client %i want to delete file>>>>>\n\n", i);
			fflush(stdout);
			delete(thread_socketfd,i);
		}
		else if( strcmp(buff,"rename")==0)
		{
			fprintf(stdout,"\n<<<<<Client %i want to rename file>>>>>\n\n", i);
			fflush(stdout);
			rename_file(thread_socketfd,i);
		}

		bzero(buff, SIZE);
		//fprintf(stdout,"\n");
    		//fflush(stdout);
    		
    		
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

	listen(socketfd, 10);

	pthread_t thread_id[10];

	int i = 0;

	while(1)
	{
		//Accept call creates a new socket for the incoming connection
		client_address_size = sizeof(client_address);
		newsocketfd = accept(socketfd, (struct sockaddr*) &client_address, &client_address_size);

		if(newsocketfd < 0)
		{
			error_terminate("Error : No accept");
		}
		else
		{
			client_number++;
			fprintf(stdout,"Connection Stablishedd with Client %d !!!!!!!!!!!!!!!!!!!!\n",client_number);
			fflush(stdout);
		}

		if( pthread_create( &thread_id[i++] , NULL ,  client_connection_handler , (void*) &newsocketfd) < 0)
        {
            error_terminate("could not create thread");
            return 1;
        }
        else
        {
        	fprintf(stdout, "Handelar Assigned for Client %d !!!!!!!!!!!!!!!!!!!!\n", client_number);
			fflush(stdout);
        }

	}
		
	//close(thread_socketfd);
	close(socketfd);

	return 0;



}

