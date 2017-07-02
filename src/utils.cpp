#include "utils.h"
#include <math.h>

float p2alt(float p){
  return (1.0-(pow(((double)p/101350.0),0.190284)))*145366.45;
}
