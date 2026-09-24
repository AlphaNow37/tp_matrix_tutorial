# All main files to compile
TARGET=main_matrix main_matrix_static test_matrix

# Main target: compile everything in $(TARGET)
all: $(TARGET)

# Compiler used
CC=gcc

# Compiler and linker flags
CFLAGS=-Wall -Wextra -pedantic -std=c11 -fPIC $(DEPFLAGS)
LDFLAGS=-L.
LDLIBS=-lmatrix

# Adapt CFLAGS depending on DEBUG Make variable
ifndef DEBUG
  CFLAGS += -O2
else
  CFLAGS += -Og -g
endif

# Dynamic library production
libmatrix.so: matrix.o
	$(CC) -shared -fPIC -o $@ $^

# Static library production
libmatrix.a: matrix.o
	ar r $@ $^

# Rule to produce main_matrix_static (as a static executable)
main_matrix_static: main_matrix.o libmatrix.a
	$(CC) $(LDFLAGS) -static -o $@ $< $(LDLIBS)

# Rule to produce test_matrix (as a static executable)
test_matrix: test_matrix.o libmatrix.a
	$(CC) $(LDFLAGS) -static -o $@ $< $(LDLIBS)

# main_matrix depends on libmatrix.so
main_matrix: main_matrix.o libmatrix.so
	$(CC) $(LDFLAGS) -o $@ $< $(LDLIBS) -Wl,-rpath,.

# Remove all generated files except targets
clean:
	-rm -f *.o *.gch *.d *.a *.so

# Remove targets as well as other generated files
fclean: clean
	-rm $(TARGET)

# Run unit tests
test: test_matrix
	./test_matrix

# clean, fclean and test have rules but do not produce real files
.PHONY: clean fclean test

# Automatic generation of dependencies .d files
DEPFLAGS=-MMD -MP -MF $*.d
SRCS:=$(wildcard *.c)
DEPFILES:=$(SRCS:%.c=%.d)
include $(wildcard $(DEPFILES))
