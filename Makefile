CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lm
SRC_DIR = .
MATRIX_DIR = matrix

SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(MATRIX_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

TARGET = parser_csv

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
