#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "floatvar.h"

int main(int args, char* argv[]) {

  FVC::FloatVar fv(50, "abc", '_tni');
  
  printf("V: %d\n", fv.getNumber<int>());
  printf("N: %s\n", fv.getName()); 
  printf("F: %c\n", (char)fv.getFormat() + 'A');
  printf("T: %.4s\n", (char*)fv.getType().dt.sym);
  
  printf("Hello from WanichusilkToolKit.TestChamber project!\n");
  return 0;
}
