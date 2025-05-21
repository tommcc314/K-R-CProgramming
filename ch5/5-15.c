#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 5000
#define MAXLEN 1000 

char *lineptr[MAXLINES];

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

int readlines(char *lineptr[], int maxlines) { 
  int len, nlines; 
  char *p, line[MAXLEN]; 
  nlines = 0; 
  while ((len = customgetline(line, MAXLEN)) > 0) {
    if (nlines >= maxlines || (p = malloc(len * sizeof(char))) == NULL) 
      return -1; 
    else { 
      line[len-1] = '\0';
      strcpy(p, line); 
      lineptr[nlines++] = p; 
    } 
  }
  return nlines; 
} 

void writelines(char *lineptr[], int nlines) { 
  int i; 
  for (i = 0; i < nlines; i++) {
    printf("%s\n", lineptr[i]);
    free(lineptr[i]);
  }
}

int customstrcmp(char *s, char *t) { 
  int i; 
  for (i = 0; s[i] == t[i]; i++) 
    if (s[i] == '\0') 
      return 0; 
  return s[i] - t[i]; 
}

char upper(char c) {
  if (c >= 'a' && c <= 'z') return 'A' + (c - 'a');
  else return c;
}

int customstrcmpnocase(char *s, char *t) {
  int i; 
  for (i = 0; upper(s[i]) == upper(t[i]); i++) 
    if (s[i] == '\0') 
      return 0; 
  return upper(s[i]) - upper(t[i]);
}

int numcmp(char *s1, char *s2) { 
  double v1, v2; 
  v1 = atof(s1); 
  v2 = atof(s2); 
  if (v1 < v2) 
    return -1; 
  else if (v1 > v2) 
    return 1; 
  else 
    return 0; 
} 

void swap(void *v[],  int i, int j) { 
  void *temp; 
  temp = v[i]; 
  v[i] = v[j]; 
  v[j] = temp; 
}

void customqsort(void *v[], int left, int right, int greater, int (*comp)(void *, void *)) { 
  int i, last;  
  if (left >= right)
    return;
  swap(v, left, (left + right)/2); 
  last = left; 
  for (i = left+1; i <= right;  i++) 
    if ((greater ? -1 : 1) * (*comp)(v[i], v[left]) < 0) 
      swap(v, ++last, i); 
  swap(v, left, last); 
  customqsort(v, left, last-1, greater, comp); 
  customqsort(v, last+1, right, greater, comp); 
} 

int main(int argc, char* argv[]) {
  int nlines, numeric = 0, greater = 0, nocase = 0;
  for (int i = 1; i <= argc; ++i) {
    if (customstrcmp(argv[i - 1], "-n") == 0) numeric = 1;
    if (customstrcmp(argv[i - 1], "-r") == 0) greater = 1;
    if (customstrcmp(argv[i - 1], "-f") == 0) nocase = 1;
  }
  if (argc > 1 && customstrcmp(argv[1], "-n") == 0) 
    numeric = 1; 
  if ((nlines = readlines(lineptr, MAXLINES)) >= 0) { 
    customqsort((void**) lineptr, 0, nlines-1, greater,
      (int (*)(void*,void*))(numeric ? numcmp : nocase ? customstrcmpnocase : customstrcmp)); 
    writelines(lineptr, nlines); 
    return 0; 
  } else { 
    printf("input too big to sort\n"); 
    return 1; 
  } 
}