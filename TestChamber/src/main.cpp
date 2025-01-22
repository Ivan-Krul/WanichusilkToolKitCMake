#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include "floatvar.h"

int main(int args, char* argv[]) {

  FVC::FloatVar fv(-5.3f, "abc", '_tni');
  
  FVC::FloatVar fv1 = FVC::FloatVar(53, "def", 'beef');

  std::cout << "V: " << fv1.getNumber<int>() << '\n';
  std::cout << "N: " << fv1.getName() << '\n';
  std::cout << "F: " << (char)((char)fv1.getFormat() + 'A') << '\n';
  std::cout << "V: " << fv1.getType().dt.sym << '\n';

  printf("Hello from WanichusilkToolKit.TestChamber project!\n");
  return 0;
}
