LIBS += -lmodbus

modbusbridge: modbusbridge.o
	$(CC) $(LDFLAGS) modbusbridge.o -o modbusbridge $(LIBS)
modbusbridge.o: modbusbridge.c
	$(CC) $(CFLAGS) -c modbusbridge.c -o modbusbridge.o

# remove object files and executable when user executes "make clean"
clean:
	rm *.o modbusbridge
