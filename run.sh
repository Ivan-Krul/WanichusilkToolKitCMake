now=$(date +%s)sec
./build/bin/TestChamber.exe
echo "time elapsed: " $(TZ=UTC date --date now-$now +%H:%M:%S.%N)
read -p "Press Enter to continue" </dev/tty