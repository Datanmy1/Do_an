#include <WiFi.h>

#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP32.h>  //IMPPORT LIBRARY FROM http://easycoding.tn/index.php/resources/

char Cayenneusername[] = "1d3582e0-24ff-11ed-bf0a-bb4ba43bd3f6";
char Cayennepassword[] = "500c2660d1ed84607a6fa3f67e484ff1dd1b7cb0";
char CayenneclientID[] = "4ed2a260-2838-11ed-baf6-35fab7fd0ac8";

#define pinDOC_PINLITHIUM 36
float gtcambien;
float vol_out;
float vol_in;
int pt;

void setup()
{
  Serial.begin(9600);

  WiFi.disconnect();
  delay(3000);
  Serial.println("START");
  WiFi.begin("Hoan Kim", "999999999");
  while ((!(WiFi.status() == WL_CONNECTED))) {
    delay(300);
    Serial.print("..");

  }
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP()));
  Cayenne.begin(Cayenneusername, Cayennepassword, CayenneclientID);
}

void loop()
{
  gtcambien = analogRead(pinDOC_PINLITHIUM);
  Serial.print("Cảm biến: "); Serial.println(gtcambien);

  vol_out = (gtcambien * 5) / 4096.0; // Chuyển từ analog sang V; 0-1023 ~ 0-5v4
  Serial.print("Volt out: "); Serial.println(vol_out);

  vol_in = float(vol_out / float(980.0 / float(10170.0 + 980.0))); // Tính vol nguồn cần đo
  Serial.print("Volt nguồn: "); Serial.println(vol_in);

  pt = (vol_in*100)/4.2;

    /*
       Thevenin Norton

       Vth = (Vcc x R2) / R1 + R2
       Vout = (Vin x R2) / R1 + R2 => Vin = Vout / (R2/(R1+R2));
    */

    Cayenne.virtualWrite(2, vol_in, "voltage", "v");
  Cayenne.virtualWrite(1, pt, "batt", "p");

  delay(5000);
}
