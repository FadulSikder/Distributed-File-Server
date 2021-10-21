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
#include <math.h>

#define SIZE 1024

char job_holder[20][150] = {0};
int job_number = 0 ;



void error_terminate(const char *message)
{

	perror ( message );
	exit(1);

}

int compare(const void * a, const void * b)
{
    return ( *(long*)a - *(long*)b );
}

double calculate_pi()
{
	double pi = 2 * acos(0.0);

	return pi;
}


long add_number(long add[])
{
	return add[0] + add[1]; 
}

void sort(long *sort_array, int elements)
{
	qsort(sort_array, elements, sizeof(long), compare);
}

void matrix_multiplication(int ar, int ac_br,int bc,int matrix_a[ar][ac_br],int matrix_b[ac_br][bc],int matrix_c[ar][bc])
{
	int i,j,k;
	for(i = 0; i < ar; ++i)
	{
		for(j = 0; j < bc; ++j)
		{
			for(k=0; k< ac_br; ++k)
			{
				matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
			}
		}
	}

	return;

}
   

void stub(int socketfd, char *buff)
{
	int i= 0;
	char buffer[SIZE];
	bzero(buffer, SIZE);
	char message_token[50][50] = {0};
	//memset(message_token, 0, 50*50*sizeof array[0][0]);
	char * token = strtok(buff, " ");
    // loop through the string to extract all other tokens
	while( token != NULL )
	{
        //printf( " %s\n", token ); //printing each token
		strcpy(message_token[i], token);
		i++;
    	token = strtok(NULL, " ");
    }
    /*int g = 0 ;
    while(g<i)
    {
        printf("%s\n", message_token[g]);
        g++;
    }
	*/
	memset(buff, 0 , SIZE);

    if(strcmp(message_token[0],"calculate_pi")==0)
    {

    	double pi = 0 ;
    	char tmp[10];
    	pi = calculate_pi();

    	strcat(buffer,"PI =");
    	sprintf(tmp, "%f", pi);
		strcat(buffer, " ");
		strcat(buffer, tmp);
		strcpy(buff, "Ack");
		strcpy(job_holder[job_number++], buffer);
		//send()
		//printf("%s\n", buffer);

    }
    else if(strcmp(message_token[0],"add")==0)
    {
    	long add[2], sum = 0;
    	char tmp[10];
    	int j = 0 ;

    	while(j<i){
    		char *ptr;
    		add[j] = strtol(message_token[j+1], &ptr, 10);
    		j++;
    	}
    	sum = add_number(add);

    	strcat(buffer,"Addition Result =");
    	sprintf(tmp, "%ld", sum);
		strcat(buffer, " ");
		strcat(buffer, tmp);
		strcpy(buff, "Ack");
		strcpy(job_holder[job_number++], buffer);
    	//printf("%s\n", buffer);

    }
   	else if(strcmp(message_token[0],"sort")==0)
    {
    	//printf("Entered Sort Func");
    	long sort_array[50] = {0};
    	char tmp[10];
    	int j = 0, m = 0 ;

    	while(j<i){
    		char *ptr;
    		sort_array[j] = strtol(message_token[j+1], &ptr, 10);
    		//printf("%ld\n",sort_array[j]);
    		j++;
    	}
    	sort(sort_array,j-1);

    	strcat(buffer,"Sorted Array =");

    	while(m<j-1)
    	{
	    	sprintf(tmp, "%ld", sort_array[m]);
	    	m++;
			strcat(buffer, " ");
			strcat(buffer, tmp);
		}
		strcpy(buff, "Ack");
		strcpy(job_holder[job_number++], buffer);
    	//printf("%s\n", buffer);
    }
    else if(strcmp(message_token[0],"mat_mul")==0)
    {
    	int a_row = 0, a_col_b_row = 0, b_col = 0,j = 0,m,n;
    	char tmp[10];
    	char *ptr;
    	a_row = strtol(message_token[++j], &ptr, 10);
    	a_col_b_row = strtol(message_token[++j], &ptr, 10);
    	b_col = strtol(message_token[++j], &ptr, 10);

    	int matrix_a[a_row][a_col_b_row], matrix_b[a_col_b_row][b_col],matrix_c[a_row][b_col];
    	memset( matrix_a, 0, a_row*a_col_b_row*sizeof(int) );
    	memset( matrix_b, 0, a_col_b_row*b_col*sizeof(int) );
    	memset( matrix_c, 0, a_row*b_col*sizeof(int) );
   		/*Counter variables for the loop*/

   		for(m=0; m<a_row; m++)
   		{
	      	for(n=0; n<a_col_b_row; n++)
	      	{
	        	matrix_a[m][n] = strtol(message_token[++j], &ptr, 10);
	      	}
   		}
   		for(m=0; m<a_col_b_row; m++)
   		{
	      	for(n=0;n<b_col;n++)
	      	{
	        	matrix_b[n][m] = strtol(message_token[++j], &ptr, 10);
	      	}
   		}

   		matrix_multiplication(a_row, a_col_b_row, b_col, matrix_a, matrix_b, matrix_c);

   		strcat(buffer,"Mutiplication Result Matrix C = \n");
   		for(m=0;m<a_row;m++)
	    {
	        for(n=0;n<b_col;n++)
	        {
	            //printf("%d\t",matrix_c[m][n]);
	            sprintf(tmp, "%d", matrix_c[m][n]);
				strcat(buffer, " ");
				strcat(buffer, tmp);
	        }
	        strcat(buffer, "\n");

		}
		strcpy(buff, "Ack");
		strcpy(job_holder[job_number++], buffer);
		//printf("%s\n", buffer);
	}
	else if(strcmp(message_token[0],"result")==0)
	{
		int req_job_number = 0, j = 0;
		char tmp[10];
    	char *ptr;
    	req_job_number = strtol(message_token[++j], &ptr, 10);
    	strcpy(buff, job_holder[req_job_number-1] );
    	return;

	}
	//send(socketfd, buffer, SIZE, 0);
	
	


}



int main(int argc, char *argv[])
{
	// Defining the IP and Port
	char *ip = "127.0.0.1";
	const int port_no = 8080;
	char buff[SIZE];
	bzero(buff, SIZE);

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

	//Accept call creates a new socket for the incoming connection
	client_address_size = sizeof(client_address);
	newsocketfd = accept(socketfd, (struct sockaddr*) &client_address, &client_address_size);

	if(newsocketfd < 0)
	{
		error_terminate("Error : No accept");
	}
	else
	{
		fprintf(stdout,"Connection Stablishedd with Client!!!!!!!!!!!!!!!!!!!!\n");
		fflush(stdout);
	}


	while(1)
	{
		check = recv(newsocketfd, buff, SIZE, 0);
		stub(newsocketfd,buff);
		send(newsocketfd, buff, SIZE, 0);
	}
		
	//close(thread_socketfd);
	close(socketfd);

	return 0;



}







/*while(1)
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

}*/
	