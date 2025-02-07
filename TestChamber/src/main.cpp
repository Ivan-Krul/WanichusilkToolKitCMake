#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>

#include "floatvar.h"
#include "fvdisk.h"

#undef FLOAT_VAR_INDEX_GUARD

int main(int args, char* argv[]) {
  FVC::FloatVarDisk fvd;
  fvd.load("abacada.txt");
  FVC::FloatVar fv = fvd.pullFloatVar();

  std::cout << "N: " << fv.getName() << '\n';
  std::cout << "S: " << fv.getSize() << '\n';

  for (const auto& elem : fv) {
    std::cout << "\tN: " << elem.getName() << '\n';
  }

  printf("Hello from WanichusilkToolKit.TestChamber project!\n");
  return 0;
}
