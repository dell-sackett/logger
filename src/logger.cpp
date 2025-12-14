/******************************************************************************
 * logger.c
 * @author Dell Sackett
 * @date 19 September 2025
 * Description: This is the implimentaion of a logging task for FreeRTOS for
 *              the ESP32. It is self contained and only needs to be started
 *              by passing tskLogger to the task create function of your 
 *              choice. Additionally, it needs allocated 1536 bytes of task
 *              memory to run correctly
 *  
 *****************************************************************************/

// Includes
#include "logger.h"

// Variable from the header
QueueHandle_t logQueueHandle;

/** 
 * logWrite
 * @brief Add a message to the log.
 *
 * @param logLevel The log level for this entry
 * @param message  The message for this entry 
 * 
 * Description: This functions adds and entry into the log queue to be printed
 *              to the serial port. Specifically Serial from the Arduino 
 *              Framework. It takes two parameters. logLevel: uint16_t - one 
 *              of the log levels defined in logger.h INFO, WARNING, or 
 *              CRITICAL. message a std::string with the message to be 
 *              displayed. Cannot exceed LOG_MESSAGE_MAX_SIZE - 1 or it will be
 *              truncated. By default LOG_MESSAGE_MAX_SIZE is 500 characters.
 **/
void logWrite(uint16_t logLevel, std::string message) {
    
    // Malloc some memory
    LogMessage logMsg;
    int messageLength = message.size();

    // If the incoming message is too long then we'll truncate it.
    if (messageLength > LOG_MESSAGE_MAX_SIZE) {
        messageLength = LOG_MESSAGE_MAX_SIZE - 1;
    }
    memcpy(&logMsg.msg, message.c_str(), messageLength);

    // If the incomming message was too long, then we need to add a null termination
    if (messageLength == LOG_MESSAGE_MAX_SIZE - 1) {
        logMsg.msg[LOG_MESSAGE_MAX_SIZE - 1] = '\0';
    } else {
        logMsg.msg[messageLength] = '\0';
    }
    logMsg.logLevel = logLevel;
    xQueueSend(logQueueHandle, (void*)&logMsg, (portMAX_DELAY/portTICK_PERIOD_MS));

}

/** 
 * initLog
 * @brief Initialize the log.
 *
 * @param queue A QueueHandle_t structure.
 * 
 * Description: This function initializes the the log. Specifically it allows
 *              the user to pass in a QueueHandle_t for the log to use, or if 
 *              the input parameter is NULL, it creates a queue for them.
 **/
void initLog(QueueHandle_t queue) {
    if (queue == NULL) {
        logQueueHandle = xQueueCreate(LOG_QUEUE_SIZE, sizeof(LogMessage));
    } else {
        logQueueHandle = queue;
    }
#if DEBUG
    Serial.print("logQueueHandle created at: ");
    Serial.println((int)logQueueHandle);
#endif
}

/** 
 * tskLogger
 * @brief The function to be called by the FreeRTOS task for logging.
 *
 * @param *pvParameters A void pointer to Stack Allocated parameters to pass in
 * 
 * Description: This is the main logging task. It monitors the queue and writes
 *              entries in the queue to the serial port.
 **/
void tskLogger(void *pvParameters) {
    
    Serial.begin(LOG_BAUD);
    Serial.println("Logger Task Starting!");

    initLog(NULL);
    LogMessage inboundMessage;

    while(1) {

        if(xQueueReceive(logQueueHandle, (void*)&inboundMessage, (1000 / portTICK_PERIOD_MS)) == pdTRUE) {

            // Print the log level
            switch(inboundMessage.logLevel) {

                case INFO:
                    Serial.print("[INFO] ");
                    break;
                case WARNING:
                    Serial.print("[WARNING] ");
                    break;
                case CRITICAL:
                default:
                    Serial.print("[CRITICAL] ");
                    break;
            }

            // Print the message
            Serial.println(inboundMessage.msg);
            Serial.println();
            
            // Free the pointer to the string for the message first, then the message. 
            //vPortFree(inboundMessage.msg);

        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}