#include <OneWire.h> 
#include <DallasTemperature.h>
#include <Ethernet.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#define ONE_WIRE_BUS 31 

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
EthernetClient client;

// MYSQL
IPAddress server_addr(10,0,0,238); 
char user[] = "";         
char password[] = "";        
MySQL_Connection conn((Client *)&client);

char INSERT_DATA[] = "INSERT INTO pederyodb.temp (verdi) VALUES (%s)";
char query[128];

char timestamp[20];
char temperatur[5];

void setup(void) 
{ 
 // start serial port 
 Serial.begin(115200); 
 Ethernet.begin(mac_addr);
 Serial.println("Connecting...");
  if (conn.connect(server_addr, 3306, user, password)) {
    delay(1000);
    sensors.begin();   
    sensors.requestTemperatures();
    Serial.println(sensors.getTempCByIndex(0));
  }
  else
    Serial.println("Connection failed.");
  //conn.close();
} 

void loop(void) { 
  double temp = hentTemp();
  kjorSql(temp);
  delay(30000);
}

double hentTemp(){
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

void kjorSql(double temp) {
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  dtostrf(temp, 2, 2, temperatur);
  sprintf(query, INSERT_DATA, temperatur);
  cur_mem->execute(query);
  delete cur_mem;
}
