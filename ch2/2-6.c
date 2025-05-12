#include <stdio.h>

int setbits(unsigned int x, unsigned int p, unsigned int n, unsigned int y) {
  // int leftx = x >> (p + 1);
  // int middlex = (x >> (p+1-n)) & ~(~0 << n);
  // int rightx = x & ~(~0 << (p+1-n));
  // int righty = y & ~(~0 << n);
  return (x & (~0 << (p + 1))) | ((y & ~(~0 << n)) << (p + 1 - n)) | (x & ~(~0 << (p+1-n)));; 
}

int main(int argc, char* argv[]) {
  printf("%d\n", setbits(56, 3, 3, 17));
  return 0;
}