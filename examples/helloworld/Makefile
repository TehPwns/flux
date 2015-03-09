CC=g++
CFLAGS=-c -Wall -std=c++11 -I../../
TARGET=helloflux

#--------#

objects = flux.o main.o

all: $(TARGET)

$(TARGET): $(objects)
	$(CC) $(objects) -o $(TARGET)

flux.o: ../../flux.cpp
	$(CC) $(CFLAGS) ../../flux.cpp
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

.PHONY : clean
clean :
	rm $(TARGET) $(objects)
