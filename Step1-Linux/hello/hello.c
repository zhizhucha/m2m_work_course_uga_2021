#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

extern ssize_t getline(char **lineptr, size_t *n, FILE *stream);

int main(){
  char* line=NULL;
  size_t len=0;
  ssize_t count=0;

  // wait 2 seconds, so that the kernel can finish printing stuff.
  // indeed, some of the boot process is asynchronous and happens
  // after the initial process has been started.
  sleep(2);

  // Be polite, ask for the user name...
  printf("Hello! What is your name: ");
  fflush(stdout);
  
  while (1) {
    count = getline(&line,&len,stdin);
    if (count==-1) {
      printf("\n\nI am not hearing you, bye.\n");
			printf("I am going to die now...\n\n\n");
			fflush(stdout);
      return -1;
    }
    if (count>1)
      break;
    free(line);
    line = NULL;
    len = 0;
  }
  line[count-1]='\0';
  printf("Nice to greet you %s, welcome.\n",line);
  printf("To terminate this conversation, just say \"bye\".\n");
    
  // Now enter parrot mode...
  while(1){
    free(line);
    line = NULL;
    len = 0;
    if (count>1)
      printf("What say you? ");
    else 
      printf("I beg your pardon? ");
    fflush(stdout);
    count = getline(&line,&len,stdin);
    if (count==-1) {
      printf("\n\nI lost you, bye.\n");
			printf("I am going to die now...\n\n\n");
			fflush(stdout);
      return -1;
    }
    if (count>1) {
      line[count-1]='\0';
			if (0==strcmp(line,"bye")) {
				printf("\n\nHave a good one then!\n\n");
				printf("I am going to die now...\n\n\n");
				fflush(stdout);
				return 0;
			}
			else
				printf("You said: %s, very well.\n",line);
    }
  }
}
