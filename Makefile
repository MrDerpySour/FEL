
# Variable declaration
CC=g++
CFLAGS=-c -O3 -std=c++17

# Targets
all: fel

fel: byte_code.o context.o flag_list.o helper.o interpreter.o manager.o var_commands.o var_manager.o var_memory.o
	ar vr src/lib/libFEL.a byte_code.o context.o flag_list.o helper.o interpreter.o manager.o var_commands.o var_manager.o var_memory.o

clean:
	rm -rf *.o

byte_code.o: src/byte_code.cpp
	$(CC) $(CFLAGS) src/byte_code.cpp

context.o: src/context.cpp
	$(CC) $(CFLAGS) src/context.cpp

flag_list.o: src/flag_list.cpp
	$(CC) $(CFLAGS) src/flag_list.cpp

helper.o: src/helper.cpp
	$(CC) $(CFLAGS) -O3 src/helper.cpp

interpreter.o: src/interpreter.cpp
	$(CC) $(CFLAGS) src/interpreter.cpp

manager.o: src/manager.cpp
	$(CC) $(CFLAGS) src/manager.cpp

var_commands.o: src/var_commands.cpp
	$(CC) $(CFLAGS) src/var_commands.cpp

var_manager.o: src/var_manager.cpp
	$(CC) $(CFLAGS) src/var_manager.cpp

var_memory.o: src/var_memory.cpp
	$(CC) $(CFLAGS) src/var_memory.cpp
