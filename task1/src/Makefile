CC=gcc
CFLAGS= -Werror -Wall -Wextra -pedantic-errors
CSSEFLAGS= -msse3 -D USE_SSE
TARGET=fir
SRC= filter.c test.c main.c

without_sse:
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS)

sse:
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(CSSEFLAGS)

clean:
	$(RM) *.o $(TARGET)
