#include<stdio.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<pthread.h>
#include<termios.h>
#include<stdlib.h>
#define MAXCLIENT 3
struct sockaddr_in ser,cli;
void *recv_thread(void *arg);
void *send_thread(void *arg);
pthread_t sample_t[MAXCLIENT];
int sfd,cfd[MAXCLIENT],temp=0;
int main()
{
int len,num;
pthread_t snd,rcv[MAXCLIENT];
sfd=socket(AF_INET,SOCK_STREAM,0);
ser.sin_family=AF_INET;
inet_aton("127.0.0.1",&(ser.sin_addr));
ser.sin_port=htons(8760);
num=bind(sfd,(struct sockaddr*)&ser,sizeof(ser));
printf("Chatroom Ready!\n");
num=listen(sfd,MAXCLIENT);
len=sizeof(cli);
while(temp<MAXCLIENT)
{
cfd[temp]=accept(sfd,(struct sockaddr*)&cli,&len);
printf("Client[%d] Joined!\n",temp+1);
int *ptr=(int *)malloc(sizeof(int));
*ptr=temp;
if(temp==0)
pthread_create(&snd,NULL,send_thread,NULL);
pthread_create(&rcv[temp],NULL,recv_thread,(void *)ptr);
temp++;
}
pthread_join(snd,NULL); //Termination of rcv thread is handled
within snd itself.
return 0;
}

void *send_thread(void *arg)
{
char str[200];
int pos;
while(1)
{
pos=0;
fgets(str,200,stdin);
if(strcmp(str,"!DISCONNECT")==10)
{
while(pos<=temp-1)
{
if(pthread_cancel(sample_t[pos])==0)

pos++;

}
close(sfd);
exit(0);
}
else
{
while(pos<=temp-1)
{
write(cfd[pos],(void *)str,sizeof(str));

pos++;

}
}
}
}
void *recv_thread(void *arg)
{
int n;
char str[200];
int pos=*((int *)arg);
sample_t[pos]=pthread_self();
while(1)
{
n=read(cfd[pos],(void *)str,sizeof(str));
if(n>0)
{
if(strcmp(str,"!DISCONNECT")==10)
{
printf("Client[%d] Left!\n",pos+1);
}
else
{
printf("Client[%d] : %s",pos+1,str);
}
fflush(stdout);
}

}
}
