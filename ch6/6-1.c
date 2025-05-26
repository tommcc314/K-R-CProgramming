#include <stdio.h> 
#include <ctype.h> 
#include <string.h>
#include <stdlib.h> 

#define MAXWORD 100
#define BUFSIZE 100

int bufp = 0, string = 0, multiline_comment = 0;
char buf[BUFSIZE];

struct tnode {
  char *word;
  int count;
  struct tnode *left;
  struct tnode *right;
};

struct tnode *talloc(void) { 
  return (struct tnode *) malloc(sizeof(struct tnode)); 
} 

char *customstrdup(char *s) { 
  char *p; 
  p = (char *) malloc(strlen(s)+1);
  if (p != NULL) 
    strcpy(p, s); 
  return p; 
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

int getword(char *word, int lim) { 
  int c, getch(void); 
  void ungetch(int); 
  char *w = word; 
  while (isspace(c = getch())) ; 
  if (c != EOF) 
    *w++ = c;
  if (!isalpha(c) && c != '_') {
    switch (c) {
      case '"':
        string = !string;
        break;
      case '/':
        *w = getch();
        if (*w == '*') multiline_comment = 1;
        ungetch(*w);
        break;
      case '*':
        *w = getch();
        if (*w == '/') multiline_comment = 0;
        ungetch(*w);
        break;
      default:
        break;
    }
    *w = '\0'; 
    return c; 
  }
  for ( ; --lim > 0; w++) 
    if ((!isalnum(*w = getch()) && !string && *w != '_') || (string && (*w == '"' || *w == EOF))) { 
      ungetch(*w); 
      break; 
    } 
  *w = '\0'; 
  return word[0]; 
} 

struct tnode *addtree(struct tnode *p, char *w) { 
  int cond; 
  if (p == NULL) {
    p = talloc();
    p->word = customstrdup(w); 
    p->count = 1; 
    p->left = p->right = NULL; 
  } else if ((cond = strcmp(w, p->word)) == 0) 
    p->count++;
  else if (cond < 0)
    p->left = addtree(p->left, w); 
  else
    p->right = addtree(p->right, w); 
  return p; 
}

void treeprint(struct tnode *p) { 
  if (p != NULL) { 
    treeprint(p->left); 
    printf("%4d %s\n", p->count, p->word); 
    treeprint(p->right); 
  } 
} 

int main(int argc, char* argv[]) {
  struct tnode *root; 
  char word[MAXWORD]; 
  root = NULL; 
  while (getword(word, MAXWORD) != EOF) 
    if (isalpha(word[0]) && !multiline_comment) 
      root = addtree(root, word); 
  treeprint(root); 
  return 0; 
}