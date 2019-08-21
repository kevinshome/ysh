/*

the yikes shell (ysh)

Copyright 2019 kevinshome

the yikes shell (ysh) is released under the Massachusettes Institute of Technology (MIT) License. terms
of the license can be found in the "LICENSE" file. if this file is not available,
a copy of the MIT License can also be found at https://opensource.org/licenses/MIT

*/

// src/remchar.c
// holds function for replacing parts of a string
//
// took this from some stackoverflow answer and didn't want
// to put it in the main ysh.c file so i put it here (i plan to relocate it soon, it really doesn't need it's own file)

#include <ysh/yikes.h>

char *repstr(char *str, char *orig, char *rep)
{
  static char buffer[4096];
  char *p;

  if(!(p = strstr(str, orig))) // Is 'orig' even in 'str'?
    return str;

  strncpy(buffer, str, p-str); // Copy characters from 'str' start to 'orig' st$
  buffer[p-str] = '\0';

  sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));

  return buffer;
}
