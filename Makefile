module_name = test

CC = gcc

OBJS = main.o \
	dsp_block.o

EXECUTABLE = $(module_name)_jk

CFLAGS = -ljack -D ARCH_LINUX=1

%.o: %.c
	$(CC) -ggdb $(CFLAGS) -c $< -o $@

all: $(OBJS)
	touch $(EXECUTABLE)
	rm ./$(EXECUTABLE)
	$(CC) $(OBJS) $(CFLAGS) -o $(EXECUTABLE) -g

clean:
	rm $(OBJS) $(EXECUTABLE)