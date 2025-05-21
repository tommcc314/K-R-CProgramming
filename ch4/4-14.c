#include <stdio.h>

#define swap(t,x,y) t temp = x; x = y; y = temp;

int main(int argc, char* argv[]) {
  int x = 5, y = 6;
  swap(int, x, y);
  printf("x=%d,y=%d\n", x, y);
}