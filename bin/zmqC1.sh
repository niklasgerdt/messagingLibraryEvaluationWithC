PAUSE=$1
DIR=bin/build

$DIR/zmqpub $PAUSE tcp://168.1.1.1:5001 A 100 &
$DIR/zmqpub $PAUSE tcp://168.1.1.1:5002 B 100 &
$DIR/zmqpub $PAUSE tcp://168.1.1.1:5003 C 100 &
$DIR/zmqpub $PAUSE tcp://168.1.1.1:5004 D 100 &
$DIR/zmqsub tcp://168.1.1.2:6001 N1 &
$DIR/zmqsub tcp://168.1.1.2:6001 N2 &
$DIR/zmqsub tcp://168.1.1.2:6001 N3 &
$DIR/zmqsub tcp://168.1.1.2:6001 N4 &

read sig;
killall zmqsub zmqpub

