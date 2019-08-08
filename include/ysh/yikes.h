/*

the yikes shell (ysh)

Copyright 2019 kevinshome

the yikes shell (ysh) is released under the Massachusettes Institute of Technology (MIT) License. terms
of the license can be found in the "LICENSE" file. if this file is not available,
a copy of the MIT License can also be found at https://opensource.org/licenses/MIT

*/

// this is the yikes shell (ysh) meta-include file
// i have no shame in admitting i stole this idea from CPython

#include <sys/wait.h>

#include <readline/history.h>
#include <readline/readline.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
