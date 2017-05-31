linux application
- no gui (for now)
- sends audio to jack for output
- compiles the contents of a 'dsp-block' into the main loop
- should work w wrLib directly
	> rename wrLib to wrDsp
	> wrLib should host systems & control based things (eg printf over usart w/ dma & buffer)

next
- command-line interaction
	> set parameters
	> change dsp graph (sets a control-flow input)
	> then:: change dsp graph w list of function pointers

