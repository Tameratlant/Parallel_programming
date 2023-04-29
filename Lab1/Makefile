CC = mpicc

SRC = main.c
SRC0 = native_sol.c
OBJ = $(SRC:.c = .o)
OBJ0 = $(SRC0:.c = .o)

TARGET = build/task
TARGET0 = build/native_task

.PHONY: all clean

all: $(TARGET) $(TARGET0)

$(TARGET): $(OBJ)
	$(CC) $^ -o $@

$(TARGET0): $(OBJ0)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $^ -o $@

clean:
	rm -f build/*