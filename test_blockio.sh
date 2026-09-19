#!/bin/bash
echo "=================================================="
echo " Chronos OS - Block I/O Thread-Safety Test"
echo "=================================================="

echo ""
echo "[1] Reloading module..."
sudo rmmod chronos 2>/dev/null
sudo insmod chronos.ko
sleep 1

echo ""
echo "[2] Initial stats:"
cat /proc/chronos

echo ""
echo "[3] Sending 1 single Read request..."
echo "R" | sudo tee /proc/chronos > /dev/null
sleep 1
echo "Stats after 1 request:"
cat /proc/chronos

echo ""
echo "[4] Sending 10 concurrent Write requests (waiting for completion)..."
for i in $(seq 1 10); do
    echo "W" | sudo tee -a /proc/chronos > /dev/null &
done
wait
sleep 1
echo "Stats after 10 concurrent requests (queue fully drained):"
cat /proc/chronos

echo ""
echo "[5] Sending 20 concurrent requests WITHOUT waiting (to see live queue):"
for i in $(seq 1 20); do
    echo "W" | sudo tee -a /proc/chronos > /dev/null &
done
echo "Stats immediately after submission (queue should be > 0 here):"
cat /proc/chronos
wait
sleep 2
echo "Stats after queue fully drains:"
cat /proc/chronos

echo ""
echo "=================================================="
echo " Thread-safe Block I/O Queue Manager - Test Complete"
echo "=================================================="
