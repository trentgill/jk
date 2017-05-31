module_name = test



sim_sourcefiles = main.c \
	dsp_block.c

sim_outfile = $(module_name)_jk

sim_flags = -ljack -llo -D ARCH_LINUX=1

all:
	touch $(sim_outfile)
	rm ./$(sim_outfile)
	gcc $(sim_sourcefiles) $(sim_flags) -o $(sim_outfile) -g
