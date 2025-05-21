#include <stdio.h>

static int daytab[2][13] = {
  {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

int day_of_year(int year, int month, int day) {
  int *p, leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
  p = *(daytab + leap) + 1;
  while (--month)
    day += *p++;
  // for (i = 1; i < month; i++)
  //   day += daytab[leap][i];
  return day;
}

void month_day(int year, int yearday, int *pmonth, int *pday) {
  int *p, leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
  p = *(daytab + leap)+ 1;
  while (yearday > *p)
    yearday -= *p++;
  // for (i = 1; yearday > daytab[leap][i]; i++)
  //   yearday -= daytab[leap][i];
  *pmonth = p - *(daytab + leap);
  *pday = yearday;
}

int main(int argc, char* argv[]) {
  int year = 1457, month = 3, day = 24, pmonth, pday, yearday;
  yearday = day_of_year(year, month, day);
  printf("yearday: %d\n", yearday);
  month_day(year, yearday, &pmonth, &pday);
  if (month == pmonth && day == pday) {
    printf("backwards check success!\n");
  }
  else {
    printf("error: backwards check failed. pmonth = %d, pday = %d\n", pmonth, pday);
  }
}