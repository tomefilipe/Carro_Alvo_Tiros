#include "streamer.hpp"

void Streamer::begin(uint8_t fps, StreamingProvider *provider){
    _totalPeriod = pdMS_TO_TICKS(1000/fps);
    
    if ( provider != nullptr ){
        log_i("Getting provider");
        _provider = provider;
    } 
    else{
        log_i("Getting defaut provider");
        _provider = new StreamingProvider;
    }

    _provider->begin(fps);
    _clientManager.begin(500);
    run("streamer", 3);
}

void Streamer::on(const Uri &uri, WebServer::THandlerFunction fn, HTTPMethod method){
    _clientManager._server.on(uri, method, fn);
}

void Streamer::send(int code, const char * contentType, String& content){
    _clientManager._server.send(code, contentType, content);
}

int Streamer::args(){   
    return _clientManager._server.args();
}

String Streamer::arg(int i){
    return _clientManager._server.arg(i);
}

String Streamer::argName(int i){
    return _clientManager._server.argName(i);
}

WebServer *Streamer::server(){
    return &_clientManager._server;
}

void Streamer::_loop(){
    UBaseType_t available = _clientManager.available();
    _period = _totalPeriod;
    if ( available ){
        /// adjust the waiting time to serve all customers with the desired latency
        _period /= available;
        /// if provider is suspended wakeup and wait to capture a sample of stream
        if ( _provider->resume() ) return;
        if( _clientManager.next() ){
            _provider->waitAvailable();
            _clientManager.send(_provider->buffer(), _provider->size());
            _provider->release();
        }
    }else{
        _provider->suspend();
    }
}