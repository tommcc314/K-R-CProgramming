#include <stdio.h>
#include <string.h>

#define MAXLINE 1000

int main(int argc, char *argv[]) {
  char line1[MAXLINE], line2[MAXLINE];
  FILE *fp1, *fp2;
  if (argc != 3) {
    printf("fdiff: require 2 files to compare. %d provided\n", argc - 1);
    return 1;
  }
  fp1 = fopen(argv[1], "r"), fp2 = fopen(argv[2], "r");
  while (fgets(line1, MAXLINE - 2, fp1) && fgets(line2, MAXLINE - 2, fp2) && !strcmp(line1, line2));
  if (strcmp(line1, line2))
    printf("Diff found:\nFile 1: %s\nFile 2: %s\n", line1, line2);
  else
    printf("No diff found\n");
  return 0;
}