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
int job_number = 1 ;

void error_terminate(const char *message)
{

	perror ( message );
	exit(1);

}

void stub(int socketfd,int job)
{
	char buffer[SIZE];
	bzero(buffer, SIZE);

	if(job == 1)
	{
		strcat(buffer, "calculate_pi");
		
	}
	else if(job == 2)
	{
		int number;
		char tmp[10];

		strcat(buffer, "add");

		printf("Please give the first number: ");
		scanf("%d", &number);

		sprintf(tmp, "%d", number);
		strcat(buffer, " ");
		strcat(buffer, tmp);

		printf("Please give the second number: ");
		scanf("%d", &number);

		sprintf(tmp, "%d", number);
		strcat(buffer, " ");
		strcat(buffer, tmp);

	}
	 else if(job == 3)
	{
		char tmp[100];
		memset(tmp,0, 100);
		strcat(buffer, "sort");

		printf("Please give the numbers by using space between them and press enter at the end: ");
		fgets(tmp, 100, stdin);
		strcat(buffer, " ");
		strcat(buffer, tmp);
		//printf("%s\n", buffer);

	}
	else if(job == 4)
	{
		char tmp[100];
		memset(tmp,0, 100);
		int a_row, a_collum__b_row, b_collum,i;
		strcat(buffer, "mat_mul");


		printf("Please give the Matrix A row number: ");
		scanf("%d", &a_row);
		int ch;
		while ( (ch = getchar()) != '\n' && ch != EOF) { };
		sprintf(tmp, "%d", a_row);
		strcat(buffer, " ");
		strcat(buffer, tmp);
		//memset(tmp,0, 100);

		printf("Please give the Matrix A collum number( Matrix B should also have the same number of row): ");
		scanf("%d", &a_collum__b_row);
		//int ch;
		while ( (ch = getchar()) != '\n' && ch != EOF) { };
		sprintf(tmp, "%d", a_collum__b_row);
		strcat(buffer, " ");
		strcat(buffer, tmp);
		//memset(tmp,0, 100);

		printf("Please give the Matrix B collum number: ");
		scanf("%d", &b_collum);
		//int ch;
		while ( (ch = getchar()) != '\n' && ch != EOF) { };
		sprintf(tmp, "%d", b_collum);
		strcat(buffer, " ");
		strcat(buffer, tmp);
		//memset(tmp,0, 100);

		for(i= 0; i < a_row ; i++)
		{
			printf("Enter the elements of the row %d of Matrix A (by using space between the number) : ", i+1);
			fgets(tmp, 100, stdin);
			tmp[strcspn(tmp, "\n")] = 0;
			strcat(buffer, " ");
			strcat(buffer, tmp);
		}

        fflush(stdin);

		for(i= 0; i < a_collum__b_row ; i++)
		{
			printf("Enter the elements of the row %d of Matrix B (by using space between the number) : ", i+1);
			fgets(tmp, 100, stdin);
			tmp[strcspn(tmp, "\n")] = 0;
			strcat(buffer, " ");
			strcat(buffer, tmp);
		}
	}
	else
	{
		char tmp[10];
		strcat(buffer, "result");
		job = job -4 ;
		sprintf(tmp, "%d", job);
		strcat(buffer, " ");
		strcat(buffer, tmp);
	}

	send(socketfd, buffer, SIZE, 0);

}


void calculate_pi(int socketfd)
{
	char buffer[SIZE];
	bzero(buffer, SIZE);
	stub(socketfd,1);
	recv(socketfd, buffer, SIZE, 0);
	if(strcmp(buffer,"Ack")==0)
	{
		printf("	Your request have been accepted with Job Number %d\n Please look for the result in your next request\n",job_number++);
		fflush(stdout);
	}

}

void add_two_number(int socketfd)
{
	char buffer[SIZE];
	bzero(buffer, SIZE);
	stub(socketfd,2);
	recv(socketfd, buffer, SIZE, 0);
	if(strcmp(buffer,"Ack")==0)
	{
		printf("	Your request have been accepted with Job Number %d\n Please look for the result in your next request\n",job_number++);
		fflush(stdout);
	}
}

void sort_a_array(int socketfd)
{
	char buffer[SIZE];
	bzero(buffer, SIZE);
	stub(socketfd,3);
	recv(socketfd, buffer, SIZE, 0);
	if(strcmp(buffer,"Ack")==0)
	{
		printf("	Your request have been accepted with Job Number %d\n Please look for the result in your next request\n",job_number++);
		fflush(stdout);
	}
}

void multiply_two_matrix(int socketfd)
{
	char buffer[SIZE];
	bzero(buffer, SIZE);
	stub(socketfd,4);
	recv(socketfd, buffer, SIZE, 0);
	if(strcmp(buffer,"Ack")==0)
	{
		printf("	Your request have been accepted with Job Number %d\n Please look for the result in your next request\n",job_number++);
		fflush(stdout);
	}
}

void asking_job_result(int socketfd)
{
	int i = 1,req_job_num = 0 ;
	char buffer[SIZE];
	bzero(buffer, SIZE);
	printf("Job resul that are available : \n");
	fflush(stdout);
	while(i<job_number)
	{
		printf("Job %d\n", i++);
		fflush(stdout);
	}
	printf("Please choose Job Number you want result for(1/2/3.....) : ");
	fflush(stdout);
	scanf("%d",&req_job_num);
	int ch;
	while ( (ch = getchar()) != '\n' && ch != EOF) { };

	stub(socketfd,req_job_num+4);
	recv(socketfd, buffer, SIZE, 0);
	printf("!!!!!!!!!!The returned result for Job %d !!!!!!!!!!!!\n%s\n",req_job_num,buffer);
	fflush(stdout);
	

}


int main(int argc, char *argv[])
{
	
	// Defining the IP and Port
	char *ip = "127.0.0.1";
	const int port_no = 8080;
	
	// Defining variables
	int socketfd, n;
	struct sockaddr_in server_address;
	
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
	    	fprintf(stdout,"Connection has Stablished with the Server!!!!!!!!!!!!!!\n");
	    	fflush(stdout);
	}
	
	//Creating UI and choosing which function to perform
	while(1)
	{
		if(job_number==1)
		{
			fprintf(stdout,"\nWhat operation do you want to do?\n1. Calculate PI\n2. Add Two Numbers\n3. Sort An Array\n4. Multiply Two Matrix\n0. Terminate the program.\nPlease type the number of the option you want to perform(1/2/3/4/0): ");
			fflush(stdout);
		}
		else
		{
			fprintf(stdout,"\nWhat operation do you want to do?\n1. Calculate PI\n2. Add Two Numbers\n3. Sort An Array\n4. Multiply Two Matrix\n5. Request a Privious Job Result\n0. Terminate the program.\nPlease type the number of the option you want to perform(1/2/3/4/5/0): ");
			fflush(stdout);
		}
		setbuf(stdin, NULL);
		scanf("%d", &n);
		int ch;
		while ( (ch = getchar()) != '\n' && ch != EOF) { };
		
		fprintf(stdout,"\n");
    	fflush(stdout);
		
		switch(n)
		{
			case 1:
				calculate_pi(socketfd);
				break;
				
			case 2: 
				add_two_number(socketfd);
				break;
				
			case 3:
				sort_a_array(socketfd);
				break;
				
			case 4:
				multiply_two_matrix(socketfd);
				break;

			case 5:
				asking_job_result(socketfd);
				break;
		
		}

		if(n == 0)
		{
			fprintf(stdout,"Terminating the client program!!!!!!!!!!!!!!\n");
    		fflush(stdout);
    		break;
		}
		fprintf(stdout,"\n");
    	fflush(stdout);
    
    }
    
    close(socketfd);
    
    return 0;
}
