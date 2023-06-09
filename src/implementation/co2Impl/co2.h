#pragma once

#include <stdint.h>
#include "mh_z19.h"
#include "serial.h"

void co2impl_init();
mh_z19_returnCode_t co2impl_measure();
uint16_t co2impl_getMeasurement();
