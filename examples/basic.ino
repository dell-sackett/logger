#include <logger.h>

int counter = 0;

void setup() {
    xTaskCreate(tskLogger,
            "Logging Task",
            LOGGER_MEM,
            NULL,
            1,
            NULL);
}

void loop() {

    logWrite(INFO, "This is log message " + counter + "!");
    counter += 1;
    
    vTaskDelay(2000 / portTICK_PERIOD_MS);

}