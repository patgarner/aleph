#include <string.h>
#include "drumsyn.h"
#include "env_int.h"
#include "filter_1p.h"
#include "module.h"
#include "params.h" 

static void set_param_gate(drumsynVoice* vp, s32 val) {
  if(val > 0) { 
    filter_1p_fr32_set_slew( &(vp->lpAmp) , vp->ampSlewUp);
    filter_1p_fr32_in( &(vp->lpAmp), vp->amp);

    filter_1p_fr32_set_slew( &(vp->lpFreq) , vp->freqSlewUp);
    filter_1p_fr32_in( &(vp->lpFreq), vp->freqOn);

    filter_1p_fr32_set_slew( &(vp->lpRq) , vp->rqSlewUp);
    filter_1p_fr32_in( &(vp->lpRq), vp->rqOn);

  } else {
    filter_1p_fr32_set_slew( &(vp->lpAmp) , vp->ampSlewDown);
    filter_1p_fr32_in( &(vp->lpAmp), 0);

    filter_1p_fr32_set_slew( &(vp->lpFreq) , vp->freqSlewDown);
    filter_1p_fr32_in( &(vp->lpFreq), vp->freqOff);

    filter_1p_fr32_set_slew( &(vp->lpRq) , vp->rqSlewDown);
    filter_1p_fr32_in( &(vp->lpRq), vp->rqOff);
  }
}

// set parameter by value
void module_set_param(u32 idx, pval v) {
  drumsynVoice* vp; // 	tmp voice pointer

  switch(idx) {

  case eParamGate0 : // 1bit gate
    vp = voices[0];
    set_param_gate(vp, v.s);
    break;
    
  case eParamAmp0 : // fract32 amp
    vp = voices[0];
    vp->amp = v.fr;
    if(vp->lpAmp.y > v.fr) { vp->lpAmp.y = v.fr; }
    break;
    
  case eParamAmpAtkSlew0 : // fract32 raw 1pole coefficient
    voices[0]->ampSlewUp = v.s;
    break;
    
  case eParamAmpRelSlew0 :
    voices[0]->ampSlewDown = v.s;
    break;

    // freq env
  case eParamFreqAtkSlew0 :
    voices[0]->freqSlewUp = v.s;
    break;

  case eParamFreqRelSlew0 :
    voices[0]->freqSlewDown = v.s;
    break;

  case eParamFreqStart0 :
    // release target
    vp->freqOff = v.fr;

    break;
  case eParamFreqEnd0 :
    vp->freqOn = v.fr;
    break;

    // rq env
  case eParamRqAtkSlew0 :
    voices[0]->rqSlewUp = v.s;
    break;
  case eParamRqRelSlew0 :
    voices[0]->rqSlewDown = v.s;
    break;

  case eParamRqStart0 :
    vp->rqOff = v.fr;
    break;
  case eParamRqEnd0 :
    vp->rqOn = v.fr;
    break;

  case eParamLow0 :	       
    filter_svf_set_low( vp->svf, v.fr);
    break;
  case eParamHigh0 :	       
    filter_svf_set_high( vp->svf, v.fr);
    break;
  case eParamBand0 :	       
    filter_svf_set_band( vp->svf, v.fr);
    break;
  case eParamNotch0 :	       
    filter_svf_set_notch( vp->svf, v.fr);
    break;


    //// TODO
    /// .. other voices...
  default:
    break;
  }


}



// set parameter by value (fix16)
/*
void module_set_param(u32 idx, pval v) {
  drumsynVoice* vp; // 	tmp voice pointer
  u8 b; // 		tmp bit

  switch(idx) {
  case eParamGate0 :
    b = v.s > 0;
    vp = &(voices[0]);
    env_int_set_gate(voices[0]->envAmp, b);
    env_int_set_gate(voices[0]->envFreq, b);
    env_int_set_gate(voices[0]->envRq, b);
    break;
  case eParamAmp0 :
    voices[0]->amp = v.fr;
    break;
  case eParamAmpAtkSlew0 : // raw 1pole coefficient
    env_int_set_atk_slew(voices[0]->envAmp, v.fr);
    break;
  case eParamAmpRelSlew0 :
    
    //    env_int_set_rel_slew(voices[0]->envAmp, v.fr);
    break;
  case eParamAmpAtkCurve0 :
    // TODO
    break;
  case eParamAmpRelCurve0 :
    // TODO
    break;
    // freq env
  case eParamFreqAtkSlew0 :
    env_int_set_atk_slew(voices[0]->envFreq, v.fr);
    break;
  case eParamFreqRelSlew0 :
    env_int_set_rel_slew(voices[0]->envFreq, v.fr);
    break;
  case eParamFreqAtkCurve0 :
    // TODO
    break;
  case eParamFreqRelCurve0 :
    // TODO
    break;
  case eParamFreqStart0 :
    //    voices[0]->envStartFreq = v.fr;
    env_int_set_scale(voices[0]->envFreq, v.fr);
    break;
  case eParamFreqEnd0 :
    voices[0]->envEndFreq = v.fr;
    break;
    // rq env
  case eParamRqAtkSlew0 :
    env_int_set_atk_slew(voices[0]->envRq, v.fr);
    break;
  case eParamRqRelSlew0 :
    env_int_set_rel_slew (voices[0]->envRq, v.fr);
    break;
  case eParamRqAtkCurve0 :
    // TODO
    break;
  case eParamRqRelCurve0 :
    // TODO
    break;
  case eParamRqStart0 :
    //    voices[0]->envStartRq = v.fr;
    env_int_set_scale(voices[0]->envRq, v.fr);
    break;
  case eParamRqEnd0 :
    voices[0]->envEndRq = v.fr;
    break;

  case eParamLow0 :	       
    break;
  case eParamHigh0 :	       
    break;
  case eParamBand0 :	       
    break;
  case eParamNotch0 :	       
    break;


    //// TODO
    /// .. other voices...


  default:
    break;
  }
}



#endif // DRUMSYN_NOENV
*/

void fill_param_desc(void) {
  strcpy(gModuleData->paramDesc[eParamGate0].label, "Gate0");
  strcpy(gModuleData->paramDesc[eParamGate0].unit, "");
  gModuleData->paramDesc[eParamGate0].type = PARAM_TYPE_FRACT;
  gModuleData->paramDesc[eParamGate0].min = 0;
  gModuleData->paramDesc[eParamGate0].max = FR32_MAX;

  strcpy(gModuleData->paramDesc[eParamTrig0].label, "Trig0");
  strcpy(gModuleData->paramDesc[eParamTrig0].unit, "");
  gModuleData->paramDesc[eParamTrig0].type = PARAM_TYPE_FRACT;
  gModuleData->paramDesc[eParamTrig0].min = 0;
  gModuleData->paramDesc[eParamTrig0].max = FR32_MAX;

  strcpy(gModuleData->paramDesc[eParamAmp0].label, "Amp0");
  strcpy(gModuleData->paramDesc[eParamAmp0].unit, "");
  gModuleData->paramDesc[eParamAmp0].type = PARAM_TYPE_FRACT;
  gModuleData->paramDesc[eParamAmp0].min = 0;
  gModuleData->paramDesc[eParamAmp0].max = FR32_MAX;

  strcpy(gModuleData->paramDesc[eParamAmpAtkSlew0].label, "AmpAtkSlew0");
  strcpy(gModuleData->paramDesc[eParamAmpAtkSlew0].unit, "");
  gModuleData->paramDesc[eParamAmpAtkSlew0].type = PARAM_TYPE_FRACT;
  gModuleData->paramDesc[eParamAmpAtkSlew0].min = 0;
  gModuleData->paramDesc[eParamAmpAtkSlew0].max = FR32_MAX;

  strcpy(gModuleData->paramDesc[eParamAmpRelSlew0].label, "AmpRelSlew0");
  strcpy(gModuleData->paramDesc[eParamAmpRelSlew0].unit, "");
  gModuleData->paramDesc[eParamAmpRelSlew0].type = PARAM_TYPE_FRACT;
  gModuleData->paramDesc[eParamAmpRelSlew0].min = 0;
  gModuleData->paramDesc[eParamAmpRelSlew0].max = FR32_MAX;

  /* strcpy(gModuleData->paramDesc[eParamAmpAtkCurve0].label, "AmpAtkCurve0"); */
  /* strcpy(gModuleData->paramDesc[eParamAmpAtkCurve0].unit, ""); */
  /* gModuleData->paramDesc[eParamAmpAtkCurve0].type = PARAM_TYPE_FRACT; */
  /* gModuleData->paramDesc[eParamAmpAtkCurve0].min = 0; */
  /* gModuleData->paramDesc[eParamAmpAtkCurve0].max = FR32_MAX; */

  /* strcpy(gModuleData->paramDesc[eParamAmpRelCurve0].label, "AmpRelCurve0"); */
  /* strcpy(gModuleData->paramDesc[eParamAmpRelCurve0].unit, ""); */
  /* gModuleData->paramDesc[eParamAmpRelCurve0].type = PARAM_TYPE_FRACT; */
  /* gModuleData->paramDesc[eParamAmpRelCurve0].min = 0; */
  /* gModuleData->paramDesc[eParamAmpRelCurve0].max = FR32_MAX; */

  strcpy(gModuleData->paramDesc[eParamFreqAtkSlew0].label, "FreqAtkSlew0");
  strcpy(gModuleData->paramDesc[eParamFreqAtkSlew0].unit, "");
  gModuleData->paramDesc[eParamFreqAtkSlew0].type = PARAM_TYPE_FRACT;
  gModuleData->paramDesc[eParamFreqAtkSlew0].min = 0;
  gModuleData->paramDesc[eParamFreqAtkSlew0].max = FR32_MAX;

  strcpy(gModuleData->paramDesc[eParamFreqRelSlew0].label, "FreqRelSlew0");
  strcpy(gModuleData->paramDesc[eParamFreqRelSlew0].unit, "");
  gModuleData->paramDesc[eParamFreqRelSlew0].type = PARAM_TYPE_FRACT;
  gModuleData->paramDesc[eParamFreqRelSlew0].min = 0;
  gModuleData->paramDesc[eParamFreqRelSlew0].max = FR32_MAX;

  /* strcpy(gModuleData->paramDesc[eParamFreqAtkCurve0].label, "FreqAtkCurve0"); */
  /* strcpy(gModuleData->paramDesc[eParamFreqAtkCurve0].unit, ""); */
  /* gModuleData->paramDesc[eParamFreqAtkCurve0].type = PARAM_TYPE_FRACT; */
  /* gModuleData->paramDesc[eParamFreqAtkCurve0].min = 0; */
  /* gModuleData->paramDesc[eParamFreqAtkCurve0].max = FR32_MAX; */

  /* strcpy(gModuleData->paramDesc[eParamFreqRelCurve0].label, "FreqRelCurve0"); */
  /* strcpy(gModuleData->paramDesc[eParamFreqRelCurve0].unit, ""); */
  /* gModuleData->paramDesc[eParamFreqRelCurve0].type = PARAM_TYPE_FRACT; */
  /* gModuleData->paramDesc[eParamFreqRelCurve0].min = 0; */
  /* gModuleData->paramDesc[eParamFreqRelCurve0].max = FR32_MAX; */

  strcpy(gModuleData->paramDesc[eParamFreqStart0].label, "FreqStart0");
  strcpy(gModuleData->paramDesc[eParamFreqStart0].unit, "");
  gModuleData->paramDesc[eParamFreqStart0].type = PARAM_TYPE_FRACT;
  gModuleData->paramDesc[eParamFreqStart0].min = 0;
  gModuleData->paramDesc[eParamFreqStart0].max = FR32_MAX;

  strcpy(gModuleData->paramDesc[eParamFreqEnd0].label, "FreqEnd0");
  strcpy(gModuleData->paramDesc[eParamFreqEnd0].unit, "");
  gModuleData->paramDesc[eParamFreqEnd0].type = PARAM_TYPE_FRACT;
  gModuleData->paramDesc[eParamFreqEnd0].min = 0;
  gModuleData->paramDesc[eParamFreqEnd0].max = FR32_MAX;

  strcpy(gModuleData->paramDesc[eParamRqAtkSlew0].label, "RqAtkSlew0");
  strcpy(gModuleData->paramDesc[eParamRqAtkSlew0].unit, "");
  gModuleData->paramDesc[eParamRqAtkSlew0].type = PARAM_TYPE_FRACT;
  gModuleData->paramDesc[eParamRqAtkSlew0].min = 0;
  gModuleData->paramDesc[eParamRqAtkSlew0].max = FR32_MAX;

  strcpy(gModuleData->paramDesc[eParamRqRelSlew0].label, "RqRelSlew0");
  strcpy(gModuleData->paramDesc[eParamRqRelSlew0].unit, "");
  gModuleData->paramDesc[eParamRqRelSlew0].type = PARAM_TYPE_FRACT;
  gModuleData->paramDesc[eParamRqRelSlew0].min = 0;
  gModuleData->paramDesc[eParamRqRelSlew0].max = FR32_MAX;

  /* strcpy(gModuleData->paramDesc[eParamRqAtkCurve0].label, "RqAtkCurve0"); */
  /* strcpy(gModuleData->paramDesc[eParamRqAtkCurve0].unit, ""); */
  /* gModuleData->paramDesc[eParamRqAtkCurve0].type = PARAM_TYPE_FRACT; */
  /* gModuleData->paramDesc[eParamRqAtkCurve0].min = 0; */
  /* gModuleData->paramDesc[eParamRqAtkCurve0].max = FR32_MAX; */

  /* strcpy(gModuleData->paramDesc[eParamRqRelCurve0].label, "RqRelCurve0"); */
  /* strcpy(gModuleData->paramDesc[eParamRqRelCurve0].unit, ""); */
  /* gModuleData->paramDesc[eParamRqRelCurve0].type = PARAM_TYPE_FRACT; */
  /* gModuleData->paramDesc[eParamRqRelCurve0].min = 0; */
  /* gModuleData->paramDesc[eParamRqRelCurve0].max = FR32_MAX; */

  strcpy(gModuleData->paramDesc[eParamRqStart0].label, "RqStart0");
  strcpy(gModuleData->paramDesc[eParamRqStart0].unit, "");
  gModuleData->paramDesc[eParamRqStart0].type = PARAM_TYPE_FRACT;
  gModuleData->paramDesc[eParamRqStart0].min = 0;
  gModuleData->paramDesc[eParamRqStart0].max = FR32_MAX;

  strcpy(gModuleData->paramDesc[eParamRqEnd0].label, "RqEnd0");
  strcpy(gModuleData->paramDesc[eParamRqEnd0].unit, "");
  gModuleData->paramDesc[eParamRqEnd0].type = PARAM_TYPE_FRACT;
  gModuleData->paramDesc[eParamRqEnd0].min = 0;
  gModuleData->paramDesc[eParamRqEnd0].max = FR32_MAX;

  strcpy(gModuleData->paramDesc[eParamLow0].label, "Low0");
  strcpy(gModuleData->paramDesc[eParamLow0].unit, "");
  gModuleData->paramDesc[eParamLow0].type = PARAM_TYPE_FRACT;
  gModuleData->paramDesc[eParamLow0].min = 0;
  gModuleData->paramDesc[eParamLow0].max = FR32_MAX;

  strcpy(gModuleData->paramDesc[eParamHigh0].label, "High0");
  strcpy(gModuleData->paramDesc[eParamHigh0].unit, "");
  gModuleData->paramDesc[eParamHigh0].type = PARAM_TYPE_FRACT;
  gModuleData->paramDesc[eParamHigh0].min = 0;
  gModuleData->paramDesc[eParamHigh0].max = FR32_MAX;

  strcpy(gModuleData->paramDesc[eParamBand0].label, "Band0");
  strcpy(gModuleData->paramDesc[eParamBand0].unit, "");
  gModuleData->paramDesc[eParamBand0].type = PARAM_TYPE_FRACT;
  gModuleData->paramDesc[eParamBand0].min = 0;
  gModuleData->paramDesc[eParamBand0].max = FR32_MAX;

  strcpy(gModuleData->paramDesc[eParamNotch0].label, "Notch0");
  strcpy(gModuleData->paramDesc[eParamNotch0].unit, "");
  gModuleData->paramDesc[eParamNotch0].type = PARAM_TYPE_FRACT;
  gModuleData->paramDesc[eParamNotch0].min = 0;
  gModuleData->paramDesc[eParamNotch0].max = FR32_MAX;

  /// TODO: more voices
}
