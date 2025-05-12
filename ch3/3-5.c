#include <stdio.h>

void reverse(char s[]) {
  char temp;
  int i = 0, j = 0;
  while (s[j++] != '\0');
  j -= 2;
  for (; i < j; i++, j--) {
    temp = s[i];
    s[i] = s[j];
    s[j] = temp;
  }
}

void itob(int n, char s[], int b) {
  char encoding[b];
  for (int i = 0; i < b; ++i) {
    if (i < 10) encoding[i] = '0' + i;
    if (i > 9 && i < 36) encoding[i] = 'A' + i - 10;
    if (i > 35) encoding[i] = 'a' + i - 36;
  }
  int i = 0, sign;
  if ((sign = n) < 0) {
    n = -n;
  }
  do {
    s[i++] = encoding[(n % b)];
  } while ((n /= b) > 0);
  if (sign < 0) {
    s[i++] = '-';
  }
  s[i] = '\0';
  reverse(s);
}

int main(int argc, char* argv[]) {
  char s[100];
  itob(13469235, s, 24);
  printf("%s\n", s);
}