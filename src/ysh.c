/*

the yikes shell (ysh)

Copyright 2019 kevinshome

the yikes shell (ysh) is released under the Massachusettes Institute of Technology (MIT) License. terms
of the license can both be found below (only in this file), and in the "LICENSE" file. if neither of these are available,
a copy of the MIT License can also be found at https://opensource.org/licenses/MIT

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall
be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

// src/ysh.c
// the main file for ysh, where all the main shit happens

#include <ysh/yikes.h>

#include <ysh/defines.h>
#include <ysh/builtins.h>

int release = 2;
/*
0 > regular binary build
1 > nightly build
2 (or really anything else) > development (github master branch) release
*/
extern char *repstr(char *str, char *orig, char *rep);
extern int alias_num;
char *lineforit;
char *anotherstr;

int exists(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}
int filestuffs(const char *filename){
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    int num = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        if(strstr(line, "ref: ref/heads/") == 0){
          int numhere2 = 0;
          anotherstr = malloc(128);
          for(int numhere = 16; numhere<strlen(line); numhere++){
            anotherstr[numhere2] = line[numhere];
            numhere2++;
          }
        }
      }

    fclose(fp);
    if (line)
        free(line);
    return 1;
}

char *read_line(void){
  char *line = NULL;
  size_t bufsize = 0;
  getline(&line, &bufsize, stdin);
  return line;
}

char **split_line(char *line)
{
  int bufsize = TOKEN_BUFFER, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "ysh: allocation error\n");
    exit(1);
  }

  token = strtok(line, TOKEN_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += TOKEN_BUFFER;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "ysh: allocation error :(\n");
        exit(1);
      }
    }

    token = strtok(NULL, TOKEN_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

int ysh_start(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror("ysh");
    }
    exit(1);
  } else if (pid < 0) {
    perror("ysh");
  } else {
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
      if(wpid == 0){
        /*
        this only exists to get the gcc debugger off of my back,
        obviously, it will never be used, since wpid was assigned right before,
        but since i'm now using it in an if statement here, gcc thinks it's now being used,
        so it won't print out the "var not used" error, fuck you gcc
        */
        printf("fuck you gcc, you piece of shit\n");
      }
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int yshexec(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < ysh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }
  return ysh_start(args);
}

int ysh_aliaschk(char **args){
  int i;

  for (i = 0; i < alias_num; i++) {
    if (strcmp(args[0], aliases_lt[i]) == 0) {
      args[0] = definitions_lt[i];
      yshexec(args);
      return 2;
    }
  }
  return 1;
}

int ysh_hist_mgmt(char *line){

  char *histfname = malloc(128);
  char *user = getenv("USER");

  sprintf(histfname, "%s%s%s", "/home/", user, "/.ysh_hist");
  repstr(line, "\n", "");
  add_history(line);
  write_history(histfname);
  free(histfname);

  return 1;
}

void signal_callback_handle(){
  /*
  okay so, a little explanation of why i've set this up like i have

  1). the SIGINT callback handler has to be void, so i can't simply return a value, bc gcc gets mad at me
  2). i can't just define hiimcarl (look, jimmy neutron was the shit) and not use it, bc gcc gets mad at me

  either way, this setup returns no compile-time errors, and does what it needs to successfully
  so don't critique it, and especially don't put it on r/badcode, please.
  thx,
  mgmt
  */

  int hiimcarl = 0;
  if(hiimcarl != 0){
    hiimcarl = 0;
  }
}

void ysh(void){
    char *line;
    char **args;
    int status;

    do{

      char *user = getenv("USER");

      char hostname[1024];
      hostname[1023] = '\0';
      gethostname(hostname, 1023);

      long size;
      char *buf;
      char *cwd;

      size = pathconf(".", _PC_PATH_MAX);

      if ((buf = (char *)malloc((size_t)size)) != NULL){
        cwd = getcwd(buf, (size_t)size);
        cwd = repstr(cwd, "/home/noah", "~");
      }


      char *fname = malloc(128);
      strcpy(fname, getcwd(buf, (size_t)size));
      strcat(fname, "/.git/HEAD");
      if(exists(fname) == 1){
        filestuffs(fname);
        printf("\33[36m %s@%s (%s)\n\
\33[31m git branch - %s\33[37m", user, hostname, cwd, anotherstr);
      }else {
        printf("\33[36m %s@%s (%s) \33[37m \n", user, hostname, cwd);
      }

      line = readline("> ");
      ysh_hist_mgmt(line);
      args = split_line(line);

      if(ysh_aliaschk(args) == 2){
        status = 1;
      }else{status = yshexec(args);}

      free(line);
      free(args);
    } while (status);
}

void helpmenu(void){
    printf("ysh: the yikes shell\n");
    fputs("usage: ysh [OPTION]\n\n\
-v, --version    show program version and exit\n\
-h, --help       show this menu and exit\n\n\
ysh, (C) 2019 kevinshome\n", stdout);
    if(release == 0){
    fputs("\n\
built using gcc 8.3.0 on debian stretch\n\
this binary was built with lots of love on xx.xx.2019\n", stdout);
} else if(release == 1){
      fputs("\n\
built using gcc 8.3.0 on debian stretch (nightly build)\n\
this binary was built with lots of love on xx.xx.2019\n", stdout);
    } else{
      fputs("\n\
ysh development release\n", stdout);
    }
}

static struct option const longopts[] =
{
  {"help", no_argument, NULL, 'h'},
  {"version", no_argument, NULL, 'v'}
};

int arghandle(int argc, char **argv){
  int optc;

  while ((optc = getopt_long (argc, argv, "hv", longopts, NULL)) != -1)
  {
    switch (optc)
      {
        case 'h':
        helpmenu();
        break;

        case 'v':
        printf("ysh 0.1\njet fuel can't melt steel beams\n");
        break;

      }
  }

  return 0;
}

int ysh_init(char *filename){
  char line[100];
  FILE *file = fopen(filename, "r");

  if(file == NULL) {

   printf("Error opening ~/.yshrc\nCreating blank file at ~/.yshrc\n");
   mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
   creat(filename, mode);
   return 1;

  }

  while (fgets(line, 100, file) != NULL) {

    char *init_line = malloc(128);
    char **init_args;

    strcpy(init_line, line);

    init_args = split_line(init_line);
    yshexec(init_args);

    free(init_line);
    free(init_args);

  }

  return 1;
}

int main(int argc, char **argv){

  //ysh init
  char *user = getenv("USER");
  char *filename = malloc(128);
  sprintf(filename, "%s%s%s", "/home/", user, "/.yshrc");
  ysh_init(filename);
  free(filename);
  //end ysh init

  signal(SIGINT, signal_callback_handle);
  //signal(SIGTSTP, signal_callback_handle);
  // i'll implement this later when i find a fix bc as of right now, pressing ctrl-z will crash the entire fucking console

  if(argc > 1){
    arghandle(argc, argv);
  }else{
    ysh();
  }
  return 0;
}
