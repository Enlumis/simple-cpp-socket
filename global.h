#ifndef CGLOBAL_H_
# define CGLOBAL_H_

#define coutprefix __FILE__<<":"<<__LINE__<<"> "

static bool  gg_exit = false;

#define PACKET_ID_MAX 10

static void   my_strncpy(char *dest, char *str, int n)
{
  int   i;

  i = 0;
  while (i < n)
    {
      dest[i] = str[i];
      i++;
    }
}

#endif