#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define MAX_LINE		80 /* 80 chars per line, per command */

int getLength(char *str){
	int i=0;
	while(str[i]!='\0'){
		i++;
	}
	return i+1;
}

int main(void)
{
	pid_t child_pid, parent_pid;
	char buffer[MAX_LINE];		//Buffer to hold user input
    	int should_run;	
 	char dont_wait=0;
	int i=0, upper=0, len=0;

			
    while (1){   
        printf("my_shell>");
        fflush(stdout);
	dont_wait=0;
	fgets(buffer,MAX_LINE,stdin);
	len=getLength(buffer);
	
	for(i=0;i<len;i++){
		if(buffer[i]=='&') dont_wait=1;
	}

	child_pid = fork();

	if(child_pid<0){
		fprintf(stderr,"Error ocurred when trying to fork");
		return 1;
	}else if(child_pid==0){		//If child process is running
		char *cmd[MAX_LINE]={"/bin/bash","-c",buffer};
		execvp(cmd[0],cmd);
	}else if(child_pid > 0){	//If parent process is running
		wait(NULL);		//This does not wait because of how execvp works, all exec functions do not return to code, only return when error found
	}
    }   
	return 0;
}
