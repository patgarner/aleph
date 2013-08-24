#ifndef _ALEPH_LINES_PARAMS_H_
#define _ALEPH_LINES_PARAMS_H_

#define PARAM_RQ_MIN 0x00000000
#define PARAM_RQ_MAX 0x00400000


enum params {

  eParam_delay0,
  eParam_loop0,
  eParam_rate0,
  eParam_write0,
  eParam_pre0,
  eParam_pos_write0,
  eParam_pos_read0,

  eParam_delay1,
  eParam_loop1,
  eParam_rate1,
  eParam_write1,
  eParam_pre1,
  eParam_pos_write1,
  eParam_pos_read1,

  eParam_coeff0,
  eParam_rq0,
  eParam_low0,
  eParam_high0,
  eParam_band0,
  eParam_notch0,

  eParam_coeff1,
  eParam_rq1,
  eParam_low1,
  eParam_high1,
  eParam_band1,
  eParam_notch1,

  eParam_adc0_del0,
  eParam_adc0_del1,
  eParam_adc1_del0,
  eParam_adc1_del1,
  eParam_adc2_del0,
  eParam_adc2_del1,
  eParam_adc3_del0,
  eParam_adc3_del1,

  eParam_del0_del0,
  eParam_del0_del1,
  eParam_del1_del0,
  eParam_del1_del1,

  eParam_del0_dac0,
  eParam_del0_dac1,
  eParam_del0_dac2,
  eParam_del0_dac3,
  eParam_del1_dac0,
  eParam_del1_dac1,
  eParam_del1_dac2,
  eParam_del1_dac3,

  eParam_adc0_dac0,
  eParam_adc0_dac1,
  eParam_adc0_dac2,
  eParam_adc0_dac3,
  eParam_adc1_dac0,
  eParam_adc1_dac1,
  eParam_adc1_dac2,
  eParam_adc1_dac3,
  eParam_adc2_dac0,
  eParam_adc2_dac1,
  eParam_adc2_dac2,
  eParam_adc2_dac3,
  eParam_adc3_dac0,
  eParam_adc3_dac1,
  eParam_adc3_dac2,
  eParam_adc3_dac3,

  eParamNumParams
};  

extern void fill_param_desc(void);

#endif // header guard 
// EOF
