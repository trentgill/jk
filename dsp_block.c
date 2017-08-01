#include "dsp_block.h"
#include <stdlib.h>

#include "wrMath.h"

#ifdef SINGLE_SAMPLE
float module_process_frame(float in)
{
    return ((float)rand()) / (float)0x3FFFFFFF - 1.0;
}
#else
void module_process_frame(float* in, float* out, uint16_t b_size)
{
	for(uint16_t i=0; i< b_size; i++){
        float rando = ((float)rand()) / (float)0x3FFFFFFF - 1.0;
        *out++ = rando;
	}
    mul_vf_f(out, 0.5, out, b_size);
}
#endif

