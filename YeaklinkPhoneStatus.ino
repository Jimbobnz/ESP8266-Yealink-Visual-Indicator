/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>

const char* ssid = "YourSSID";
const char* password = "dontpanic";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  Serial.begin(115200);
  delay(10);

  lcd.init();                      // initialize the lcd 
  lcd.init();

  
  
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print( WiFi.localIP() );

  delay(10000);
  lcd.noBacklight();  
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;
  
  if (req.indexOf("/established") != -1) {
   lcd.clear();
    lcd.setCursor(1,0);
    lcd.print( "SORRY, BUSY." );
    lcd.setCursor(1,1);
    lcd.print( "ON THE PHONE!" );    
    lcd.backlight(); 
  }
  else if (req.indexOf("/onhook") != -1) {
    lcd.setCursor(0,0);
    lcd.clear();
    lcd.noBacklight();   
  }
  else if (req.indexOf("/offhook") != -1)
  {
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print( "SORRY, BUSY." );
    lcd.setCursor(0,1);
    lcd.print( "MAKING PHONECALL" );    
    lcd.backlight();
  }
  else {
    Serial.println("invalid request");
    client.stop();
    lcd.noBacklight(); 
    return;
  }

  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>";
  
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

