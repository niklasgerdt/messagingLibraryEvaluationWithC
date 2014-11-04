bin/zmqpubsub tcp://*:6001 tcp://localhost:5001 tcp://localhost:5002 tcp://localhost:5003 tcp://localhost:5004 &
bin/zmqsub tcp://localhost:6001 A &
bin/zmqsub tcp://localhost:6001 B &
bin/zmqsub tcp://localhost:6001 N &
bin/zmqsub tcp://localhost:6001 AB &
bin/zmqpub 1000000 tcp://*:5001 A 100 &
bin/zmqpub 1000000 tcp://*:5002 B 100 &
bin/zmqpub 1000000 tcp://*:5003 C 100 &
bin/zmqpub 1000000 tcp://*:5004 D 100 &
