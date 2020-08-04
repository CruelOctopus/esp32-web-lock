#include <Arduino.h>
#include <WiFi.h>
#include "mbedtls/md.h"
#include <Cryptojs.h>

const char *Ok_js_header = "HTTP/1.1 200 OK\nHost: site.com\nContent-Type: application/javascript; charset=utf-8\nConnection: close\nContent-Length:";
//'''+str(len(content))+''' \r\n\r\n'''

// Replace with your network credentials
//const char* ssid     = "ESP32-Access-Point";
//const char* password = "123456789";

const char *ssid = "ssid";
const char *password = "password";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header = "";

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;

uint8_t RandomNumberBuffer[32];
uint8_t hmacResult[32];

char *key = "1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef";

void Hmac256Calculate(uint8_t *key, uint8_t *payload, uint8_t *hmacResult)
{
  //char *key = "secretKey";
  //char *payload = "Hello HMAC SHA 256!";

  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;

  const size_t payloadLength = strlen((char*)payload);
  const size_t keyLength = strlen((char*)key);

  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 1);
  mbedtls_md_hmac_starts(&ctx, (const unsigned char *)key, keyLength);
  mbedtls_md_hmac_update(&ctx, (const unsigned char *)payload, payloadLength);
  mbedtls_md_hmac_finish(&ctx, hmacResult);
  mbedtls_md_free(&ctx);
}
String ByteToHex(char *hex)
{
  String result = "";
  Serial.print("Hash: ");
  for (int i = 0; i < sizeof(hex); i++)
  {
    char str[3];

    sprintf(str, "%02x", (int)hex[i]);
    Serial.print(str);
    result += str;
  }

  return result;
}
int Hmac256Verify(uint8_t *key, uint8_t *rowdata, String encrypteddata)
{
  uint8_t hmacResult[32];
  String result = "";

  Hmac256Calculate(key, rowdata,hmacResult);
  result = ByteToHex((char*)hmacResult);
  int counter = 0;
  for (int i = 0; i < 64; i++)
  {
    counter += result[i] ^ encrypteddata[i];
  }
  return counter;
}
void StringHexToByte(String InputData, uint8_t *bytearray, uint8_t arraylen)
{
  uint8_t b = 0, bt = 0;
  uint8_t halfbyte = 0;
  uint8_t j = 0;
  if ((InputData.length() % 2 == 0) && (InputData.length() == arraylen * 2))
  {
    for (int i = 0; i < arraylen; i++)
    {
      bytearray[i] = 0;
    }
    for (int i = 0; i < arraylen * 2; i++)
    {
      switch (InputData[i])
      {
      case '0':
      {
        b = 0;
        break;
      };
      case '1':
      {
        b = 1;
        break;
      };
      case '2':
      {
        b = 2;
        break;
      };
      case '3':
      {
        b = 3;
        break;
      };
      case '4':
      {
        b = 4;
        break;
      };
      case '5':
      {
        b = 5;
        break;
      };
      case '6':
      {
        b = 6;
        break;
      };
      case '7':
      {
        b = 7;
        break;
      };
      case '8':
      {
        b = 8;
        break;
      };
      case '9':
      {
        b = 9;
        break;
      };
      case 'a':
      {
        b = 10;
        break;
      };
      case 'b':
      {
        b = 11;
        break;
      };
      case 'c':
      {
        b = 12;
        break;
      };
      case 'd':
      {
        b = 13;
        break;
      };
      case 'e':
      {
        b = 14;
        break;
      };
      case 'f':
      {
        b = 15;
        break;
      };
      }
      if (halfbyte == 0)
      {

        bt = b << 4;
        halfbyte = 1;
      }
      else
      {
        bt |= b;
        bytearray[j] = bt;
        j++;
        halfbyte = 0;
        bt = 0;
      }
    }
  }
}
void setup()
{

  // put your setup code here, to run once:
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);

  /*------------- WIFI-AP
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);
 
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
--------------WIFI AP END */

  WiFi.begin(ssid, password);

  // пытаемся подключиться к WiFi-сети:
  while (WiFi.status() != WL_CONNECTED)
  {
    // подключаемся к сети WPA/WPA2; поменяйте эту строчку,
    // если используете открытую или WEP-сеть:
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected"); //  "Подключение
                                    //  по WiFi выполнено"
  Serial.println("IP address: ");   //  "IP-адрес: "
  Serial.println(WiFi.localIP());

  server.begin();
}
String GetRandomString(uint8_t *buffer, uint8_t size)
{
  void *Vp = buffer;
  uint32_t *Bp = static_cast<uint32_t *>(Vp);

  for (int i = 0; i < (size / 4); i++)
  {
    *(Bp + i) = esp_random();
  }
  //void *Vp = buffer;
  //uint8_t * Bp = static_cast<uint8_t*>(Vp);

  String test = "";
  char str[3];
  for (int i = 0; i < size; i++)
  {
    sprintf(str, "%02x", (int)buffer[i]);
    test = test + str;
    Serial.print(str);
  }

  Serial.println();
  Serial.print(test);
  Serial.println();

  return test;
}
String StrCut(String Inputdata, int pos1, int pos2)
{
  String OutputData = "";
  for (int i = pos1; i < pos2; i++)
  {
    OutputData += Inputdata[i];
  }
  return OutputData;
}

void GetPath(String str, String *path, int *NumArg)
{
  String buffer = "";
  String HTTP = "HTTP";
  String GET = "GET";

  int pos1, pos2;
  pos1 = 0;
  pos2 = 0;
  pos1 = str.indexOf(GET);
  pos2 = str.indexOf(HTTP);
  if ((pos1 != std::string::npos) && pos1 == 0)
  {
    if (pos2 != std::string::npos)
    {
      buffer = StrCut(str, pos1 + 4, pos2 - 1);
    }
  }

  //string path [10];
  int spliter[10];
  int counter = 0;
  for (int i = 0; i < buffer.length(); i++)
  {
    if (buffer[i] == '/')
    {
      spliter[counter] = i;
      counter++;
    }
  }
  *NumArg = counter;
  spliter[counter] = buffer.length();

  for (int i = 0; i < counter; i++)
  {
    for (int j = spliter[i]; j < spliter[i + 1]; j++)
    {
      path[i] += buffer[j];
    }
    //cout<<path[i]<<endl;
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available(); // Listen for incoming clients

  String RandomString = "";
  String OldRandomValue = "";
  char IsRandomSend = 0;
  int NArg = 0;
  String path[5];
  if (client)
  {                                // If a new client connects,
    Serial.println("New Client."); // print a message out in the serial port
    String currentLine = "";       // make a String to hold incoming data from the client
    int i = 0;
    while (client.connected())
    { // loop while the client's connected
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c);        // print it out the serial monitor
        //Serial.print(i);
        header += c;
        if (c == '\n')
        { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          // if (currentLine.length() == 0)
          // {
          // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
          // and a content-type so the client knows what's coming, then a blank line:
          GetPath(header, path, &NArg);
          Serial.print(path[0]);
          Serial.print(NArg);
          if (path[NArg - 1].indexOf(".js") > 0)
          {
            header = path[NArg - 1];
            if (header.indexOf("/Crypto.js") >= 0)
            {
              client.println("HTTP/1.1 200 OK");
              client.println("Host: site.com");
              client.println("Content-Type: text/javascript");
              client.println("Content-Encoding: gzip");
              client.println("Connection: close");
              client.println("Content-Length: 825");
              //Serial.print(sizeof(SC_Crypto_js_min_gz));
              client.println();
              client.write(SC_Crypto_js_min_gz, sizeof(SC_Crypto_js_min_gz));
              break;
            }
            if (header.indexOf("/HMAC.js") >= 0)
            {
              client.println("HTTP/1.1 200 OK");
              client.println("Host: site.com");
              client.println("Content-Type: text/javascript");
              client.println("Content-Encoding: gzip");
              client.println("Connection: close");
              client.println("Content-Length: 253");
              //Serial.print(sizeof(SC_Crypto_js_min_gz));
              client.println();
              client.write(SC_HMAC_js_min_gz, sizeof(SC_HMAC_js_min_gz));
              break;
            }
            if (header.indexOf("/SHA256.js") >= 0)
            {
              client.println("HTTP/1.1 200 OK");
              client.println("Host: site.com");
              client.println("Content-Type: text/javascript");
              client.println("Content-Encoding: gzip");
              client.println("Connection: close");
              client.println("Content-Length: 1014");
              //Serial.print(sizeof(SC_Crypto_js_min_gz));
              client.println();
              client.write(SC_SHA256_js_min_gz, sizeof(SC_SHA256_js_min_gz));
              break;
            }
          }
          else
          {

            if (path[0] == "/")
            {
              Serial.println("GET /root");
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html; charset=utf-8");
              client.println("Connection: close");
              client.println();

              // Display the HTML web page
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");
              // CSS to style the on/off buttons
              // Feel free to change the background-color and font-size attributes to fit your preferences
              client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
              client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
              client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
              client.println(".button2 {background-color: #555555;}</style>");

              client.println("<script type=\"text/javascript\" src=\"Crypto.js\"></script>");
              //client.println(s);
              client.println("<script type=\"text/javascript\" src=\"HMAC.js\"></script>");
              client.println("<script type=\"text/javascript\" src=\"SHA256.js\"></script>");
              client.println("</head>");
              // Web Page Heading
              client.println("<body><h1>ESP32 Web Server</h1>");

              // Display current state, and ON/OFF buttons for GPIO 27
              client.println("<p>GPIO 27 - State " + output27State + "</p>");
              // If the output27State is off, it displays the ON button
              if (output27State == "off")
              {
                client.println("<button onclick=\"StateChange()\" id=\"lock1\" value=\"Unlock\" class=\"button\">Unlock</button>");
              }
              else
              {
                client.println("<button onclick=\"StateChange()\" id=\"lock1\" value=\"Lock\" class=\"button button2\">Lock</button>");
              }
              client.println("</body></html>");

              // The HTTP response ends with another blank line
              client.println();
              // Break out of the while loop
              break;
            }
            if ((path[0] == "/lock") && (NArg == 3))
            {
              Serial.print("Enter to verify hmac");
              if (Hmac256Verify((uint8_t*)key, RandomNumberBuffer, path[2]) == 0)
              {
                Serial.print("Correct!");
              }
            }
            if ((path[0] == "/getvalue") && (NArg == 1))
            {
              //if (IsRandomSend == 0)
              //{
              RandomString = GetRandomString(RandomNumberBuffer, 32);
              Serial.println(RandomString);
              IsRandomSend = 1;

              Serial.println("Inside /getvalue");
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html; charset=utf-8");
              client.println("Content-Length: 64");
              client.println("Connection: close");
              client.println();

              client.print(RandomString);
              client.println();
              //}
              break;
            }
          }

          break;
          //}else {currentLine="";}
        } //else if(c !='\r'){currentLine+=c;}
        i++;
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  //GetRandomString(RandomNumberBuffer,32);
  //delayMicroseconds(1000000);
}
