now=$(date +%s)sec
if [ ! -d "out" ]; then
  echo "the program hasn't been built"
  read -p "Press Enter to continue" </dev/tty
  exit 1
fi

if [ -d "./out/bin" ]; then
  ./out/bin/TestChamber.exe
else
  ./out/build/bin/TestChamber.exe
fi

echo
echo "---------------------------------"
echo "return code: " $?
echo "time elapsed: " $(TZ=UTC date --date now-$now +%H:%M:%S.%N)
read -p "Press Enter to continue" </dev/tty