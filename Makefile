CC = gcc
CFLAGG = -Wall
TARGET = server
OBJS = handler.o logger.o main.o utils.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGG) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGG) -c $<

clean: 
	rm -f *.o $(TARGET)
