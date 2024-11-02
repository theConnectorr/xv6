#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
  const int maxLen = 64;     // do dai toi da cua mot chuoi
  char ch[1];                // bo dem ky tu
  char lineBuf[512];         // bo dem chuoi
  char *linePtr = lineBuf;   // con tro ghi chuoi vao lineBuf
  char *cmdArgs[MAXARG];     // mang luu cac doi so moi
  char **argPtr = cmdArgs;   // con tro toi mang cmdArgs
  int argCount = 0;          // so doi so
  int baseArgCount = 0;      // so doi so ban dau

  // sao chep doi so ban dau vao cmdArgs
  while (argv[argCount + 1])
  {
    argPtr[argCount] = (char *)malloc(sizeof(char) * maxLen);
    strcpy(argPtr[argCount], argv[argCount + 1]);
    argCount++;
    baseArgCount++;
  }

  // doc ky tu tu STDIN
  while (read(0, ch, 1) > 0)
  {
    if (*ch == '\n')
    { 
      *linePtr = '\0';
      argPtr[argCount] = (char *)malloc(sizeof(char) * maxLen);
      strcpy(argPtr[argCount], lineBuf);
      argCount++;

      if (fork() == 0)
      { 
        if (exec(argv[1], argPtr) < 0)
        {
          printf("%s: exec failed in xargs\n", argv[1]);
        }
      }
      else
      {
        wait(0);

        // giai phong bo nho cho doi so tren dong nay
        while (--argCount >= baseArgCount)
        {
          *argPtr[argCount] = '\0';
          free(argPtr[argCount]);
        }

        // reset lai argCount va linePtr
        argCount = baseArgCount;
        linePtr = lineBuf;
      }
    }
    else if (*ch == ' ')
    { 
      *linePtr = '\0';
      argPtr[argCount] = (char *)malloc(sizeof(char) * maxLen);
      strcpy(argPtr[argCount], lineBuf);
      argCount++;
      linePtr = lineBuf;
    }
    else
    { 
      *linePtr = *ch;
      linePtr++;
    }
  }

  // giai phong doi so ban dau
  while (--baseArgCount >= 0)
  {
    free(argPtr[baseArgCount]);
  }
  exit(0);
}
