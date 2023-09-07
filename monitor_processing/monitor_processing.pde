import processing.serial.*;

Serial myPort;  // Create object from Serial class

void setup() 
{
  String portName = Serial.list()[0]; //change the 0 to a 1 or 2 etc. to match your port
  myPort = new Serial(this, portName, 57600);
}

void draw()
{
  while (myPort.available() > 0) 
  { 
    String inByte = myPort.readStringUntil('\n');
    if (inByte != null) 
    {
      println(inByte);  // Print the incoming data
    }
  }
}
