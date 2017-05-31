#pragma once

#include <stdio.h>
#include <stdlib.h>
// #include <main.h>

#ifdef SINGLE_SAMPLE
	float module_process_frame(float in);
#else
	void module_process_frame(float* in, float* out, uint16_t b_size);
#endif
