# Makefile for the smash program
CC = g++
CFLAGS = -g -Wall
CCLINK = $(CC)
OBJS = main.o atm.o banking.o account.o
RM = rm -f
# Creating the  executable
Bank: $(OBJS)
	$(CCLINK) $(CFLAGS) -o Bank $(OBJS)
# Creating the object files
main.o: main.cpp atm.h
	$(CC) $(CFLAGS) -c main.cpp
atm.o: atm.cpp atm.h
	$(CC) $(CFLAGS) -c atm.cpp
banking.o: banking.cpp banking.h
	$(CC) $(CFLAGS) -c banking.cpp
account.o: account.cpp account.h
	$(CC) $(CFLAGS) -c account.cpp
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

