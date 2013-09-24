// std
#include <string.h>
// asf
#include "delay.h"
#include "print_funcs.h"
// aleph-avr32
#include "app.h"
#include "bfin.h"
#include "control.h"
#include "interrupts.h"
// common
#include "fix.h"
#include "param_common.h"
// app
#include "app_timers.h"
#include "ctl.h"
#include "inputs.h"
#include "render.h"
#include "util.h"

// blah
#define DSP_STRING "aleph-drumsyn"

// parameter input values
static s32 inVal[DSYN_NVOICES][PARAM_VOICE_NPARAMS];
// current parameter selection for each voice
static s32 sel[DSYN_NVOICES];

//----------------------------------------
//---- static variables

static const int kParamVoiceOff[4] = { 0, 
				   PARAM_VOICE_NPARAMS,
				   PARAM_VOICE_NPARAMS_x2,
				   PARAM_VOICE_NPARAMS_x3 };

//-------------------------------------
//------ extern functions

// get parameter report from DSP
u8 ctl_report_params(void) {
  volatile char buf[64];
  volatile ParamDesc pdesc;
  u32 numParams;
  u8 i;
 
  bfin_get_num_params(&numParams);
  print_dbg("\r\nnumparams: ");
  print_dbg_ulong(numParams);

  if(numParams == 255) {
    print_dbg("\r\n report_params fail (too many)");
    return 0;
  }

  if(numParams > 0) {
    for(i=0; i<numParams; i++) {
      bfin_get_param_desc(i, &pdesc);
      
      print_dbg("\r\n got pdesc : ");
      print_dbg((const char* )pdesc.label);
    }
  } else {
    print_dbg("\r\n report_params fail (none)");
    return 0;
  }
  
  delay_ms(100);

  print_dbg("\r\n checking module label ");
  // check module label
  bfin_get_module_name(buf);

  delay_ms(10);

  print_dbg("\r\n bfin module name: ");
  print_dbg((const char*)buf);
  if(strcmp((const char*)buf, DSP_STRING)) {
    print_dbg( "\r\n report_params fail (module name mismatch)" );
    return 0;
  } else {
    return 1;
  }
}

// set initial parameters
void ctl_init_params(void) {
  u8 i, j;
  /* FIXME:
     make a text parser for intial parameters and read from sdcard?
     use stored input values from UI?
     store in flash?
  */

   // FIXME: better initial input values
   for (i=0; i<DSYN_NVOICES; i++) {
     for(j=0; j<PARAM_VOICE_NPARAMS; j++) {
       inVal[i][j] = 0x1fffffff;
     }
   }

  ///////
  ///// drum 0

    ctl_voice_param( 0,  eParamGate,	0	);
    ctl_voice_param( 0,  eParamTrig,	1	);

    ctl_voice_param( 0,  eParamAmp,		FR32_MAX >> 1	);
    ctl_voice_param( 0,  eParamAmpSus,     	FR32_MAX >> 4	);
    ctl_voice_param( 0,  eParamAmpAtkSlew, 	sec_to_slew(1.f / 32.f)	);
    ctl_voice_param( 0,  eParamAmpDecSlew,	sec_to_slew(0.04 )	);
    ctl_voice_param( 0,  eParamAmpRelSlew,	sec_to_slew(0.0625)	);
    ctl_voice_param( 0,  eParamAmpSusDur,     	12 );

    ctl_voice_param( 0,  eParamFreqAtkSlew,	sec_to_slew(1.f / 16.f)	);
    ctl_voice_param( 0,  eParamFreqDecSlew,	sec_to_slew(0.125)	);
    ctl_voice_param( 0,  eParamFreqRelSlew,	sec_to_slew(0.125)	);
    ctl_voice_param( 0,  eParamFreqOff,	hz_to_svf(220.5)	);
    ctl_voice_param( 0,  eParamFreqOn,	hz_to_svf(880.5) );
    ctl_voice_param( 0,  eParamFreqSus,	hz_to_svf(440.5)	 );
    ctl_voice_param( 0,  eParamFreqSusDur,    	0.2);

    ctl_voice_param( 0,  eParamRqAtkSlew,	sec_to_slew(0.001)	);
    ctl_voice_param( 0,  eParamRqDecSlew,	sec_to_slew(0.0625)	);
    ctl_voice_param( 0,  eParamRqRelSlew,	sec_to_slew(1.f / 32.f)	);

    ctl_voice_param( 0,  eParamRqOff,	float_to_fr32(0.12)	);
    ctl_voice_param( 0,  eParamRqOn,	float_to_fr32(0.10)	);
    ctl_voice_param( 0,  eParamRqSus,	float_to_fr32(0.24)	);
    ctl_voice_param( 0,  eParamRqSusDur,     	30	);

    ctl_voice_param( 0,  eParamLow,		float_to_fr32(0.9)	);
    ctl_voice_param( 0,  eParamHigh,	0	);
    ctl_voice_param( 0,  eParamBand,	0	);
    ctl_voice_param( 0,  eParamNotch,	0	);


    //// drum 1

    ctl_voice_param( 1,  eParamGate,	0	);
    ctl_voice_param( 1,  eParamTrig,	1	);

    ctl_voice_param( 1,  eParamAmp,		FR32_MAX >> 1	);
    ctl_voice_param( 1,  eParamAmpSus,     	FR32_MAX >> 1	);
    ctl_voice_param( 1,  eParamAmpAtkSlew, 	sec_to_slew(1.f / 32.f)	);
    ctl_voice_param( 1,  eParamAmpDecSlew,	sec_to_slew(0.04 )	);
    ctl_voice_param( 1,  eParamAmpRelSlew,	sec_to_slew(0.2)	);
    ctl_voice_param( 1,  eParamAmpSusDur,     	120 );

    ctl_voice_param( 1,  eParamFreqAtkSlew,	sec_to_slew(1.f / 16.f)	);
    ctl_voice_param( 1,  eParamFreqDecSlew,	sec_to_slew(0.125)	);
    ctl_voice_param( 1,  eParamFreqRelSlew,	sec_to_slew(0.125)	);
    ctl_voice_param( 1,  eParamFreqOff,	hz_to_svf(27.5)	);
    ctl_voice_param( 1,  eParamFreqOn,	hz_to_svf(440) );
    ctl_voice_param( 1,  eParamFreqSus,	hz_to_svf(55.5)	 );
    ctl_voice_param( 1,  eParamFreqSusDur,    	0.2);

    ctl_voice_param( 1,  eParamRqAtkSlew,	sec_to_slew(0.001)	);
    ctl_voice_param( 1,  eParamRqDecSlew,	sec_to_slew(0.0625)	);
    ctl_voice_param( 1,  eParamRqRelSlew,	sec_to_slew(1.f / 32.f)	);

    ctl_voice_param( 1,  eParamRqOff,	float_to_fr32(0.2)	);
    ctl_voice_param( 1,  eParamRqOn,	float_to_fr32(0.3)	);
    ctl_voice_param( 1,  eParamRqSus,	float_to_fr32(0.24)	);
    ctl_voice_param( 1,  eParamRqSusDur,     	30	);

    ctl_voice_param( 1,  eParamLow,		float_to_fr32(0.9)	);
    ctl_voice_param( 1,  eParamHigh,	0	);
    ctl_voice_param( 1,  eParamBand,	float_to_fr32(0.4)	);
    ctl_voice_param( 1,  eParamNotch,	0	);
    ctl_voice_param( 1,  eParamSvfPre,	0	);
    ctl_voice_param( 1,  eParamFreqEnv,	1	);
    ctl_voice_param( 1,  eParamRqEnv,	1	);


    // drum 2

    ctl_voice_param( 2,  eParamGate,	0	);
    ctl_voice_param( 2,  eParamTrig,	1	);

    ctl_voice_param( 2,  eParamAmp,		FR32_MAX >> 1	);
    ctl_voice_param( 2,  eParamAmpSus,     	FR32_MAX >> 1	);
    ctl_voice_param( 2,  eParamAmpAtkSlew, 	sec_to_slew(1.f / 32.f)	);
    ctl_voice_param( 2,  eParamAmpDecSlew,	sec_to_slew(0.04 )	);
    ctl_voice_param( 2,  eParamAmpRelSlew,	sec_to_slew(2.0)	);
    ctl_voice_param( 2,  eParamAmpSusDur,     	1000 );

    ctl_voice_param( 2,  eParamFreqAtkSlew,	sec_to_slew(1.f / 64.f)	);
    ctl_voice_param( 2,  eParamFreqDecSlew,	sec_to_slew(1.f / 32.f)	);
    ctl_voice_param( 2,  eParamFreqRelSlew,	sec_to_slew(1.0)	);
    ctl_voice_param( 2,  eParamFreqOff,	hz_to_svf(27.5)	);
    ctl_voice_param( 2,  eParamFreqOn,	hz_to_svf(35.0) );
    ctl_voice_param( 2,  eParamFreqSus,	hz_to_svf(27.5)	 );
    ctl_voice_param( 2,  eParamFreqSusDur,    	10000);

    ctl_voice_param( 2,  eParamRqAtkSlew,	sec_to_slew(0.001)	);
    ctl_voice_param( 2,  eParamRqDecSlew,	sec_to_slew(0.0625)	);
    ctl_voice_param( 2,  eParamRqRelSlew,	sec_to_slew(1.f / 32.f)	);

    ctl_voice_param( 2,  eParamRqOff,	float_to_fr32(0.04)	);
    ctl_voice_param( 2,  eParamRqOn,	float_to_fr32(0.2)	);
    ctl_voice_param( 2,  eParamRqSus,	float_to_fr32(0.02)	);
    ctl_voice_param( 2,  eParamRqSusDur,     	30000	);

    ctl_voice_param( 2,  eParamLow,		float_to_fr32(0.9)	);
    ctl_voice_param( 2,  eParamHigh,	0	);
    ctl_voice_param( 2,  eParamBand,	0	);
    ctl_voice_param( 2,  eParamNotch,	0	);


    ctl_voice_param( 2,  eParamGate,	0	);
    ctl_voice_param( 2,  eParamTrig,	1	);

    ctl_voice_param( 2,  eParamAmp,		FR32_MAX >> 1	);
    ctl_voice_param( 2,  eParamAmpSus,     	FR32_MAX >> 1	);
    ctl_voice_param( 2,  eParamAmpAtkSlew, 	sec_to_slew(1.f / 32.f)	);
    ctl_voice_param( 2,  eParamAmpDecSlew,	sec_to_slew(0.04 )	);
    ctl_voice_param( 2,  eParamAmpRelSlew,	sec_to_slew(2.0)	);
    ctl_voice_param( 2,  eParamAmpSusDur,     	1000 );

    ctl_voice_param( 2,  eParamFreqAtkSlew,	sec_to_slew(1.f / 64.f)	);
    ctl_voice_param( 2,  eParamFreqDecSlew,	sec_to_slew(0.2)	);
    ctl_voice_param( 2,  eParamFreqRelSlew,	sec_to_slew(1.0)	);
    ctl_voice_param( 2,  eParamFreqOff,	hz_to_svf(57.5)	);
    ctl_voice_param( 2,  eParamFreqOn,	hz_to_svf(440.0) );
    ctl_voice_param( 2,  eParamFreqSus,	hz_to_svf(220.5)	 );
    ctl_voice_param( 2,  eParamFreqSusDur,    	10000);

    ctl_voice_param( 2,  eParamRqAtkSlew,	sec_to_slew(0.001)	);
    ctl_voice_param( 2,  eParamRqDecSlew,	sec_to_slew(0.0625)	);
    ctl_voice_param( 2,  eParamRqRelSlew,	sec_to_slew(1.f / 32.f)	);

    ctl_voice_param( 2,  eParamRqOff,	float_to_fr32(0.04)	);
    ctl_voice_param( 2,  eParamRqOn,	float_to_fr32(0.1)	);
    ctl_voice_param( 2,  eParamRqSus,	float_to_fr32(0.02)	);
    ctl_voice_param( 2,  eParamRqSusDur,     	30000	);

    ctl_voice_param( 2,  eParamLow,		float_to_fr32(0.9)	);
    ctl_voice_param( 2,  eParamHigh,	0	);
    ctl_voice_param( 2,  eParamBand,	float_to_fr32(0.4)	);
    ctl_voice_param( 2,  eParamNotch,	0	);


    // drum 3

    ctl_voice_param( 3,  eParamGate,	0	);
    ctl_voice_param( 3,  eParamTrig,	1	);

    ctl_voice_param( 3,  eParamAmp,		FR32_MAX >> 1	);
    ctl_voice_param( 3,  eParamAmpSus,     	FR32_MAX >> 1	);
    ctl_voice_param( 3,  eParamAmpAtkSlew, 	sec_to_slew(1.f / 32.f)	);
    ctl_voice_param( 3,  eParamAmpDecSlew,	sec_to_slew(0.04 )	);
    ctl_voice_param( 3,  eParamAmpRelSlew,	sec_to_slew(2.0)	);
    ctl_voice_param( 3,  eParamAmpSusDur,     	1000 );

    ctl_voice_param( 3,  eParamFreqAtkSlew,	sec_to_slew(1.f / 64.f)	);
    ctl_voice_param( 3,  eParamFreqDecSlew,	sec_to_slew(0.2)	);
    ctl_voice_param( 3,  eParamFreqRelSlew,	sec_to_slew(1.0)	);
    ctl_voice_param( 3,  eParamFreqOff,	hz_to_svf(27.5)	);
    ctl_voice_param( 3,  eParamFreqOn,	hz_to_svf(35.0) );
    ctl_voice_param( 3,  eParamFreqSus,	hz_to_svf(27.5)	 );
    ctl_voice_param( 3,  eParamFreqSusDur,    	10000);

    ctl_voice_param( 3,  eParamRqAtkSlew,	sec_to_slew(0.001)	);
    ctl_voice_param( 3,  eParamRqDecSlew,	sec_to_slew(0.0625)	);
    ctl_voice_param( 3,  eParamRqRelSlew,	sec_to_slew(1.f / 32.f)	);

    ctl_voice_param( 3,  eParamRqOff,	float_to_fr32(0.04)	);
    ctl_voice_param( 3,  eParamRqOn,	float_to_fr32(0.2)	);
    ctl_voice_param( 3,  eParamRqSus,	float_to_fr32(0.02)	);
    ctl_voice_param( 3,  eParamRqSusDur,     	30000	);

    ctl_voice_param( 3,  eParamLow,		float_to_fr32(0.9)	);
    ctl_voice_param( 3,  eParamHigh,	0	);
    ctl_voice_param( 3,  eParamBand,	float_to_fr32(0.4)	);
    ctl_voice_param( 3,  eParamNotch,	0	);


    ctl_voice_param( 3,  eParamGate,	0	);
    ctl_voice_param( 3,  eParamTrig,	1	);

    ctl_voice_param( 3,  eParamAmp,		FR32_MAX >> 1	);
    ctl_voice_param( 3,  eParamAmpSus,     	FR32_MAX >> 1	);
    ctl_voice_param( 3,  eParamAmpAtkSlew, 	sec_to_slew(1.f / 32.f)	);
    ctl_voice_param( 3,  eParamAmpDecSlew,	sec_to_slew(0.04 )	);
    ctl_voice_param( 3,  eParamAmpRelSlew,	sec_to_slew(2.0)	);
    ctl_voice_param( 3,  eParamAmpSusDur,     	1000 );

    ctl_voice_param( 3,  eParamFreqAtkSlew,	sec_to_slew(1.f / 64.f)	);
    ctl_voice_param( 3,  eParamFreqDecSlew,	sec_to_slew(0.2)	);
    ctl_voice_param( 3,  eParamFreqRelSlew,	sec_to_slew(1.0)	);
    ctl_voice_param( 3,  eParamFreqOff,	hz_to_svf(27.5)	);
    ctl_voice_param( 3,  eParamFreqOn,	hz_to_svf(135.0) );
    ctl_voice_param( 3,  eParamFreqSus,	hz_to_svf(110.5)	 );
    ctl_voice_param( 3,  eParamFreqSusDur,    	10000);

    ctl_voice_param( 3,  eParamRqAtkSlew,	sec_to_slew(0.001)	);
    ctl_voice_param( 3,  eParamRqDecSlew,	sec_to_slew(0.0625)	);
    ctl_voice_param( 3,  eParamRqRelSlew,	sec_to_slew(1.f / 32.f)	);

    ctl_voice_param( 3,  eParamRqOff,	float_to_fr32(0.04)	);
    ctl_voice_param( 3,  eParamRqOn,	float_to_fr32(0.02)	);
    ctl_voice_param( 3,  eParamRqSus,	float_to_fr32(0.02)	);
    ctl_voice_param( 3,  eParamRqSusDur,     	30000	);

    ctl_voice_param( 3,  eParamLow,		float_to_fr32(0.9)	);
    ctl_voice_param( 3,  eParamHigh,	0	);
    ctl_voice_param( 3,  eParamBand,	0	);
    ctl_voice_param( 3,  eParamNotch,	0	);
    ctl_voice_param( 3,  eParamSvfPre,	0	);
    ctl_voice_param( 3,  eParamFreqEnv,	0	);
    ctl_voice_param( 3,  eParamRqEnv,	0	);
}

// set gate
void  ctl_set_gate(u8 ch, u8 val) {
  ctl_voice_param(ch, eParamGate, (fract32)val );
}

// increment tempo
extern void ctl_inc_tempo(s32 val) {
  static s32 ms = 1000;
  static const s32 metroMax = 32000;
  static const s32 metroMin = 50; // 16th @300bpm
  ms += val;
  if(ms < metroMin) { ms = metroMin; }
  if(ms > metroMax) { ms = metroMax; }
  timers_set_metro_ms((u32)ms);
  print_dbg("\r\n ms: ");
  print_dbg_ulong(ms);
}

// set voice and parameter
extern void ctl_voice_param(u8 vid, u32 pid, fract32 val) {
  ctl_param_change(pid + kParamVoiceOff[vid], val);
}

// increment selected parameter value for voice
extern void ctl_inc_param(u8 vid, s32 val) {
  u8 pid = sel[vid];
  s32 v = inVal[vid][pid] + val; 
  if(v < 0) { v = 0x7fffffff; }
  inVal[vid][pid] = v;
  ctl_voice_param(vid, pid, v);
  render_param(vid, pid, val);
}


// increment parameter selection
extern void ctl_inc_param_select(u8 vid, s32 inc) {
  s32 pid = sel[vid];
  pid += inc;
  if(pid < 0) { pid = 0; }
  if(pid > PARAM_VOICE_NPARAMS) { pid = 0; }
  //  ctl_voice_param(vid, pid, v);
  sel[vid] = pid;
  render_param(vid, pid, inVal[vid][pid]);
}

// get input value
const u32 ctl_get_inval (u8 vid, u32 pid) {
  return (const u32)inVal[vid][pid];
}
