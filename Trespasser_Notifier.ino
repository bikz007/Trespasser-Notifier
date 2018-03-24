#include <ESP8266WiFi.h>

//initializing global variables
int led = 5;  // Digital pin D1
int buzz=4; //Digital pin D2
int connectled=0;//Digital pin D3
int sensor = 13;  // Digital pin D7
const char* ssid = "xx"; //network ssid
const char* password = "xx"; //network password
const char* host = "maker.ifttt.com";

//func to get value from motion detector sensor ,and passing the value
bool motionreturn()
{
    //Reading input from output pin of the sensor
    long state = digitalRead(sensor);
  
    //checking---if motion detected do the following tasks
    if(state == HIGH) {
      Serial.println("Motion detected!");
      digitalWrite (led, LOW);
      digitalWrite (buzz, LOW);
      int count=0;
      while(count<150)
      {
         Serial.println(count);
         digitalWrite (led, HIGH);
         digitalWrite (buzz, HIGH);
         delay(1000);
         digitalWrite (led, LOW);
         digitalWrite (buzz, LOW);
         delay(1000);
         count++;
      }
      return true;
      //delay(1000);
    }
  
    //motion not detected?Do this following tasks
    else {
      digitalWrite (led, LOW);
      digitalWrite (buzz, LOW);
      Serial.println("Motion absent!");
      return false;
      //delay(1000);
    }
}

void setup() 
{
    //initializing serial moniter
    Serial.begin(115200);
    //initializing pins
    pinMode(sensor, INPUT);   // declare sensor as input
    pinMode(led, OUTPUT);  // declare LED as output
    pinMode(buzz, OUTPUT); //declare BUZZER as OUTPUT
    pinMode(connectled,OUTPUT); //wifi connected status
    delay(200);

    digitalWrite (connectled, LOW);
  
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to : ");
    Serial.println(ssid);
  
    WiFi.begin(ssid, password);
  
    //checking if connected to wifi or not
    while (WiFi.status() != WL_CONNECTED) {
      digitalWrite (connectled, HIGH);
      delay(100);
      Serial.print("*");
    }
  
    Serial.println("");
    //if connected print the ip-address
    Serial.println("WiFi connected");
    digitalWrite (connectled, LOW);  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}


void loop() 
{
      delay(3000); //3 seconds delay
      //connecting to the host
      Serial.println("Connecting to : ");
      Serial.print(host);
      Serial.println();
      
      
    
      // Use WiFiClient class to create TCP connections
      WiFiClient client;
      const int httpPort = 80;
      if (!client.connect(host, httpPort)) 
      {
        Serial.println("Connection failed!! :(");
        return;
      }
    
       
       
       // We now create a URI for the request
    	 String url = "<url> from/trigger....";
    	 Serial.print("Requesting URL: ");
    	 Serial.println(url);
    	  
    	 bool motion=motionreturn();
       
    	 // If motion detected-----This will send the request to the server
    	 if(motion==true)
    	 {
    	   	client.print(String("POST ") + url + " HTTP/1.1\r\n" +
    	        		"Host: " + host + "\r\n" + 
    	        		"Connection: close\r\n\r\n");
    
    	    // Read all the lines of the reply from server and print them to Serial
    			while(client.available())
    		  {
    		        String line = client.readStringUntil('\r');
    		        Serial.print(line);
    		  }
    
    		  Serial.println();
    			Serial.println("Closing connection...");
    			delay(60000*25);
    	 }
      
      delay(500); //5 nano seconds delay
}
