# Logger
Logger is a library I developed for personal use to log to the serial port of an ESP32 for my own projects. It is self contained, and just needs started as a FreeRTOS thread.


## Usage
Using the logger in the most basic way is simple, you just start the task using xTaskCreate, or whichever version is appropriate for your situation. An example can be seen below. ***Note: The vTaskDelay() is NOT OPTIONAL. It needs to be at least 2 milliseconds long or you're going to have a bad time.***

```     
xTaskCreate(tskLogger,
    "Logging Task",
    LOGGER_MEM,
    NULL,
    1,
    NULL);
vTaskDelay(2 / portTICK_PERIOD_MS); 
```

Then you just call the function logWrite() to log your message. An example can be seen below.

```
logWrite(INFO, "This is a log message!");
```

## Log levels
There are 3 log levels built in. INFO, WARNING, and CRITICAL.

## Memory
If you're strapped for memory you can change the memory depth from LOGGER_MEM to LOGGER_MEM_MIN. Likewise, if you need to log crazy large messages you can use LOGGER_MEM_MAX.  Usually LOGGER_MEM will get the job done.
