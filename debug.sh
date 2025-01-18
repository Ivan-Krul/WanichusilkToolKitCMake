if [ ! -d "out" ]; then
  echo "the program hasn't been built"
  read -p "Press Enter to continue" </dev/tty
  exit 1
fi

if [ -d "./out/bin" ]; then
  gdb -x debug_commands.txt --args "./out/bin/TestChamber.exe"
else
  gdb -x debug_commands.txt --args "./out/build/bin/TestChamber.exe"
fi
  
read -p "Press Enter to continue" </dev/tty