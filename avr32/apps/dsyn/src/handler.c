/*
  ui.c
  lppr
  aleph-avr32

 */

//asf
#include "print_funcs.h"

// avr32
#include "control.h"
#include "event_types.h"
#include "interrupts.h"
#include "screen.h"
// dsyn
#include "app_event_types.h"
#include "app_timers.h"
#include "ctl.h"
#include "grid.h"
#include "handler.h"
#include "monome.h"
#include "render.h"
#include "sequence.h"

// up and down interval timers for keys and footswitches
// static u32 swTicks[8][2];

//---------------------------------------------
//--------- static funcs

// process timing on key press and return interval
/* static u32 sw_time(u8 num, u8 val) { */
/*   u32 ret; */
/*   if( swTicks[num][val] > tcTicks) { */
/*     // overflow */
/*     ret = tcTicks + (0xffffffff - swTicks[num][val] ); */
/*     print_dbg("\r\n overflow in sw timer"); */
/*   } else { */
/*     ret = tcTicks - swTicks[num][val]; */
/*     print_dbg("\r\n sw_time: ");  */
/*     print_dbg_ulong(ret); */
/*   } */
/*   swTicks[num][val] = tcTicks; */
/*   return ret; */
/* } */

// return param increment given encoder ticks (knob acceleration)
static fix16 scale_knob_value(const s32 v) {
  s32 vabs = BIT_ABS(v);
  //  print_dbg("\r\n knob acc, val: ");
  //  print_dbg_hex((u32)v);
  if(vabs < 4) {
    //    print_dbg("\r\n ");
    return v;
  } else if (vabs < 8) {
    //    print_dbg("\r\n knob acc 1");
    return v << 2;
  } else if (vabs < 12) {
    //    print_dbg("\r\n knob acc 2");
    return v << 4;
  } else if (vabs < 19) {
    //    print_dbg("\r\n knob acc 3");
    return v << 5;
  } else if (vabs < 25) {
    //    print_dbg("\r\n knob acc 4");
    return v << 7;  } 
  else if (vabs < 32) {
    //    print_dbg("\r\n knob acc 4");
    return v << 9;
  } else {
    //    print_dbg("\r\n knob acc max");
    return v << 12;
  }
}

static void handle_monome_connect(u32 data) {
  eMonomeDevice dev;
  u8 w;
  u8 h;
  monome_connect_parse_event_data(data, &dev, &w, &h);
  if(dev != eDeviceGrid) {
    print_dbg("\r\n DSYN monome connect: unsupported device");
    return;
  }
  print_dbg("\r\n DSYN: connecting grid device");
  grid_set_size(w, h);
  timers_set_monome();
}

//---------------------------------------
//---- external funcs

// handle key presses
extern void dsyn_handler(event_t* ev) {
  u8 b;

  //  print_dbg("\r\n app event handler: ");
  //  print_dbg_hex(ev->eventType);

  switch (ev->eventType) {

  case kEventSeqNext:
    seq_advance();
    break;
    
  case kEventSwitch0:
    b = ev->eventData > 0;
    ctl_set_gate(0, b);
    render_sw_on(0, b);
    seq_tog_stage(0, seq_get_pos());
    break;

  case kEventSwitch1:
    b = ev->eventData > 0;
    ctl_set_gate(1, b);
    render_sw_on(1, b);
    seq_tog_stage(1, seq_get_pos());
    break;
  case kEventSwitch2:
    b = ev->eventData > 0;
    ctl_set_gate(2, b);
    render_sw_on(2, b);
    seq_tog_stage(2, seq_get_pos());
    break;
  case kEventSwitch3:
    b = ev->eventData > 0;
    ctl_set_gate(3, b);
    render_sw_on(3, b);
    seq_tog_stage(3, seq_get_pos());
    break;
    
  case kEventSwitch6:
    render_sw_on(2, ev->eventData > 0);
    break;
    
  case kEventSwitch7:
    render_sw_on(3, ev->eventData > 0);
    break;

  case kEventMonomeGridKey:
    //    (*monome_grid_key_handler)((void*)monomeOpFocus, (u32)e->eventData);
    grid_handle_key_event(ev->eventData);
    break;
    
  case kEventEncoder0:
    /// TEMPO
    ctl_inc_tempo(scale_knob_value(ev->eventData));
    break;
  case kEventEncoder1:
    // SCROLL GRID
    grid_inc_scroll( ev->eventData > 0 ? 1 : -1);
    break;
  case kEventEncoder2:
    // PARAM IDX 
    // .. .TODO
    break;
  case kEventEncoder3:
    // PARAM VALUE
    // ... TODO
    break;

  case kEventMonomeConnect :
    handle_monome_connect((u32)ev->eventData);
    break;
  default:
    break;
  }
}
