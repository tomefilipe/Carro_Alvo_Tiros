#pragma once
#include "tasks.hpp"
#include "streamingProvider.hpp"
#include "clientManager.hpp"
#include "WebServer.h"

/// Streaming responsable 
class Streamer: public Task{
public:
    /// Begin client manager and provider. That is inst internal variables and
    /// start 3 tasks: client manger (handler with clients queue), provider (get data) and streamer (send to all clients)
    /// @param fps Desired transmission rate 
    void begin(uint8_t fps, StreamingProvider * provider = nullptr);

    /// Interface to client maneger server, adding routes in server
    /// @param uri addres to route @param fn routine to router @param HTTP_METHOD (optional) Especify method to run this routine, defaut HTTP_ANY
    void on(const Uri &uri, WebServer::THandlerFunction fn, HTTPMethod method = HTTP_ANY);

    /// Provide same interface of WebServer send. Send respose to client @param code response http code @param contentType http type @param content content
    void send(int code, const char * contentType, String& content);

    /// Provide same interface of WebServer args @return Number of args in request
    int args();

    /// Provide same interface of WebServer arg. @param i index of argument @return Argument of request with index i
    String arg(int i);

    /// Provide same interface of WebServer argName.@param i index of argument @return Argument name of request with index i
    String argName(int i);

    /// Getter for web server
    WebServer *server();


protected:
    /// Stream to all available clients. To do this adequade sleep time to number of clients
    void _loop();

    /// Desired transmission period
    TickType_t _totalPeriod;

    /// Provide content to stream
    StreamingProvider *_provider;

    /// Manager Clients queue
    ClientManager _clientManager;
};
