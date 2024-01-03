#pragma once

#define DESIRED_W "416"
#define DESIRED_H "420"

#define OFFSET_Y "180"      //170 160
#define OFFSET_X "70"       //70
#define WIDTH "400"         //400
#define HEIGHT "210"        //140
#define SCALE_X "1.2"       //1.2
#define SCALE_Y "3.5"       //3.5

namespace Pages{
    constexpr const char * indexCrop = "<!DOCTYPE html>\
<head>\
    <meta charset=\"UTF-8\">\
</head>\
<html>\
    <div style='display: flex; justify-content: center;'>\
        <div style='width:" WIDTH "px;  height: " HEIGHT "px; overflow: hidden; margin: 0; transform: scale("  SCALE_X  ", " SCALE_Y ");'>\
            <img id='image1' style='transform: rotate(90deg);  margin: -" OFFSET_Y "px 0px 0px -" OFFSET_X "px;' src='/stream'/>\
        </div>\
    </div>\
</html>";

    constexpr const char * index = "<!DOCTYPE html>\
<head>\
    <meta charset=\"UTF-8\">\
</head>\
<html>\
    <div style='display: flex; justify-content: center;'>\
        <div>\
            <img id='image1' style='transform: rotate(90deg);' src='/stream'/>\
        </div>\
    </div>\
</html>";

    constexpr const char * indexUp = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
    "<input type='file' name='update'>"
    "<input type='submit' value='Upload'>"
"</form>"
"<div id='prg'>progress: 0%</div>"
"<script>"
"$('form').submit(function(e){"
    "e.preventDefault();"
      "var form = $('#upload_form')[0];"
      "var data = new FormData(form);"
      " $.ajax({"
            "url: '/update',"
            "type: 'POST',"               
            "data: data,"
            "contentType: false,"                  
            "processData:false,"  
            "xhr: function() {"
                "var xhr = new window.XMLHttpRequest();"
                "xhr.upload.addEventListener('progress', function(evt) {"
                    "if (evt.lengthComputable) {"
                        "var per = evt.loaded / evt.total;"
                        "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
                    "}"
               "}, false);"
               "return xhr;"
            "},"                                
            "success:function(d, s) {"    
                "console.log('success!')"
           "},"
            "error: function (a, b, c) {"
            "}"
          "});"
"});"
"</script>";

constexpr const char * iteCrop = "<!DOCTYPE html>\
<head>\
    <meta charset='UTF-8'/>\
    <script>\
        function margin(name, value){\
        	if (value == '') return;\
        	value *= -1;\
        	img = document.getElementById('image1');\
        	if (name == 'top')\
        		img.style.marginTop = value.toString() + 'px';\
        	else if ( name == 'left' )\
        		img.style.marginLeft = value.toString() + 'px';		\
        }\
        function setSize(name, value){\
        	if (value == '') return;\
        	div = document.getElementById('divCrop');\
        	if (name == 'width')\
        		div.style.width = value.toString() + 'px';\
        	else if ( name == 'height' )\
        		div.style.height = value.toString() + 'px';		\
        }\
		function setScale(){\
        	if (scaleX.value == '' || scaleY.value == '') return;\
        	div.style.transform = 'scale(' + scaleX.value + ', ' + scaleY.value + ')';	\
        }\
		function autoScaleCalc(){\
        	if (scaleX.value == '' || scaleY.value == '') return;\
			scaleX.value = " DESIRED_W ".0/divCrop.getBoundingClientRect().width;\
			scaleY.value = " DESIRED_H ".0/divCrop.getBoundingClientRect().height;\
        	div.style.transform = 'scale(' + scaleX.value + ', ' + scaleY.value + ')';	\
		}\
    </script>\
</head>\
<html>\
    <div style='display: flex; justify-content: center;'>\
        <div id='divCrop' style='overflow: hidden; margin: 0;'>\
            <img id='image1' style='transform: rotate(90deg); margin: 0' src='/sample'/>\
        </div>\
        <div style='display:flex; flex-direction: column; justify-content: center; height: 200px;'>\
        	<div style='display:flex; flex-direction: row; justify-content: space-between;'>\
				<input type='number' name = 'top'  oninput='margin(this.name,this.value)'/>\
				<input type='number' name = 'left' oninput='margin(this.name,this.value)'/>\
        	</div>\
        	<div style='display:flex; flex-direction: row; justify-content: space-between;'>\
				<input type='number' name = 'width' min='1' value='1200'   oninput='setSize(this.name,this.value)'/>\
				<input type='number' name = 'height' min='1' value='1600' oninput='setSize(this.name,this.value)'/>\
        	</div>\
			<div style='display:flex; flex-direction: row; justify-content: space-between;'>\
				<input type='number' id = 'scaleX' min='1' value='1' oninput='setScale()'/>\
				<input type='number' id = 'scaleY' min='1' value='1' oninput='setScale()'/>\
			</div>\
			<div style='display:flex; flex-direction: row; justify-content: space-between;'>\
				<button onclick='autoScaleCalc()'>Auto</button>\
			</div>\
        </div>\
    </div>\
</html>";
}
