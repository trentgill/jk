#pragma once

// uncomment to use single-sample processing
// otherwise we assume we're processing a full block natively
#define SINGLE_SAMPLE

#define IN_PORTS 1
#define OUT_PORTS 1

extern void module_init( void );

extern void module_process_frame(float* in, float* out, uint16_t b_size);
