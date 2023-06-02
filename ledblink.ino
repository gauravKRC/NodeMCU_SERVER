#include <ESP8266WiFi.h>

WiFiClient client;
WiFiServer server(80);

int onBoardLed = 2;

void setup() 
{
  Serial.begin(9600);
 // WiFi.begin("ZERO 4132", "55882244");

   WiFi.begin("CHAL HAT", "1234554321");

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("NodeMCU is connected!");
  Serial.println(WiFi.localIP());
  server.begin();
  pinMode(onBoardLed, OUTPUT);
  digitalWrite(onBoardLed, HIGH);
}

void loop() 
{
  client = server.available();  
  if (client)  // Corrected condition
  {  
    String request =  client.readStringUntil('\n');
    int timer = request.substring(6,request.length()-10).toInt();
    if (timer > 0) {
      Serial.println(timer);
      digitalWrite(onBoardLed, LOW);
      delay(1000 * timer);
      digitalWrite(onBoardLed, HIGH);
    }
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE html><html><head><title>Relay Control</title>");
  client.println("<script>function sendReq() {let value = document.getElementById('amount').value;");
  client.println("if (value >= 0 && !isNaN(parseFloat(value))) {");
  client.println("let time = parseFloat(value) * 5; let url = window.location.href + '/' + time;");
  client.println("let msg = document.getElementById('time'); msg.value = time; msg.style.color = 'green';");
  client.println("let remainingTime = setInterval(() => {time = time - 1; msg.value = time;");
  client.println("if (time == 0) { clearInterval(remainingTime); msg.style.color = 'red'; }}, 1000);");
  client.println("fetch(url).then(res => {}).catch(err => {alert('Fatal Error: Something went wrong.');});}");
  client.println("else { alert('Error: Invalid Amount'); document.getElementById('amount').value = ''; } }");
  client.println("</script></head><body onload='setup()'><h1>Relay Control</h1>");
  client.println("<p><b>Time will be 5 times your amount</b></p>");
  client.println("<br><label for='amount'>Amount:</label><br>");
  client.println("<input type='number' name='amount' id='amount'>");
  client.println("<button type='button' id='submit' onclick='sendReq()'>Submit</button>");
  client.println("<br><br><label for='time'>Time Remaining:</label><br>");
  client.println("<input type='text' id='time' disabled></body></html>");
}