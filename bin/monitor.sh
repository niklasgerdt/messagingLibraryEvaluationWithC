sar -A 1 -o logs/SAR >/dev/null 2>&1 &
nicstat -i eth0 -U 1 > logs/NICSTAT &
