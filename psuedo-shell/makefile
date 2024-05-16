CFLAGS = -W -Wall
CC= gcc
OBJS= main.o string_parser.o command.o
TARGET= pseudo-shell


$(TARGET): main.o string_parser.o command.o
	$(CC) $(CFLAGS) -o $@ $(OBJS) 


main.o:main.c string_parser.o command.o 
	$(CC) $(CFLAGS) -g -c main.c


string_parser.o: string_parser.c string_parser.h
	$(CC) $(CFLAGS) -g -c string_parser.c


command.o: command.c command.h  
	$(CC) $(CFLAGS) -g -c command.c

clean:
	rm -f $(OBJS) $(TARGET)




