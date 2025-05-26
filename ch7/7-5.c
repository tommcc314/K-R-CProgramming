#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXOP   100
#define MAXVAL  100
#define NUMBER  '0'

int sp = 0;
double val[MAXVAL];

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

int getop(char s[], char line[], int pos, int *n) {
  int cnt;
  double inp_f;
  char inp_c;
  if ((cnt = sscanf(line + pos, " %lf%n", &inp_f, n)) > 0) {
    sprintf(s, "%lf", inp_f);
    return NUMBER;
  }
  else if ((cnt = sscanf(line + pos, " %c%n", &inp_c, n)) > 0) {
    sprintf(s, "%c", inp_c);
    return inp_c;
  }
  return 0;
}

int main(int argc, char* argv[]) {
  int type, pos, n, error = 0;
  double op1, op2;
  char s[MAXOP], *line = NULL;
  size_t size = 0;

  while (getline(&line, &size, stdin) > 0) {
    pos = 0, error = 0;
    while ((type = getop(s, line, pos, &n)) > 0) {
      pos += n;
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
          else {
            printf("error: zero division\n");
            error = 1;
          }
          break;
        case '%':
          op2 = pop();
          op1 = pop();
          if (op2 != 0.0 && op1 - ((int) op1) == 0.0 && op2 - ((int) op2) == 0.0)
            push(((int) op1) % ((int) op2));
          else {
            printf("error: invalid mod\n");
            push(op1);
            error = 1;
          }
          break;
        case '\n':
          break;
        case '\0':
          break;
        default:
          printf("error: unknown command: %s\n", s);
          break;
      }
    }
    if (!error) printf("\t%.8g\n", pop());
  }
  return 0;
}

