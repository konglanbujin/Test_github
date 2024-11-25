#include "RESP.h"
#include <stdio.h>
#include "PRT.h"


/* response to PC*/
void respToPC(uint8_t type, void *data) {
  // delay(1000);
  switch (type) {
    case RESP_WV:
      PRT2("WV", *((long *)data));
      break;
    case RESP_WO:
      PRT2("WO", *((long *)data));
      break;
    case RESP_WD:
      PRT2("WD", *((long *)data));
      break;
    case RESP_SW:
      PRT2("SW", *((long *)data));
      break;
    case RESP_LA:
      PRT2("LA", *((long *)data));
      break;
    case RESP_LS:
      PRT2("LS", *((long *)data));
      break;
    case RESP_SN:
      PRT2("SN", *((long *)data));
      break;
    case RESP_SV:
      PRT2("SV", *((long *)data));
      break;
    case RESP_DBG:
      PRT2("#", *((long *)data));
      break;
    case RESP_ERR:
      PRT2("ERR", *((long *)data));
      break;
    case RESP_MS:
      PRT2("MS", *((long *)data));
      break;
    case RESP_DS:
      PRT2("DS", *((long *)data));
      break;
    case RESP_CS:
      PRT2("CS", *((long *)data));
      break;
    default:
      PRT2("*", *((long *)data));
      break;
  }
}

void returnSN() {
  Serial.println(PACT_SN);
}
