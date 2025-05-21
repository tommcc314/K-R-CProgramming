#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFSIZE 100
#define MAXOP   100
#define MAXVAL  100
#define NUMBER  '0'

int sp = 0, bufp = 0;
double val[MAXVAL];
char buf[BUFSIZE];

void push(double f) {
  if (sp < MAXVAL)
    val[sp++] = f;
  else
    printf("error: stack full, can't push %g\n", f);
}
double pop(void) {
  if (sp > 0)
    return val[--sp];
  else {
    printf("error: stack empty\n");
    return 0.0;
  }
}

int getch(void) {
  return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
  if (bufp >= BUFSIZE)
    printf("ungetch: too many characters\n");
  else
    buf[bufp++] = c;
}

int getop(char s[]) {
  int i = 0, c;
  while ((s[0] = c = getch()) == ' ' || c == '\t');
  s[1] = '\0';
  if (!isdigit(c) && c != '.') {
    if (c == '-') {     // 4.3: negatives
      if (isdigit(c = getch()) || c == '.')
        ungetch(c);
      else
        return c;
    }
    else
      return c;
  }
    
  if (isdigit(c))
    while (isdigit(s[++i] = c = getch()));
  if (c == '.')
    while (isdigit(s[++i] = c = getch()));
  s[i] = '\0';
  if (c != EOF)
    ungetch(c);
  return NUMBER;
}

int main(int argc, char* argv[]) {
  int type;
  double op1, op2;
  char s[MAXOP];

  while ((type = getop(s)) != EOF) {
    switch (type) {
      case NUMBER:
        push(atof(s));
        break;
      case '+':
        push(pop() + pop());
        break;
      case '*':
        push(pop() * pop());
        break;
      case '-':
        op2 = pop();
        push(pop() - op2);
        break;
      case '/':
        op2 = pop();
        if (op2 != 0.0)
          push(pop() / op2);
        else
          printf("error: zero division\n");
        break;
      case '%': // 4.3 modulo
        op2 = pop();
        op1 = pop();
        if (op2 != 0.0 && op1 - ((int) op1) == 0.0 && op2 - ((int) op2) == 0.0)
          push(((int) op1) % ((int) op2));
        else
          printf("error: invalid mod\n");
        break;
      case '\n':
        printf("\t%.8g\n", pop());
        break;
      default:
        printf("error: unknown command: %s\n", s);
        break;
    }
  }
  return 0;
}

