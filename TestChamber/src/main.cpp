#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>

#include "floatvar.h"
#include "fvdisk.h"

#undef FLOAT_VAR_INDEX_GUARD

int main(int args, char* argv[]) {

  FVC::FloatVar fv;

  {
    FVC::FloatVarDisk fvd;
    if (fvd.load("abacada.txt")) printf("error: %s", fvd.pullError());
    fv = fvd.pullFloatVar();
  }

  printf("Hello from WanichusilkToolKit.TestChamber project!\n");
  return 0;

  // sigsegv is called for FloatVar when it has null
}
