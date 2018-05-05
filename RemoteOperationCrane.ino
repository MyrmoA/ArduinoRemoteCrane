/*
 Created by Arie Myrmo
 
 Arduino Web Server with Ethernet Shield
 Controls Crane from the web server
 */


#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>
int magnetController = 9; 
int magnetLED = 7;
Servo winchServo; 
Servo boomServo;

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xD0, 0xE6 };
IPAddress ip(69,88,163,52); // IP address of Arduino;          
EthernetServer server(80); // server port 80   
String readString;

void setup() {
 
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  Serial.setTimeout(100);
  pinMode(magnetLED, OUTPUT);
  pinMode(magnetController, OUTPUT);
  boomServo.attach(5);
  winchServo.attach(6);
  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
  
}


void loop() {
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {   
      if (client.available()) {
        char c = client.read();
     
        //read char by char HTTP request
        if (readString.length() < 100) {
          //store characters to string
          readString += c;
          //Serial.print(c);
         }

         //if HTTP request has ended
         if (c == '\n') {          
           Serial.println(readString); //print to serial monitor for debuging
     
           client.println("HTTP/1.1 200 OK"); //send new page
           client.println("Content-Type: text/html");
           client.println();     
           client.println("<HTML>");
           client.println("<HEAD>");
           
           client.println("<link rel='stylesheet' type='text/css' href='http://randomnerdtutorials.com/ethernetcss.css' />");
           client.println("<TITLE>Remotely Operated Crane</TITLE>");
           client.println("</HEAD>");
           client.println("<BODY>");
           client.println("<H1>REMOTELY OPERATED CRANE</H1>");
           client.println("<hr />");
           client.println("<br />");  
           client.println("<H2>Arduino Crane Controls</H2>");
           client.println("<br />");  
           client.println("<a href=\"/?button1on\"\">Turn On ElectroMagnet</a>");
           client.println("<a href=\"/?button1off\"\">Turn Off ElectroMagnet</a><br />");   
           client.println("<br />");     
           client.println("<br />"); 
           client.println("<a href=\"/?button2on\"\">Lower Magnet</a>");
           client.println("<a href=\"/?button2off\"\">Stop Lower</a><br />");
           client.println("<br />"); 
           client.println("<br />"); 
           client.println("<a href=\"/?button3on\"\">Raise Magnet</a>");
           client.println("<a href=\"/?button3off\"\">Stop Raise</a><br />");
           client.println("<br />"); 
           client.println("<br />"); 
           client.println("<a href=\"/?button4on\"\">Rotate Clockwise</a>");
           client.println("<a href=\"/?button4off\"\">Stop Rotating Clockwise</a><br />");
           client.println("<br />"); 
           client.println("<br />"); 
           client.println("<a href=\"/?button5on\"\">Rotate Counter Clockwise</a>");
           client.println("<a href=\"/?button5off\"\">Stop Rotating Counter Clockwise</a><br />");
           client.println("<br>");
           client.println("<br/>");
           client.println("<br>");
           client.println("<br/>");
           client.println("Copyright 2018 by Florida Gulf Coast University");
           client.println("<br>");
           client.println("<br/>");
           client.println("<p>Created by Arie Myrmo and Jeff Lewis</p>");  
           client.println("<br />"); 
           client.println("<br>");
           client.println("<br>");
           client.println("<br/>");
           client.println("</BODY>");
           client.println("</HTML>");
     
           delay(1);
           // stopping client
           client.stop();
           
           // controls the Electromagnet and LED if you press the buttons
           if (readString.indexOf("?button1on") >0){
               digitalWrite(magnetLED, HIGH);
               digitalWrite(magnetController, HIGH);
           }
           if (readString.indexOf("?button1off") >0){
               digitalWrite(magnetLED, LOW);
               digitalWrite(magnetController, LOW);
           }

           
           // winch down
           if (readString.indexOf("?button2on") >0){
               winchServo.writeMicroseconds(1000);
           }
           if (readString.indexOf("?button2off") >0){
                winchServo.writeMicroseconds(1500);
           }

           // winch up
           if (readString.indexOf("?button3on") >0){
               winchServo.writeMicroseconds(2000);
           }
           if (readString.indexOf("?button3off") >0){
                winchServo.writeMicroseconds(1500);
           }




           // boom clockwise
           if (readString.indexOf("?button4on") >0){
               boomServo.writeMicroseconds(1540);
           }
           if (readString.indexOf("?button4off") >0){
                boomServo.writeMicroseconds(1500);
           }

           // boom counter-clockwise
           if (readString.indexOf("?button5on") >0){
               boomServo.writeMicroseconds(1460);
           }
           if (readString.indexOf("?button5off") >0){
                boomServo.writeMicroseconds(1500);
           }





            //clearing string for next read
            readString="";  
           
         }
       }
    }
}
}

