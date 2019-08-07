/*

the yikes shell (ysh)

Copyright 2019 kevinshome

the yikes shell (ysh) is released under the Massachusettes Institute of Technology (MIT) License. terms
of the license can be found in the "LICENSE" file. if this file is not available,
a copy of the MIT License can also be found at https://opensource.org/licenses/MIT

*/

//built in function definitions header for the yikes shell (ysh)

extern int ysh_cd(char **args);
extern int ysh_exit(char **args);
extern int ysh_echo(int argc, char **argv);
extern int ysh_num_builtins();
extern char *builtin_str[];
extern int (*builtin_func[]) (char **);
