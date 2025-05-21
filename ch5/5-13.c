#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXLINE 10000
#define MAXROWS 10000
#define DEFAULTTAIL 10

int customgetline(char *s,int lim) { 
  int c, i; 
  for (i=0; i < lim-1 && (c=getchar())!=EOF && c!='\n'; ++i) 
    s[i] = c; 
  if (c == '\n') { 
    s[i] = c; 
    ++i; 
  } 
  s[i] = '\0'; 
  return i; 
}

int main(int argc, char* argv[]) {
  char line[MAXLINE], nflag[MAXLINE], *lines[MAXROWS], *p;
  int nlines = 0, n = DEFAULTTAIL, c, len;
  if (argc > 1 && (*++argv)[0] == '-') {
    switch (c = *++argv[0]) {
      case 'n':
        strcpy(nflag, *++argv);
        n = atoi(nflag);
        argc = 1;
        break;
      default:
        printf("tail: illegal option %c\n", c);
        argc = 0;
        break;
    }
  }
  if (argc != 1) 
    printf("Usage: tail -n pattern\n");
  else {
    for (; nlines < MAXROWS && (len = customgetline(line, MAXLINE)) > 0; ++nlines) {
      p = malloc(len * sizeof(char));
      line[len - 1] = '\0';
      strcpy(p, line);
      lines[nlines] = p;
    }
    for (int i = nlines - n < 0 ? 0 : nlines - n; i < nlines; ++i) {
      printf("%s\n", lines[i]);
    }
    for (int i = 0; i < nlines; ++i) {
      free(lines[i]);
    }
  }
  
}