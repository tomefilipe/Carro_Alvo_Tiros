#include "OTA.h"
#include <esp_ota_ops.h>
#include <esp_partition.h>
#include <HTTPUpdate.h>

WebServer * OTA::_server;

void OTA::begin(WebServer *server){
    _server = server;

    _server->on("/update", HTTP_GET, index);
    _server->on("/update", HTTP_POST, [](){
                _server->sendHeader("Connection", "close");_server->send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
                ESP.restart();}, update);
}

void OTA::update(){
    HTTPUpload& upload = _server->upload();
    if (upload.status == UPLOAD_FILE_START) {
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH)) { //start with max available size
            log_i("not begin");
            Update.printError(Serial);
        }
    }
    if (upload.status == UPLOAD_FILE_WRITE) {
        Serial.printf("Downloaded: %4d Kb\n", upload.totalSize/1024);
      /* flashing firmware to ESP*/
        size_t size = Update.write(upload.buf, upload.currentSize);
        if (size != upload.currentSize) {
            Update.printError(Serial);
            log_v("writed: %d, desired: upload.currentSize %d", size, upload.currentSize);
        }
    } else if (upload.status == UPLOAD_FILE_END || upload.status == UPLOAD_FILE_ABORTED) {
        if (Update.end(true)) { //true to set the size to the current progress
            Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
            Update.printError(Serial);
            log_v("something dont upload");
        }
    }
}

void OTA::index(){
    String response = Pages::indexUp;
    _server->send(200, "text/html", response);
}