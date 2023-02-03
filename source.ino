#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

#define STATUS_LED 2

const char* headersList[] = {"Accept-CH","Access-Control-Allow-Origin",
"Access-Control-Allow-Credentials","Access-Control-Expose-Headers","Access-Control-Max-Age",
"Access-Control-Allow-Methods","Access-Control-Allow-Headers",
"Accept-Patch","Accept-Ranges","Age",
"Allow","Alt-Svc","Cache-Control","Connection","Content-Disposition",
"Content-Encoding","Content-Language","Content-Length","Content-Location","Content-MD5",
"Content-Range","Content-Type","Date","Delta-Base","ETag","Expires","IM","Last-Modified","Link",
"Location","P3P","Pragma","Preference-Applied","Proxy-Authenticate","Public-Key-Pins",
"Retry-After","Server","Set-Cookie","Strict-Transport-Security","Trailer","Transfer-Encoding",
"Tk","Upgrade","Vary","Via","Warning","WWW-Authenticate","X-Frame-Options"};
const int headersCount = 48;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(99999999999);
  pinMode(STATUS_LED, OUTPUT);
  String ssid = Serial.readStringUntil('\n');
  String password = Serial.readStringUntil('\n');
  digitalWrite(STATUS_LED, HIGH);
  WiFi.begin(ssid.c_str(), password.c_str());
  while(WiFi.status() != WL_CONNECTED) delay(50);
  digitalWrite(STATUS_LED, LOW);
  Serial.println("connected");
}

void loop() {
  String input = Serial.readStringUntil('\n');
  JSONVar req = JSON.parse(input);
  digitalWrite(STATUS_LED, HIGH);
  if(JSON.typeof(req) == "undefined"){
    Serial.println("{\"err\":\"INVALID_JSON\"}");
    return;
  }
  HTTPClient http;
  http.begin(req["url"]);
  int resCode;
  if(strcmp(JSON.stringify(req["headers"]).c_str(), "{}") != 0){
    JSONVar keys = req["headers"].keys();
    for(int i = 0; i < keys.length(); i++){
      http.addHeader(keys[i], req["headers"][keys[i]]);
    }
  }
  http.collectHeaders(headersList, headersCount);
  if(strcmp(JSON.stringify(req["method"]).c_str(), "\"POST\"") == 0 || strcmp(JSON.stringify(req["method"]).c_str(), "\"post\"") == 0)
    resCode = http.POST(JSON.stringify(req["body"]));
  else
    resCode = http.GET();
  
  JSONVar res;
  res["status"] = resCode;
  res["body"] = http.getString();
  int headers_count = http.headers();
  for(int i = 0; i < headers_count; i++)
    res["headers"][http.headerName(i)] = http.header(i);
  if(headers_count == 0) res["headers"]["Content-Type"] = "text/plain";
  Serial.println(JSON.stringify(res));

  http.end();
  digitalWrite(STATUS_LED, LOW);
}
