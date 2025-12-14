#include <logger.h>

int counter = 0;

void setup() {

    // Start the task
    xTaskCreate(tskLogger,
            "Logging Task",
            LOGGER_MEM,
            NULL,
            1,
            NULL);
    
    // Wait 2 milliseconds for the logger to be ready
    vTaskDelay(2 / portTICK_PERIOD_MS);

}

void loop() {

    // Put together a string
    std::string message = "This is a log message ";
    message += std::to_string(counter);
    message += "!";

    // Send the string to the logger
    logWrite(INFO, message);
    counter += 1;
    
    // Wait 2 seconds
    vTaskDelay(2000 / portTICK_PERIOD_MS);

}