
#define DEBUG true
 
void setup()
{
  digitalWrite(12, 100);
  Serial.begin(9600);
  // Configure na linha abaixo a velocidade inicial do
  // modulo ESP8266
  
  Serial1.begin(74880);
  sendData("AT+RSTrn", 2000, DEBUG);
  delay(1000);
  Serial.println("Versao de firmware");
  delay(3000);
  sendData("AT+GMRrn", 2000, DEBUG); // rst
  // Configure na linha abaixo a velocidade desejada para a
  // comunicacao do modulo ESP8266 (9600, 19200, 38400, etc)
  sendData("AT+CIOBAUD=19200rn", 2000, DEBUG);
  Serial.println("** Final **");
}
 
void loop() {}
 
String sendData(String command, const int timeout, boolean debug)
{
  // Envio dos comandos AT para o modulo
  String response = "";
  Serial1.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (Serial1.available())
    {
      // The esp has data so display its output to the serial window
      char c = Serial1.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
