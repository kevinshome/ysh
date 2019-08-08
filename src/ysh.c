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

extern char *repstr(char *str, char *orig, char *rep);

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

      printf("\33[36m %s@%s (%s) \33[37m \n", user, hostname, cwd);

      line = readline("> ");
      ysh_hist_mgmt(line);
      args = split_line(line);
      status = yshexec(args);

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
    /* fputs("\n\
built using gcc 8.3.0 on debian stretch\n\
this binary was built with lots of love on 09.07.2019\n", stdout); */ //this part is for when i build binaries, ignore it
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


int main(int argc, char **argv){
  if(argc > 1){
    arghandle(argc, argv);
  }else{
    ysh();
  }
  return 0;
}
