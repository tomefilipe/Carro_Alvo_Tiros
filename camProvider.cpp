#include "camProvider.hpp"

void CamProvider::begin(uint8_t fps){
    /// config pins of esp32 AI-Thinker
    _config = CameraPins::ai_thinker;
    _config.ledc_timer   = LEDC_TIMER_0;
    _config.ledc_channel = LEDC_CHANNEL_0;
    _config.pixel_format = PIXFORMAT_JPEG;
    _config.xclk_freq_hz = 20000000;

    if ( psramFound() ){
        log_i("PSRAM found");
        psramInit();
        _config.frame_size = FRAMESIZE_SVGA;
        _config.jpeg_quality = 15;
        _config.fb_count = 2;
    } else{
        log_i("PSRAM not found");
        _config.frame_size = FRAMESIZE_VGA;
        _config.jpeg_quality = 17;
        _config.fb_count = 1;
    }
    
    if( esp_camera_init(&_config) != ESP_OK ){
        log_e("Fail in camera init, reboot in 10s");
        vTaskDelay(pdMS_TO_TICKS(10000));
        esp_restart();
    }

    _sensor = esp_camera_sensor_get();       

                     
//////////////////////////////////////////////////////////////////////////////////////////////
        _sensor->set_quality(_sensor, 10);       // 10 to 63
        _sensor->set_brightness(_sensor, 0);      // -2 to 2
        _sensor->set_contrast(_sensor, -2);        // -2 to 2
        _sensor->set_saturation(_sensor, 0);      // -2 to 2
        _sensor->set_special_effect(_sensor, 0);  // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
        //_sensor->set_whitebal(_sensor, 1);        // aka 'awb' in the UI; 0 = disable , 1 = enable
        _sensor->set_awb_gain(_sensor, 1);        // 0 = disable , 1 = enable
        _sensor->set_wb_mode(_sensor, 2);         // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
        _sensor->set_exposure_ctrl(_sensor, 1);   // 0 = disable , 1 = enable
        _sensor->set_aec2(_sensor, 0);            // 0 = disable , 1 = enable
        _sensor->set_ae_level(_sensor, 0);        // -2 to 2
        _sensor->set_aec_value(_sensor, 1000);     // 0 to 1200
        _sensor->set_gain_ctrl(_sensor, 1);       // 0 = disable , 1 = enable
        _sensor->set_agc_gain(_sensor, 30);        // 0 to 30
        _sensor->set_gainceiling(_sensor, (gainceiling_t)0);  // 0 to 6
        _sensor->set_bpc(_sensor, 0);             // 0 = disable , 1 = enable
        _sensor->set_wpc(_sensor, 0);             // 0 = disable , 1 = enable
        _sensor->set_raw_gma(_sensor, 1);         // 0 = disable , 1 = enable
        _sensor->set_lenc(_sensor, 1);            // 0 = disable , 1 = enable
        //_sensor->set_hmirror(_sensor, 1);         // 0 = disable , 1 = enable
        _sensor->set_vflip(_sensor, 0);           // 0 = disable , 1 = enable
        //_sensor->set_dcw(_sensor, 0);             // 0 = disable , 1 = enable
        //_sensor->set_colorbar(_sensor, 0);        // 0 = disable , 1 = enable       


//////////////////////////////////////////////////////////////////////////////////////////////    
 

    /// crop image offset must be 0 for svga
    // int xi = 760; int yi = 500;                  // int xi = 380; int yi = 260;              UXGA       //720 , 310
    // int xf = 860; int yf = 640;                 //int xf = 1020; int yf = 760;                          //880 , 640
    // int rx = 800; int ry = 600;
    // rx = (xf - xi);
    // ry = (yf - yi);
     //crop(837, 445, 1600, 1200, 400, 400, UXGA);               //             SVGA = 800 por 600  UXGA = 1600 por 1200
    
    //Posi 1
    crop(837, 445, 840, 460, 400, 400, UXGA); 
    
    /// Create a task to read camera
    StreamingProvider::begin(fps);
    log_i("Getter begin");
}

bool CamProvider::crop(int offsetX, int offsetY, int totalX, int totalY, int w, int h, int quality){
    if (_sensor->id.PID == OV2640_PID){
        _sensor->set_res_raw(_sensor, quality, 0, 0, 0, offsetX, offsetY, totalX, totalY, w, h, false, false);
        return true;
    }
    log_e("Crop is not available for sensor type yet");
    return false;
}

bool CamProvider::_getData(uint8_t **ptr, size_t *size){
    _fb = esp_camera_fb_get();
    if ( _fb != nullptr ){
        if ( _fb->format == PIXFORMAT_JPEG ){
            *ptr = _fb->buf;
            *size = _fb->len;
            return true;
        }
        return frame2jpg(_fb, 15, ptr, size);
    } 
    log_i("error in frame capture");
    return false;
}

void CamProvider::_releaseData(){
    esp_camera_fb_return(_fb);
}
