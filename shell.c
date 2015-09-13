#include<stdio.h>

#include<sys/wait.h>

#include<signal.h>

#include<stdlib.h>

#include<unistd.h>

#include<string.h>

int pid;

typedef struct jobs{

	int procid;

	char name[10];

	struct jobs* next;

}job;



void sig_handler(int signo){

	printf("\n");

	kill(pid,SIGSTOP);	

}



void sig_h(int signo){

	printf("\n");

}



void main(){

	signal(SIGTTOU, SIG_IGN);

	int status;

	int id[20];

	int m=0,n=0;

   

    job* list=(job *)malloc(sizeof(job)*20);

    int p=0;

 

	while(1){

		signal(SIGTSTP,sig_h);

		signal(SIGINT,sig_h);

		signal(SIGQUIT,sig_h);

		printf(">>");

			char cmd[30];

			char* c[20];

			//scanf("%s",cmd);

			fgets(cmd,30,stdin);

			if(cmd==NULL)

				continue;

			//if(cmd[0]=='q')

				//break;

			int flag=0;

			if(cmd[0]=='\n')

				continue;

			//break;

			

			if(cmd[strlen(cmd)-2]=='&')flag=1;

			

			cmd[strlen(cmd)-1] = '\0';

			//printf("%s",cmd);

			int i=0;

			char *s = strdup(cmd);

			//printf("%c",cmd[strlen(cmd)-1]);

			c[i]=strtok(s," ");

			while(c[i] && i<19) c[++i]=strtok(NULL," ");

			int j;

			c[i]=NULL;

			if(!strcmp(c[0],"q"))

				kill(getpid(),SIGKILL);

			//for(j=0;j<i;++j)printf("%s\t",c[j]);

			//printf("\n");

			

			pid=vfork();

		

		if(pid>0){

			signal(SIGINT,sig_h);

			signal(SIGQUIT,sig_h);

			signal(SIGCHLD,SIG_IGN);

			setpgid(getpid(),getpid());

			signal(SIGTSTP,sig_handler);

			tcsetpgrp(0,getpid());

			if(flag==1){

				continue;	

			}

			else

				waitpid(pid,&status,WUNTRACED);

		}

		else{

			//int sid = setsid();

			signal(SIGTSTP,sig_handler);

			

			if(c[0]==NULL)

				continue;

			

			else if(!strcmp(c[0],"stop")){

				if(c[1]==NULL){

					printf("Invalid number of arguments to stop\n");

					continue;

				}

				if(kill(atoi(c[1]),0))

					printf("Invalid Pid\n");

				kill(atoi(c[1]),SIGSTOP);

				continue;

			}

				

			else if(!strcmp(c[0],"start")){

				if(c[1]==NULL){

					printf("Invalid number of arguments to start\n");

					continue;

				}

				if(kill(atoi(c[1]),0))

					printf("Invalid Pid\n");

				kill(atoi(c[1]),SIGCONT);

				continue;

			}

			

			int z=0;

			int b=0;

			int f1=0;

			if(!strcmp(c[0],"jobs")){

				for(z=0;z<p;z++){

					if(kill(list[z].procid,0)==0){

							list[b++]=list[z];

							printf("%s\t",list[z].name);

							printf("%d\n",list[z].procid);

					} 		

					

				}

				p=b;

				kill(getpid(),SIGKILL);

				continue;

			}

			

			if(flag!=1){

				//printf("Cmp");

				signal(SIGTTOU, SIG_IGN);

				setpgid(getpid(),getppid());

				tcsetpgrp(0,getpgrp());

				if(execvp(c[0],c)==-1)

					printf("Error in execution\n");

			}

			else{	

				c[i-1]=NULL;

				//printf("YO");

				//fflush(stdout);

				signal(SIGTTOU, SIG_IGN);

				setpgid(getpid(),getpid());

				//system(cmd);

				list[p].procid=getpid();

				strcpy(list[p].name,c[0]);

				p++;

				if(execvp(c[0],c)==-1)

					printf("Error in execution\n");

			}

			exit(0);

		}

	}

}
