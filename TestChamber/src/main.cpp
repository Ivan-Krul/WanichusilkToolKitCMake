#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "floatvar.h"

int main(int args, char* argv[]) {
  FVC::FloatVar fv;
  
  srand(time(NULL));
  
  int ran = rand() % 10;
  
  fv.list.resize(ran);
  
  printf("Capacity: %d\n", fv.list.capacity());
  
  for(int i = 0; i < ran; i++) {
    fv.list[i].binary_length = rand();
  }

  for(int i = 0; i < fv.list.size(); i++) {
    printf("\t%d\n", fv.list[i].binary_length);
  }
  
  printf("Hello from WanichusilkToolKit.TestChamber project!\n");
  return 0;
}
