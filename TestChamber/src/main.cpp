#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include "floatvar.h"
#include "fvdisk.h"

int main(int args, char* argv[]) {

  FVC::FloatVar fv(-5.3f, "abc", "int_");
  FVC::FloatVar fv2("-5.3f", "serbia", "str_");
  
  FVC::FloatVar fv1 = FVC::FloatVar("def", "list");

  fv1.reformat(FVC::FloatVar::FormatType::list);
  fv1.push(fv);
  fv1.push(fv2);

  std::cout << "V: " << fv1.getString() << '\n';
  std::cout << "S: " << fv1.getSize() << '\n';
  std::cout << "N: " << fv1.getName() << '\n';
  std::cout << "F: " << (char)((char)fv1.getFormat() + 'A') << '\n';
  std::cout << "T: " << fv1.getType().dt.sym << '\n';

  FVC::FloatVarDisk fvd;
  fvd.insertFloatVar(fv1);
  fvd.save("abacada.txt");

  printf("Hello from WanichusilkToolKit.TestChamber project!\n");
  return 0;

  // sigsegv is called for FloatVar when it has null
}
