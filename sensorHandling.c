/*
 * File:   sensorHandling.c
 * Author: M67732
 *
 * Created on November 30, 2022, 3:13 PM
 */


#include <xc.h>
#include "sensorHandling.h"
#include "mcc_generated_files/adc/adcc.h"

uint16_t  readAmbientClick(void) {
    return ADCC_GetSingleConversion(adcIn);
}
