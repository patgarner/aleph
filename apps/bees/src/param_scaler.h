/* param_scaler.h
   bees

   parameter-scaling module.

   a scaler converts io_t to full-scale 4-byte parameter value  (native DSP unit.) 
   sometimes this can be done by calculation, more often i think by tuned tabled lookup.

   each scaler type must maintain a lookup table size.
   will try and keep these flexible enough that tuning can be performed offline on table data,
   and hopefully from the aleph UI at some point.

   in addition to a DSP use value, 
   scaler must provide human-readable value as well (in 16.16 i guess)
   so, two tables per initialized parameter type.
   
*/

#ifndef _ALEPH_AVR32_BEES_SCALER_H_
#define _ALEPH_AVR32_BEES_SCALER_H_


#include "op_math.h"
#include "param_common.h"


//-- function types
// get DSP value for given type, input
typedef s32 (*scaler_get_value_fn)(io_t in);
// print human-readable value to string
typedef void (*scaler_get_str_fn)(char* dst, io_t in);
// perform a tuning routine
typedef s32 (*scaler_tune_fn)(u8 tuneId, io_t in);
// search for closest input to DSP value
// - this will tend to be slow, use sparingly in realtime contexts
typedef io_t(*scaler_get_input_fn)(s32 value);

// class structure i
typedef struct _paramScaler { 
   // get value
  scaler_get_value_fn get_val;
  // get ui representation
  scaler_get_str_fn get_str;
  // type
  ParamType type;
  //// TODO, perhaps
  // array of tuning functions
  //  scaler_tune_fn * tune;
  // num tuning functions
  //  u8 numTune;
} ParamScaler;

// initialize scaler for given param (protected)
typedef void (*scaler_init_fn)(ParamScaler* sc, const ParamDesc* desc);


// initialize scaler for given param
extern void scaler_init(ParamScaler* sc, const ParamDesc* desc);

// get DSP value 
extern s32 scaler_get_value(ParamScaler* sc, io_t in);
// print readable value to string bffer
extern void scaler_get_str(char* dst, ParamScaler* sc, io_t in);

#endif