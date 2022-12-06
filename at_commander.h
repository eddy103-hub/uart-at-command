/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PARSER_H
#define	PARSER_H


#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc_generated_files/uart/uart3.h"

#define ATCMD_PRINT_BUFFER_SIZE       64
#define ATCMD_RECEIVE_BUFFER_SIZE     64

#define COM_IsTxReady           UART3_IsTxReady 
#define COM_Write               UART3_Write
#define COM_IsRxReady           UART3_IsRxReady
#define COM_Read                UART3_Read

void ATCMD_Print(const char *format, ...);
void ATCMD_Task(void);
uint8_t ATCMD_ReadLine(void);
int ATCMD_strcon(char a[], char b[]);

#define ID_SSID                 1
#define ID_SEC_TYPE             2
#define ID_CREDENTIALS          3
#define ID_CHANNEL              4
#define ID_NTP_SVR              12
#define ID_NTP_STATIC           13

#define PAR_SEC_TYPE_WPA2       3
#define PAR_ANY_CHANNEL         255
#define PAR_NTP_STATIC          1
#define PAR_USE_CONFIGURATION   1

#define ID_MQTT_BROKER_ADDR     1
#define ID_MQTT_BROKER_PORT     2
#define ID_MQTT_CLIENT_ID       3
#define ID_MQTT_USERNAME        4
#define ID_MQTT_TLS_CONF_IDX    7

#define MQTT_BROKER_PORT        8883
#define MQTT_START_CONNECTION   1

#define MQTT_RCLEAN             1
#define MQTT_PUB_TOPIC        "test/Topic"
#define MQTT_PAYLOAD          "Test Message"
#define MQTT_DUP               0
#define MQTT_QOS               0
#define MQTT_RETAIN            0
#define MQTT_PAYLOAD_SIZE      256

typedef enum {
    STATE_INIT,
    STATE_START_WLAN,
    STATE_WAIT_FOR_AP_CONNECT,
    STATE_CONFIGURE_CLOUD,
    STATE_PUBLISH_CLOUD,
} ATCMD_STATES;

uint16_t pubMqtt(void);

#endif	/* XC_HEADER_TEMPLATE_H */

