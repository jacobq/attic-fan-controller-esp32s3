// Over-the-Air update support via free version of https://github.com/ayushsharma82/ElegantOTA
// based on Demo.ino example

#include <Arduino.h>
#include <ElegantOTA.h>

unsigned long ota_progress_millis = 0;

void onOTAStart() {
    // Log when OTA has started
    printf("OTA update started!");
    // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) {
    // Log every 1 second
    if (millis() - ota_progress_millis > 1000) {
        ota_progress_millis = millis();
        printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
    }
}

void onOTAEnd(bool success) {
    // Log when OTA has finished
    if (success) {
        printf("OTA update finished successfully!");
    } else {
        printf("There was an error during OTA update!");
    }
    // <Add your own code here>
}

void ota_init(WebServer &server)
{
    ElegantOTA.begin(&server);
    ElegantOTA.onStart(onOTAStart);
    ElegantOTA.onProgress(onOTAProgress);
    ElegantOTA.onEnd(onOTAEnd);
}

void ota_task()
{
    ElegantOTA.loop();
}