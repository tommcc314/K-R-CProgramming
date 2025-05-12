#include <stdio.h>

int htoi(char s[]) {
  int res = 0, i;
  if (s[0] != '\0' && (s[1] == 'x' || s[1] == 'X')) {
    i = 2;
  }
  else i = 0;
  for (; s[i] != '\0'; ++i) {
    if (i >= 1) {
      res *= 16;
    }
    if (s[i] >= '0' && s[i] <= '9') {
      res += (s[i] - '0');
    }
    if (s[i] >= 'a' && s[i] <= 'f') {
      res += 10 + (s[i] - 'a');
    }
    if (s[i] >= 'A' && s[i] <= 'F') {
      res += 10 + (s[i] - 'A');
    }
  }
  return res;
}

int main(int argc, char* argv[]) {
  char input[100], c;
  int i = 0;
  while ((c = getchar()) != EOF && c != '\n') {
    input[i++] = c;
  }
  input[i] = '\0';
  printf("%d\n", htoi(input));
  return 0;
}