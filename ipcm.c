#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


int find(int a)
{
	int count=1;
	while(a>9)
	{
		count++;
		a/=10;
	}
	return count;
}

int toint(char *s)
{
	int i=0,k=1,ans=0;
	int size=strlen(s)-1;
	if(size<0)
		return -1;
	for(i=0;i<size;i++)
		k*=10;
	i=0;
	while(s[i]!='\0')
	{
		if(s[i]>='0'&& s[i]<='9')
		  {
			int temp=(s[i]-'0')*k;
			ans+=temp;
		   }
		else
			return -1;
		k/=10;
		i++;
	}
	return ans;
}

void concat(char *a,char *b,char *c)
{
	int i;
	int n1=strlen(a);
	int n2=strlen(b);
	n2+=n1;
	for(i=0;i<n1;i++)
		c[i]=a[i];
	for(i=n1;i<n2;i++)
		c[i]=b[i-n1];
	c[i]='\0';	
}

char * tostr(int a)
{
	int i=0;
	int size=find(a);
	char *str=(char*)malloc(sizeof(int)*(size+1));
	for(i=size-1;i>=0;i--)
	{
		int temp=a%10;
		str[i]=temp+'0';
		a/=10;
	}
	str[size]='\0';
	return str;
}

int msgget(int key,int *queue)
{
	int size=find(key);
	char c[size+1];
	int temp = open("q.c",O_RDWR);
		read(temp,c,size);
		close(temp);
	*queue = open("testing.c",O_CREATE);
	if(toint(c)==key)
	{
		printf(1,"\n\nMessage Queue is already Present:\n\n");
	}	
	else
	{
		temp=open("q.c",O_RDWR);		
		write(temp,tostr(key),strlen(tostr(key)));
		close(temp);
		printf(1,"\n\nMessage Queue has been created With Key:%d\n\n",key);
	}
	return temp;
}
int check(int id,int key)
{
	
	int size=find(key);
	char c[size+1];
	int temp = open("q.c",O_RDWR);
		read(temp,c,size);
	if(toint(c)==key)
		return 1;
	else
		return 0;
}
int message_send(int key,int *id,char * message,int size)
{
	
	char temp[strlen(message)+2];
	concat("1",message,temp);
	*id = open("testing.c",O_RDWR);
	if(*id <= 0)
		return -1;	
	
	char c[2];
	read(*id,c,1);
	close(*id);
	if(toint(c)==1)
		printf(1,"\n\nA Message sent is waiting to Be recieved...\n\n");

	else
	{
		*id = open("testing.c",O_RDWR);
		write(*id,temp,size);
		close(*id);
		printf(1,"\n\nMessage Sent Successfully.....\n\n");
	}
	return *id;
}

int message_rcv(int key,int *id,int size)
{
	int i;
	*id = open("testing.c",O_RDWR);
	if(*id <= 0)
		return -1;	
	char c[size+2], q[2];
	read(*id,q,1);
	close(*id);
	int temp=toint(q);
	if(temp==1 || temp==0)		
	{
		*id = open("testing.c",O_RDWR);
		char l;
		i=0;
		while(read(*id,&l,1) && l!='\n'&& i<size+1)	
		c[i++]=l;
		c[i]='\0';
		close(*id);
		printf(1,"\n\nThe Message is :");
			for(i=1;i<strlen(c);i++)
			printf(1,"%c",c[i]);
		printf(1,"\n\n");
		c[0]='0';
		*id = open("testing.c",O_RDWR);
		write(*id,c,size+2);
		close(*id);		
	}	
	else 
		printf(1,"\nNo Message Available:\n\n");		
	return *id;
}
void man()
{
	printf(1,"\t\tIPC MESSAGE QUEUE-MANUAL\t\t\n\n");
	printf(1,"1. msgget [key] --- To Create A Message Queue With the given Key\n\n2. msgsnd [key] [size]--enter--<Message>--- To Send A Message With the given Key\n\n3. msgrcv [key] [size] --- To recieve A Message With the given Key\n\n");
exit();
}



void failed()
{
	printf(1,"\n\nIPC Message Queue failed, use msg man to View Manual.\n\n");
		exit();
}
int main(int argc, char *argv[])
{
  	int id=0,qid=0;
  	if(argc <=1)
	{
    	printf(1,"\n\nUse ipcm man to view the manual\n\n");
    	exit();
  	}
	else if(argc==2) 
	{
		if(!strcmp(argv[1],"man"))
			man();
		else
			failed();
	}
	else if(argc==3) 
	{
		
		if(!strcmp(argv[1],"msgget")&& toint(argv[2])!=-1)			
			{
				open("q.c",O_CREATE);
				qid=msgget(toint(argv[2]),&id);
			}
		else
			failed();
	}
	else if(argc==4)	
	{
		int a=toint(argv[2]);
		int b=toint(argv[3]);		
		
		if(!strcmp(argv[1],"msgrcv")&&b!=-1)
			{
				if(check(qid,a))
					message_rcv(a,&id,b);
				else
				{
					printf(1,"\nInvalid Key:\n");
					failed();
				}
			}
		else if(!strcmp(argv[1],"msgsnd")&& b!=-1 && a!=-1)
		{
				if(check(qid,a)==0)
				{	
					printf(1,"\nInvalid Key:\n");
					failed();
				}
				else
				{
					printf(1,"\n\nEnter Your Message:");
					int i=0;
					char text[b];
					char c;
					while(read(0,&c,1) && c!='\n'&& i<b)		
					text[i++]=c;
					text[i]='\0';			
					message_send(a,&id,text,b); 
				
					
				}
		}
	
		else
			failed();
	}
	else
	{
		failed();
	}
   exit();
}

