#!/bin/bash
sudo rmmod chronos 2>/dev/null
echo "=== Config A: Idle ==="
sudo insmod chronos.ko
sleep 60
cat /proc/chronos > report/config_A.txt
sudo rmmod chronos

echo "=== Config B: Under Load ==="
sudo insmod chronos.ko
stress-ng --cpu 4 --timeout 60s &
sleep 60
cat /proc/chronos > report/config_B.txt
sudo rmmod chronos

echo "=== Config C: Concurrent Access ==="
sudo insmod chronos.ko
for i in 1 2 3 4 5; do cat /proc/chronos & done
wait
cat /proc/chronos > report/config_C.txt
sudo rmmod chronos
