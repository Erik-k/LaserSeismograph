import processing.serial.*;

PrintWriter output;
Serial myPort;  // The serial port
int stopByte = 44; // stop at character 44 which is ,

void setup() {
  // List all the available serial ports
  printArray(Serial.list());
  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[2], 115200);
  output = createWriter("PDreadings.csv");
}

void draw() {
  while (myPort.available() > 0) {
    String inBuffer = myPort.readString();   
    if (inBuffer != null) {
      println(inBuffer);
      output.println(inBuffer);
      output.flush();
    }
  }
}