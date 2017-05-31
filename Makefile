module_name = test



OBJS = main.o \
	dsp_block.o

EXECUTABLE = $(module_name)_jk

sim_flags = -ljack -llo -D ARCH_LINUX=1

all:
	touch $(EXECUTABLE)
	rm ./$(EXECUTABLE)
	gcc $(OBJS) $(sim_flags) -o $(EXECUTABLE) -g

clean:
	rm $(OBJS) $(EXECUTABLE)