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
  "hello",
  "alias"
};

int ysh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

int alias_num = 0;
int alias_max = 5;
int do_not_alloc = 0;
int notinarr = 1;

char **aliases_lt;
char **definitions_lt;

int ysh_alias(char **args){

  if(do_not_alloc == 0){ //initial array size allocation
    aliases_lt = malloc(sizeof(*aliases_lt) * alias_max);
    definitions_lt = malloc(sizeof(*definitions_lt) * alias_max);
    do_not_alloc++;
  }

  if(strcmp("list",args[1]) == 0){
    int i;
    for (i = 0; i < alias_num; i++){
      printf("%s >> %s\n", aliases_lt[i], definitions_lt[i]);
    }
  }else {
    if(args[2] == NULL){
      for (int i = 0; i < alias_num; i++){
        if(strcmp(args[1], aliases_lt[i]) == 0){
          printf("%s >> %s\n", aliases_lt[i], definitions_lt[i]);
          notinarr = 0;
        }
      }
      if(notinarr == 0){
        notinarr = 1;
        return 1;
      }else if(notinarr == 1){
        printf("%s: not an alias\n", args[1]);
        return 1;
      }
    }
    if(alias_num == alias_max - 1){
      alias_max = alias_max + 5;

      char **temp_array = realloc(aliases_lt, sizeof(*aliases_lt) * alias_max);
      if (temp_array)
      {
          aliases_lt = temp_array;
      }
    }
      int alias_len = strlen(args[1]);
      int def_len = strlen(args[2]);

      aliases_lt[alias_num] = malloc(alias_len + 5);
      definitions_lt[alias_num] = malloc(def_len + 5);

      sprintf(aliases_lt[alias_num], "%s", args[1]);
      sprintf(definitions_lt[alias_num], "%s", args[2]);
      alias_num++;
  }
  return 1;
}

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
  &ysh_hello,
  &ysh_alias
};
