/*

the yikes shell (ysh)

Copyright 2019 kevinshome

the yikes shell (ysh) is released under the Massachusettes Institute of Technology (MIT) License. terms
of the license can be found in the "LICENSE" file. if this file is not available,
a copy of the MIT License can also be found at https://opensource.org/licenses/MIT

*/

// src/builtins.c
// built in functions used in the yikes shell (ysh)

#include <ysh/yikes.h>

extern int remchar(char *s, char c);
extern int ysh_start(char **args);

char *builtin_str[] = {
  "cd",
  "exit",
  "echo",
  "eboys?",
  "which",
  "hello"
};

int ysh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

int ysh_hello(char **args){
  printf("hello there, i'm ysh!\ni suck right now, but i'm soon to be your favorite shell!\n(hopefully)\n");
  return 1;
}

int ysh_which(char **args){
  int i;
  for (i = 0; i < ysh_num_builtins(); i++) {
    if (strcmp(args[1], builtin_str[i]) == 0) {
      printf("%s: ysh built-in command\n", builtin_str[i]);
    }
  }
  ysh_start(args);
  return 1;
}

int ysh_eboys(char **args){
  printf("\neboys?\neboys are just self absorbed incels who\nare controlling, and use the way they dress to mask their\ndull and bland, trend following personality...\n\n");
  return 1;
}

int ysh_echo(char **args){
  if(strstr(args[1], "$") != NULL){
    char *envar = args[1];
    memmove(&envar[0], &envar[1], strlen(envar)); //remove $ char in front of requested envar
    envar = getenv(envar);
    printf(envar);
    printf("\n");
  } else { //if user is not trying to echo an env value, call /bin/echo
    ysh_start(args);
  }
  return 1;
}

int ysh_cd(char **args)
{

  char *homedir = getenv("HOME"); //grab current user's home dir

  if (args[1] == NULL) {
    chdir(homedir); //if user does not specify directory to go to, go to user's home directory
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
  return 1;
}

int ysh_exit(char **args)
{
  printf("exiting ysh, bye!\n");
  exit(0);
  return 1;
}

int (*builtin_func[]) (char **) = {
  &ysh_cd,
  &ysh_exit,
  &ysh_echo,
  &ysh_eboys,
  &ysh_which,
  &ysh_hello
};
