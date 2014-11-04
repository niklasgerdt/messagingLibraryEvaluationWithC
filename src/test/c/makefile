UNITY_ROOT=../unity/src
UNITY=$(UNITY_ROOT)/unity.c
SYMBOLS=-DTEST
CLEANUP = rm -f build/*
INC_DIRS=-Isrc -I$(UNITY_ROOT)

all: clean compile test

clean:
	$(CLEANUP)

compile:
	gcc $(INC_DIRS) $(SYMBOLS) $(UNITY) unityspike.c -o build/spike.o
	gcc filespike.c -o build/filespike.o

test:
	build/spike.o
	build/filespike.o


