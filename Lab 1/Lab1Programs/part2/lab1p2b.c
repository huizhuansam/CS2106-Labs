#include <stdio.h>

int accumulate(int x) {
  static int acc = 0;
  acc = acc + x;
  printf("acc is now %d\n", acc);
}

int main() {
  for(int i = 1; i <= 10; i++) {
    accumulate(i);
  }
}
