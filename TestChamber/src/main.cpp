#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>

#include "floatvar.h"
#include "fvdisk.h"

#undef FLOAT_VAR_INDEX_GUARD

int main(int args, char* argv[]) {
  float abc[] = { -9.0f,1.2f,5.2f };

  FVC::FloatVar fv(abc, 3, "abc", "aray");

  FVC::FloatVarDisk fvd;
  fvd.insertFloatVar(fv);
  fvd.save("sup.txt");

  printf("Hello from WanichusilkToolKit.TestChamber project!\n");
  return 0;
}
