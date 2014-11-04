PAUSE=$1
BASE=bin/build

$BASE/dummypub $PAUSE tcp://0.0.0.0:0000 A 100 &
$BASE/dummypub $PAUSE tcp://0.0.0.0:0000 B 100 &
$BASE/dummypub $PAUSE tcp://0.0.0.0:0000 C 100 &
$BASE/dummypub $PAUSE tcp://0.0.0.0:0000 D 100 &

read sig;
killall dummypub
