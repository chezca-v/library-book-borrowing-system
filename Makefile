CC = gcc
CFLAGS = -std=c99 -Wall -Wextra
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
TARGET = library

all: $(TARGET)

$(TARGET): $(OBJS)
<TAB>$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
<TAB>$(CC) $(CFLAGS) -c $< -o $@

clean:
<TAB>rm -f $(TARGET) $(OBJS)
