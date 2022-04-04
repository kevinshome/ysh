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

int dev_build = 1;
extern char *repstr(char *str, char *orig, char *rep);
extern int alias_num;

char *git_branch_str;

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

    while ((read = getline(&line, &len, fp)) != -1) {
        if(strstr(line, "ref: ref/heads/") == 0){
          int numhere2 = 0;
          git_branch_str = malloc(128);
          for(int numhere = 16; (line[numhere] != '\n'); numhere++){
            if(line[numhere] == '\n'){
              break;
            }
            git_branch_str[numhere2] = line[numhere];
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
    fprintf(stderr, "ysh: allocation error :(\n");
    exit(1);
  }

  token = strtok(line, TOKEN_DELIM);
  //printf("%s", token);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    /*if(strcmp(tokens[position], '"') == 0){
      printf("str delim found\n");
    }*/

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
  pid_t pid;
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
      waitpid(pid, &status, WUNTRACED);
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

void _handle_sigint_sigstp(){
  exit(0);
}

void ysh(void){
    char *line;
    char **args;
    int status;

    do{

      char *user = getenv("USER");

      char hostname[128];
      hostname[127] = '\0';
      gethostname(hostname, 127);

      long size;
      char *buf;
      char *cwd = NULL;

      size = pathconf(".", _PC_PATH_MAX);

      if ((buf = (char *)malloc((size_t)size)) != NULL){
        cwd = getcwd(buf, (size_t)size);
        char *homedir = malloc(64);
        sprintf(homedir, "/home/%s", user);
        cwd = repstr(cwd, homedir, "~");
        free(homedir);
      }

      printf("\33[36m %s@%s (%s) \33[37m \n", user, hostname, cwd);
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
    char *compiler = malloc(32);
    char *devrelease = malloc(32);
    #if defined(__GNUC__) && !defined(__clang__)
      sprintf(compiler, "GCC %d.%d.%d", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    #elif __clang__
      sprintf(compiler, "Clang %d.%d.%d", __clang_major__, __clang_minor__, __clang_patchlevel__);
    #endif

    if (dev_build){
      sprintf(devrelease, "--- DEVELOPMENT BUILD ---\n");
    } else {
      sprintf(devrelease, "");
    }

    char *buildmsg = malloc(128);
    sprintf(buildmsg, "\n\
built using %s with lots of love on %s\n%s", compiler, __DATE__, devrelease);

    printf("ysh: the yikes shell\n");
    fputs("usage: ysh [OPTION]\n\n\
-v, --version    show program version and exit\n\
-h, --help       show this menu and exit\n\n\
ysh, (C) 2019 kevinshome\n", stdout);
    fputs(buildmsg, stdout);
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
        printf("ysh 0.3\ncome alone with me, and the butterflies and bees\n");
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
  #ifndef __APPLE__
  sprintf(filename, "%s%s%s", "/home/", user, "/.yshrc");
  #else
  sprintf(filename, "%s%s%s", "/Users/", user, "/.yshrc");
  #endif
  ysh_init(filename);
  free(filename);
  //end ysh init

  signal(SIGINT, _handle_sigint_sigstp);
  signal(SIGTSTP, _handle_sigint_sigstp);

  if(argc > 1){
    arghandle(argc, argv);
  }else{
    ysh();
  }
  return 0;
}
