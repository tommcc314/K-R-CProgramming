#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE 3
static struct nlist *hashtab[HASHSIZE];

struct nlist {
  struct nlist *next;
  char *name;
  char *defn;
}; 

unsigned hash(char *s) { 
  unsigned hashval; 
  for (hashval = 0; *s != '\0'; s++) 
    hashval = *s + 31 * hashval;
  return hashval % HASHSIZE; 
} 
    
struct nlist *lookup(char *s) { 
  struct nlist *np; 
  for (np = hashtab[hash(s)];  np != NULL; np = np->next) 
    if (strcmp(s, np->name) == 0) 
      return np;
  return NULL;
}

struct nlist *predlookup(char *s) { 
  struct nlist *np, *pred = hashtab[hash(s)];
  for (np = hashtab[hash(s)];  np != NULL; np = np->next) 
    if (strcmp(s, np->name) == 0) 
      return pred;
    pred = np;
  return NULL;
} 

struct nlist *install(char *name, char *defn) { 
  struct nlist *np; 
  unsigned hashval; 
  if ((np = lookup(name)) == NULL) {
    np = (struct nlist *) malloc(sizeof(*np)); 
    if (np == NULL || (np->name = strdup(name)) == NULL) 
      return NULL; 
    hashval = hash(name); 
    np->next = hashtab[hashval]; 
    hashtab[hashval] = np; 
  } else
    free((void *) np->defn);
  if ((np->defn = strdup(defn)) == NULL) 
    return NULL; 
  return np; 
} 

int undef(char *name) {
  struct nlist *np, *pred;
  unsigned hashval = hash(name);
  if ((np = lookup(name)) == NULL) return 0;
  else {
    if (!(pred = predlookup(name))) {
      printf("undef: name not found in lookup table\n");
    }
    else if (pred != hashtab[hashval])
      pred->next = np->next;
    else 
      hashtab[hashval] = np->next;
    free(np);
  }
  return 1;
}

void printtable() {
  for (int i = 0; i < HASHSIZE; ++i)
    for (struct nlist *np = hashtab[i]; np != NULL; np = np->next)
      printf("name: %s, definition: %s\n", np->name, np->defn);
}

int main(int argc, char* argv[]) {
  install("lmao", "funny");
  install("omg", "surprising");
  install("bruh", "disappointed");
  install("cap", "lying");
  undef("bruh");
  printtable();
}