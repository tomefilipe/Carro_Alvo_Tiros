#pragma once
#include <WebServer.h>
#include <Update.h>

#include "index.hpp"

/// Update over the air (OTA) class 
class OTA{
public:
    /// copy server pointer to internal atribute and add router to update 
    /// @param  server pointer to server
    void begin(WebServer *server);

    /// Update firmware
    static void update();

    static void index();

protected:
    /// Web server
    static WebServer * _server;
};