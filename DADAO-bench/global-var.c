//
// global variables
//
#include<stdlib.h>

struct Type {
  int x;
  struct Type *next;
} type, *Stype;

void foo() {
  Stype = (struct Type *)malloc(sizeof(type));
  free(Stype);
}

int main(int argc, char *argv[]) {
  foo();
}
