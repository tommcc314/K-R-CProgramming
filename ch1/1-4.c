#include <stdio.h>

int main(int argc, char* argv[]) {
  float fahr, celsius, lower, upper, step;
  lower = 0;
  upper = 100;
  step = 10;
  celsius = lower;
  while (celsius <= upper) {
    fahr = (9.0/5.0) * celsius + 32;
    printf("%3.0f %6.1f\n", celsius, fahr);
    celsius = celsius + step;
  }
}