CC = mpicc
CCFLAGS = -Wall

SRC = main.c
SRC0 = native_sol.c
OBJ = $(SRC:.c = .o)
OBJ0 = $(SRC0:.c = .o)

TARGET = build/task
TARGET0 = build/native_task

.PHONY: all clean

all: $(TARGET) $(TARGET0)

$(TARGET): $(OBJ)
	$(CC) $(CCFLAGS) $^ -o $@

$(TARGET0): $(OBJ0)
	$(CC) $(CCFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CCFLAGS) $^ -o $@

clean:
	rm -f $(TARGET) $(TARGET0)
