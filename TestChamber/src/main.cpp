#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "floatvar.h"

int main(int args, char* argv[]) {

  FVC::FloatVar fv(50, "abc", 5123);
  
  printf("V: %d\n", fv.getNumber<int>());
  printf("N: %s\n", fv.getName()); 
  
  printf("Hello from WanichusilkToolKit.TestChamber project!\n");
  return 0;
}
