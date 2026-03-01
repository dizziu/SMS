# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11

# Target executable
TARGET = sms

# Source files
SRC = main.c student.c auth.c utils.c

# Object files (auto generated)
OBJ = $(SRC:.c=.o)

# Default rule
all: $(TARGET)

# Link object files
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Compile .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Remove compiled files
clean:
	rm -f $(OBJ) $(TARGET)

# Rebuild everything
rebuild: clean all

# Run program
run: $(TARGET)
	./$(TARGET)
