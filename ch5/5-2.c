#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFSIZE 100

int bufp = 0;
char buf[BUFSIZE];

int getch(void) {
  return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
  if (bufp >= BUFSIZE)
    printf("ungetch: too many characters\n");
  else
    buf[bufp++] = c;
}

int getfloat(double *pn) { 
  int c;
  double sign, decimal = 1.0;
  while (isspace(c = getch())); 
  if (!isdigit(c) && c != EOF && c != '+' && c != '-') { 
    ungetch(c);
    return 0; 
  } 
  sign = (c == '-') ? -1.0 : 1.0; 
  if (c == '+' || c == '-') 
    c = getch();
  for (*pn = 0; isdigit(c); c = getch()) 
    *pn = 10.0 * *pn + (c - '0');
  if (c == '.') {
    c = getch();
    for (*pn = 0; isdigit(c); c = getch(), decimal *= 10)
      *pn = 10.0 * *pn + (c - '0');
  }
  *pn /= decimal;
  *pn *= sign; 
  if (c != EOF) 
    ungetch(c); 
  return c; 
}

int main(int argc, char* argv[]) {
  int type;
  double f;

  while ((type = getfloat(&f)) != EOF) {
    if (type) {
      printf("%f", f);
    }
  }
}

