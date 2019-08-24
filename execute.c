#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <termios.h>
#include <sys/fcntl.h>
#define INPUT_STRING_SIZE 80

#define NORMAL 				00
#define OUTPUT_REDIRECTION 	11
#define INPUT_REDIRECTION 	22
#define BACKGROUND			44
#define OUTPUT_APP	55
#define For(i,begin,end) for(int i=(begin);i<(end);i++)

extern int top;
typedef struct proc
{
	int id;
	char process[1024];//stores mapping of process and process id
}proc;
extern struct termios shell_tmodes;

extern int shell_terminal;
extern pid_t shell_pgid;

extern int no;
extern proc pro[1024];
extern proc jobs[1024];
typedef void (*sighandler_t)(int);
extern char * home;
int cd(char **args)//cd command
{

	if(!args[1])//check for no argument in cd
	{
		chdir(home);
		return 1;
	}
	int i=0;
	char directory[1024];
	strcpy(directory,args[1]);
	while(directory[i] != '\0')
		i++;
	if(directory[i]=='~')
		if(i>0)
		perror("error");
		if(directory[0]=='~')
		{
			char array1[1024],array2[1024];
			int j=1;
			int m=0;
			for(;directory[j]!='\0';j++,m++)
				array1[m]=directory[j];

			int k=m;
			j=0;
			for(;home[j]!='\0';j++)
			{
				array2[j]=home[j];
			}
			For(l,0,k)
			{
				array2[j]=array1[l];
				j++;
			}
			array2[j]='\0';
			strcpy(directory,array2);
		}
		strcpy(args[1],directory);
		if(args[1][0]!='\0' && chdir(args[1])!=0)
			perror("error");

		return 1;
	}



int  pwd()//pwd command
{	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	printf("%s\n",cwd);
	return 1;
}


int fg_to_bg(proc x,proc y,int val)
{

	tcsetpgrp(shell_terminal,shell_pgid);
	tcsetattr (shell_terminal, TCSADRAIN, &shell_tmodes);	
	For(i,val - 1,no -1)
	{
		pro[i]=pro[i+1];
		jobs[i]=jobs[i+1];
	}

	pro[no-1]=x;
	jobs[no-1]=y;
	return 0;
}
int killall()
{	
	For(i,0,no)
	{	
		printf("Killing this Process: %d\n",pro[i].id );
		kill(pro[i].id,9);
	}
	no=0;
	return 1;
}
int echo(char **args,int argc) //echo command
{

	int bul = 1;
	while(argc > 1 && bul)
	{

		int j;
		for(j=0;args[1][j]!='\0';j++)
		{
			if(args[1][j]!='"')
				printf("%c",args[1][j]);

		}
		printf(" ");
		bul = 0;
	}
	int i;
	For(i,2,argc)
	{
		int j;
		for(j=0;args[i][j]!='\0';j++)
		{
			if(args[i][j]!='"')
				printf("%c",args[i][j]);

		}
		printf(" ");
	}

	printf("\n");
	return 1;
}

int bg_to_fg(char **args)
{
	int i=0;
	int val=0;
	while(args[1][i]!='\0')
	{
		val=10*val+args[1][i]-'0';
		i++;
	}
	int pgid=getpgid(pro[val-1].id);
	tcsetpgrp(STDIN_FILENO,pgid);
	kill(pro[val-1].id,SIGCONT);
	int status,wpid;
	int fdshrey =2;
	while(fdshrey < 3){
		fdshrey ++;
		break;
	}
	do 
	{
		wpid = waitpid(pro[val-1].id, &status, WUNTRACED);
		if(WIFSTOPPED(status))
		{
			fg_to_bg(pro[val-1],jobs[val-1],val);
			return 1;

		}

	}while (!WIFEXITED(status) && !WIFSIGNALED(status));//wait for foreground process*/
		tcsetpgrp(STDIN_FILENO,getpgid(getpid()));
		int c=0;
		int j=0;
		proc temp[1024];
		proc temp2[1024];
		for(c=0;c<no;c++)
		{
			if(c!=val-1)
			{	
				temp[j]=pro[c];
				temp2[j]=jobs[c];
				j++;
			}
		}
		no=j;
		for(c=0;c<no;c++)
		{
			pro[c]=temp[c];
			jobs[c]=temp2[c];
		}
		return 1;
		int new = 2;
		while(new < 3){
			new ++;
			break;
		}
	}
	int kjobs(char **args)
	{
		int i=0;
		int val=0;;
		while(args[1][i]!='\0')
		{
			val=10*val+args[1][i]-'0';
			i++;
			int fdshrey =2;
			while(fdshrey < 3){
				fdshrey ++;
				break;
			}
		}
		printf("%d\n",val);
		printf("%d" ,pro[val-1].id);

		int sig=0;
		i=0;
		while(args[2][i]!='\0')
		{
			sig=10*sig+args[2][i]-'0';
			i++;
		}
		printf("%d \n",sig);
		if(kill(pro[val-1].id,sig)<0)
			perror("failed");
		else
			fprintf(stderr, "successfully killed %d\n",pro[val-1].id );
		proc temp3[1024];
		proc temp4[1024];
		int c=0;
		int m=0;
		For(c,0,val)
		{
				//	pid_t pid_result = waitpid(pro[c].id, &status, WNOHANG);
			if(kill(pro[c].id,0)==-1 && errno==ESRCH)
				printf("Process %s  exited whose id is %d\n",pro[c].process,pro[c].id);
			else
			{	
				temp3[m]=pro[c];
				temp4[m]=jobs[c];
				m++;
			}
		}
		For(c,val,no)
		{
			if(kill(pro[c].id,0)==-1 && errno==ESRCH)
				printf("Process %s  exited whose id is %d\n",pro[c].process,pro[c].id);
			else
			{	

				temp3[m]=pro[c];
				temp4[m]=jobs[c];
				m++;
			}	
		}
		no=m;
		For(c,0,no)
		{
			pro[c]=temp3[c];
			jobs[c]=temp4[c];
		}

		return 1;
	}
	/*prints the prompt onto the stderr*/
	
	int printbgjobs()
	{
		int i=0;
		for(;i<no;i++)
		{
			printf("[%d] %s[%d]",i+1,jobs[i].process,jobs[i].id);
			printf("\n");

		}
		return 1;
	}


	void execute(char **cmdArgv, int mode, char **supplementPtr,int cmdArgc)
	{

		if(strcmp(*cmdArgv, "killallbg") == 0)
		{
			killall();
		}
		else if(strcmp("pinfo",*cmdArgv)==0)
		{

			char randstr[100],readstr[10004],randstr1[100],readstr1[1000];
			char *argvs[100],*argvs1[100];
			int pid =getpid();
			if(cmdArgv[1]==NULL)
			{
			        sprintf(randstr,"/proc/%d/stat",pid);
			        sprintf(randstr1,"/proc/%d/exe",pid);
			}
			else
			{
			        sprintf(randstr,"/proc/%s/stat",cmdArgv[1]);
			        sprintf(randstr1,"/proc/%s/exe",cmdArgv[1]);
			}
			int fd = open(randstr,O_RDONLY);
			int fd1 = open(randstr1,O_RDONLY);
			int numbyte = read(fd,readstr,10000);
			int numbyte1 = readlink(randstr1,readstr1,1000);
			char * wrd = strtok(readstr," ");
			int i=0;
			while(wrd!=NULL)
			{
			        argvs[i++]=wrd;
			        wrd=strtok(NULL," ");
			}
			printf("pid:\t\t %s\nProcess Status:  %s\nMemory: \t %s\nExecutable Path: %s\n",argvs[0],argvs[2],argvs[22],readstr1);
			
		}

		else if(strcmp(*cmdArgv, "jobs") == 0)
		{
			printbgjobs();
		}

		else if(strcmp(*cmdArgv, "pwd") == 0)
		{
			pwd();
			
		}
		else if(strcmp(*cmdArgv,"cd")==0){
			
			cd(cmdArgv);
		}


		
		else if(strcmp(*cmdArgv,"echo")==0)
		{	
			echo(cmdArgv,cmdArgc);
			
		}
		else if(strcmp(*cmdArgv,"kjob")==0)
		{
			kjobs(cmdArgv);
		}
		else if(strcmp(*cmdArgv,"fg")==0){
			if(cmdArgc==2)
			{
				int status,pgid5,pid5,child_pid;
				pid5=pro[atoi(cmdArgv[1])-1].id;
				if(pid5>=0)								//get the pgid of the job
				{
				//fprintf(stderr,"%s\n",getname(pid)->pname);                                        
					pgid5=getpgid(pid5);
					tcsetpgrp(shell_terminal,pgid5);						//give control of the terminal to the process
					child_pid=pgid5;
					if(killpg(pgid5,SIGCONT)<0)								//send a SIGCONT signal
						perror("can't continue!");
					waitpid(pid5,&status,WUNTRACED);						//wait for it to stop or terminate
					if(!WIFSTOPPED(status))
					{
					//remove					//if terminated remove from jobs table
						child_pid=0;
					}
					else
						fprintf(stderr,"\n[%d]+ stopped ",child_pid);		//if stopped print message
					tcsetpgrp(shell_terminal,shell_pgid);							//return control of terminal to the shell
			}
			else
				fprintf(stderr,"Error:Job doesn't exist\n");
		}
		else
			fprintf(stderr,"Please give one argument!\n");

	}

	else{ 

		pid_t pid;
		int status1;
		int i;
		
		pid = fork();

		if( pid < 0)
			exit(-1);
		
		else if(pid == 0)
		{

			if(mode == BACKGROUND)
			{
				setpgid(getpid(),getpid()); 
				sigset_t emptyset;
				sigemptyset(&emptyset);
				signal (SIGINT, SIG_DFL);
				signal (SIGQUIT, SIG_DFL);
				signal (SIGTSTP, SIG_DFL);
				signal (SIGTTIN, SIG_DFL);
				signal (SIGTTOU, SIG_DFL);
				signal (SIGCHLD, SIG_DFL);
			}
			int fd0;
			char input[64],output2[64];
			int fd1,fd2;
			int in = 0;
			int out = 0;
			int append = 0;
			char output[64];
			// finds where '<' or '>' or '>>' occurs and make that argv[i] = NULL , to ensure that command wont't read that
			i = 0;
			while(cmdArgv[i] != '\0')
			{
				if(strcmp(cmdArgv[i],"<")==0)
				{        
					cmdArgv[i]=NULL;
					strcpy(input,cmdArgv[i+1]);
					in=2;           
				}               

				else if(strcmp(cmdArgv[i],">>")==0)
				{      
					cmdArgv[i]=NULL;
					strcpy(output2,cmdArgv[i+1]);
					append=2;
				}         

				else if(strcmp(cmdArgv[i],">")==0)
				{      
					cmdArgv[i]=NULL;
					strcpy(output,cmdArgv[i+1]);
					out=2;
				}  
				i++;       
			}

			//if '<' char was found in string inputted by user
			if(in)
			{ 	
				if ((fd0 = open(input, O_RDONLY, 0)) < 0) 
					perror("Could not succesfully open file, verify permissions");

				if(dup2(fd0, 0)<0) // STDIN_FILENO here can be replaced by 0 
					printf("Duping has failed , cannot redirect the file as input\n");
				int fdshrey =2;
				while(fdshrey < 3){
					fdshrey ++;
					break;
				}
				close(fd0); // necessary
			}

			//if '>' char was found in string inputted by user 
			if (out)
			{
				if ((fd1 = creat(output , 0644)) < 0) 
					perror("Sorry , Couldn't open the output file , check the permissions");
				int fdshrey =2;
				while(fdshrey < 3){
					fdshrey ++;
					break;
				}
				if(dup2(fd1, STDOUT_FILENO)<0) // 1 here can be replaced by STDOUT_FILENO
					printf("duping failed , cannot redirect the output to file\n");
				close(fd1);
			}

			//if '>>' char was found in string inputted by user 
			if (append)
			{
				if ((fd2 = open(output2 ,O_WRONLY|   O_APPEND)) < 0) 
					perror("Sorry , Couldn't open the output file , check the permissions");

				if(dup2(fd2, STDOUT_FILENO)<0) // 1 here can be replaced by STDOUT_FILENO
					printf("duping failed , cannot redirect the output to file\n");
				close(fd2);
			}
			if ((execvp(*cmdArgv, cmdArgv) < 0)) {     // execute the command  
				printf("*** ERROR: exec failed\n");
				exit(1);
			}


		}
		else
		{
			if(mode == BACKGROUND)
			{ 
				setpgid(pid,pid);

				jobs[no].id=(int)pid;
				pro[no].id=(int)pid;
				strcpy(pro[no].process,cmdArgv[0]);
				strcpy(jobs[no].process,"\0");
				int j=0;
				while(cmdArgv[j]!=NULL)
				{	
					strcat(jobs[no].process,cmdArgv[j]);
					strcat(jobs[no].process," ");
					j++;
				}
				no++;		
			}

		else{
			
			waitpid(pid, &status1, 0);
		}
			//wait(NULL);
	}

}

}
