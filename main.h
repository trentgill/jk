#pragma once

// uncomment to use single-sample processing
// otherwise we assume we're processing a full block natively
/*
#define SINGLE_SAMPLE
*/

// #ifndef SAMPLERATE
// #define SAMPLERATE    48000
// #endif





// #if ARCH_BFIN

// #define SDRAM_ADDRESS 0x00000000
// #define SDRAM_SIZE    0x07ffffff


// #else
// void *SDRAM_ADDRESS;
#define IN_PORTS 1
#define OUT_PORTS 1
// #define SDRAM_SIZE 0x07ffffff

// fract32 in[IN_PORTS];
// fract32 out[OUT_PORTS];

#ifdef SINGLE_SAMPLE
	extern float module_process_frame(float in);
#else
	extern void module_process_frame(float* in, float* out, uint16_t b_size);
#endif
