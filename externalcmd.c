#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "externalcmd.h"
#include "variables.h"


void externalCommand(char *command, char *varargs){
  char **args = (char**) calloc(1, 80);
  int argc = 1;
  char *arg;
  char arg_delimiter[2] = " ";



  pid_t pid = fork();
  int status;

  if(pid == 0){
    int pathn;
    printf("PATHN1 : %d\n", pathn);
    char **paths = pathsToCommArr(&pathn, command);
    printf("PATHN2 : %d\n", pathn);

    while(varargs != 0){
      args[argc] = (char*) malloc(80);
      arg = strsep(&varargs, arg_delimiter);
      strcpy(args[argc], arg);
      argc++;
      args = realloc(args, (argc+1) * 80);
    }

    for(int i = 0; i < pathn; i++){
      args[0] = (char*) malloc(80);
      args[0] = paths[i];
      printf("PATHS : %s", paths[i]);
      execv(paths[i], args);
    }

  }
  else if(pid > 0){ //Parent
    if(WIFEXITED(status)){
      setExitcode(WEXITSTATUS(status));
    }
    else if(WIFSIGNALED(status)){
      printf("Process terminated due to SIGNAL %d\n", WTERMSIG(status));
    }
  }
  else{
    perror("waitpid()");
  }

  waitpid(pid, &status, 0);
}

char** pathsToCommArr(int *pathn, char *program){
  char *paths = value("PATH");

  char **patharr = (char**) calloc(1,100);
  char delimiter[2] = ":";

  int pathnL = 0;

  while(paths!=0){
    char *path = strsep(&paths, delimiter);
    patharr[pathnL] = (char*) malloc(100);
    strcpy(patharr[pathnL], path);
    strcat(patharr[pathnL], "/");
    strcat(patharr[pathnL], program);
    patharr = (char**) realloc(patharr, pathnL+1 * 100);
    pathnL++;
  }

  *pathn = pathnL;

  return patharr;
}