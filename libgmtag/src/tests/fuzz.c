#include "libgmtag.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (void)
{
  srand(time(NULL));

  char something[2048] = {0};
  something[0] = '#';
  for (size_t i = 1; i < 2048; i++)
  {
    switch (rand() % 64)
    {
      case 0:
      {
        something[i] = '#';
      }
      break;
      case 1:
      {
        something[i] = '\n';
      }
      break;
      case 2:
      {
        something[i] = (char)(rand() % 256);
      }
      break;
      default:
      {
        something[i] = (char)((rand() % ('Z' - '0')) + '0');
      }
      break;
    }
  }
  something[2048 - 1] = '\0';

  printf("%s", something);

  GmTagObject data = tags_from_buffer(something);
  unset_tags(data);

  return 0;
}
