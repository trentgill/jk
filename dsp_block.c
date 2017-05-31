#include "dsp_block.h"

#ifdef SINGLE_SAMPLE
float module_process_frame(float in)
{
	return in; // echo
}
#else
void module_process_frame(float* in, float* out, uint16_t b_size)
{
	for(uint16_t i=0; i< b_size; i++){
		*out++ = *in++; // echo
	}
}
#endif
