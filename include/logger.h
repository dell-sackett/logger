/******************************************************************************
 * logger.h
 * @author Dell Sackett
 * @date 19 September 2025
 * Description: This is the interface of a logging task for FreeRTOS for
 *              the ESP32. It is self contained and only needs to be started
 *              by passing tskLogger to the task create function of your 
 *              choice. Additionally, it needs allocated 1536 bytes of task
 *              memory to run correctly
 *  
 *****************************************************************************/
#pragma once
#ifndef LOGGER_H
#define LOGGER_H
#include "Arduino.h"

// Log level defines
#define INFO        0
#define WARNING     1
#define CRITICAL    2

// Log lengths
#define LOG_QUEUE_SIZE 10
#define LOG_MESSAGE_MAX_SIZE 500

// Define some memory depths for the task
#define LOGGER_MEM_MIN  1536
#define LOGGER_MEM      2048
#define LOGGER_MEM_MAX  4096

// Log Message Structure
typedef struct {
    char msg[LOG_MESSAGE_MAX_SIZE];
    uint16_t logLevel;

} LogMessage;

// Log variables
extern QueueHandle_t logQueueHandle;

// functions
void tskLogger(void *pvParameters);
void logWrite(uint16_t logLevel, std::string message);
void initLog(QueueHandle_t queue);

// log speed
#ifndef LOG_BAUD
#define LOG_BAUD 115200
#endif


#endif