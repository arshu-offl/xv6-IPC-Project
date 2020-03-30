#include "types.h"
#include "user.h"
#include "stat.h"
#include "fcntl.h"

int key = 999; // contains db

int main()
{

	
	char **args = malloc(3);
	args[0] = "ipc";
	args[1] = "shmget";
	args[2] = "999";
		
	int pid = fork();
	if(!pid)
	{
		exec("ipc",args);
		exit();
	}
	else{
		wait();
		while(1)
		{
			printf(1,"\n1.Register\n2.Login\nEnter your choice : ");
			int choice;
			int fd;
			fd = open("999",O_RDWR); //open db
			read(1,&choice,4);
			if(choice == 1)
			{
				printf("\nEnter username : ");

				char ch;
				while(1)
				{
					read(0,&ch,1);
					if(ch == '\n')
						break;
				}

				if(check(fd,username)){
					printf("\nAlready Exists!\n");
				}
			}
		}
		exit();
	}
}