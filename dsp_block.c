#include "dsp_block.h"
#include <stdlib.h>

#include "wrMath.h"
#include "wrFilter.h"

filter_lp1_t filt;

void module_init( void )
{
 //   lp1_init( &filt );
  //  lp1_set_coeff( &filt, 0.0001 );
}

void module_process_frame(float* in, float* out, uint16_t b_size)
{
#ifdef SINGLE_SAMPLE
    *out++ = 0.0;
#else
    for(uint16_t i=0; i< b_size; i++){
        float rando = ((float)rand()) / (float)0x3FFFFFFF - 1.0;
        *out++ = rando;
    }
    float tmp[b_size];
    mul_vf_f(out, 0.5, tmp, b_size);
    lp1_step_v( &filt, tmp, out, b_size );
#endif
}

