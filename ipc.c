#include "types.h"
#include "user.h"
#include "stat.h"
#include "fcntl.h"
#include "ipc.h"

int itr = 0; // Key value iterator

char * strcat(char *a,char *b)
{
	char *t = malloc(200);
	int i = 0,k = 0;
	
	while(a)
		t[i++] = a[k++];
	k = 0;
	
	while(a)
		t[i++] = b[k++];
	
	t[i] = '\0';

	return t;
}

char * ktof(int k)
{
	int i = 0;
	char *str = (char *) malloc(sizeof(int) * 100);
	while(k)
	{
		int d = k % 10;
		str[i++] = d + '0';
		k /= 10;
	}
	
	str[i] = '\0';
	return str;
}

int shmat(int k,int *fd,int mode)
{
	*fd = open(ktof(k),mode);
	if(*fd <= 0)
		return -1;
	
	return *fd;
}

int shmget(int k,int *fd)
{
	*fd = open(ktof(k),O_CREATE);
	if(*fd <= 0 ) // Failure
		return -1;
	
	return *fd;
}

int find(int k)
{
	return 0;
}

int toInt(char *a)
{
	int i = strlen(a)-1;
	int ans = 0;
	int weight = 1;
	while(i>=0)
	{
		int dig = a[i] - '0';
		ans += dig * weight;
		weight *= 10;
		i--;
	}

	return ans;
}

int isInt(char *a)
{
	while(*a)
	{
		if(*a >= '0' && *a <= '9'){}
		else
			return 0;		
		a++;
	}
	return 1;
}

void man()
{
	char man[] = "----------  IPC MANUAL DOCUMENTATION ------------\nList of Commands :\n\nSHARED MEMORY : \n\nipc shmget [key-id]  -----> Create a Shared Memory Segment with key = key-id\n\n\nipc shmdt [key-id]  -----> Delete the Created Shared memory segment\n\n\nipc shmat [key-id] [mode (RD/WR)]  -----> Attach to the Shared Memory segment and Read or Write with respect to the given mode specs\n\n--------------------------------------------------\n";

	printf(1,"%s",man);
}

int main(int argc,char **argv)
{
	
	//Decode syntax
	if(argc == 2 && !strcmp(argv[1],"man"))
	{
		man();
		goto exit;
	}
	else if( (argc == 3 || argc == 4) && ( !strcmp(argv[1],"shmat") || !strcmp(argv[1],"shmget") || !strcmp(argv[1],"shmdt"))&& isInt(argv[2]))
	{
		int key = toInt(argv[2]);
		int fd = -1; //FILE DESCRIPTOR
		if(!strcmp(argv[1],"shmat"))
		{
			if(argc == 3 || ( strcmp(argv[3],"RD") && strcmp(argv[3],"WR") && strcmp(argv[3],"RDWR")) )
				goto bad;

			int mode;
			if(!strcmp(argv[3],"RD"))
				mode = O_RDONLY;
			else if(!strcmp(argv[3],"WR"))
				mode = O_WRONLY;
			else
				mode = O_RDWR;

		
			if(shmat(key,&fd,mode) != -1)
			{
				if(mode == O_WRONLY)
				{
					printf(1,"ipc write > ");
					char c;
					while(read(0,&c,1) && c!='\n') 
							write(fd,&c,1);		
				}
				if(mode == O_RDONLY)
				{
					printf(1,"Enter bytes to read > ");
					int i  = 0;
					char str[20],c;
					while(read(0,&c,1) && c!='\n' &&write(fd,&c,1))
							str[i++] = c;

					int bytes = toInt(str);

					printf(1,"ipc read > ");				
					while(read(fd,&c,1) == 1 && bytes--)
							printf(1,"%c",c);
				}	

				printf(1,"\n");
			}
			else
				printf(1,"Attach failed\n");
		}	
		else if(!strcmp(argv[1],"shmget"))
		{
			printf(1,"Shared memory is created with key : %d\n",key);
			int shmid = shmget(key,&fd);
			printf(1,"SHMID : %d\n",shmid);
		}
		else	
		{

			if(shmget(key,&fd) == -1)
				printf(1,"Detach Failure - fatal error (shm not found)\n");	
			else
			{
				char **args = malloc(2);
				*args = malloc(100);

				args[0] = "rm\0";
				args[1] = ktof(key);
				printf(1,"Detach Successful\n"); 
				exec("rm",args);
			}	
		}	
		goto exit;
	}
	else
		goto bad;
	
	bad :
		printf(1,"Syntax Error : use \"ipc [mode] [number] [RD.WR]\" use \"ipc man \" for help\n");

	exit :
		exit();
}
