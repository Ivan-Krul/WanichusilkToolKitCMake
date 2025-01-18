#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "floatvar.h"

int main(int args, char* argv[]) {

  FVC::FloatVar fv(50, "abc", '_tni');
  
  FVC::FloatVar fv1 = fv;

  printf("V: %d\n", fv1.getNumber<int>());
  printf("N: %s\n", fv1.getName()); 
  printf("F: %c\n", (char)fv1.getFormat() + 'A');
  printf("T: %.4s\n", (char*)fv1.getType().dt.sym);
  
  printf("Hello from WanichusilkToolKit.TestChamber project!\n");
  return 0;
}
