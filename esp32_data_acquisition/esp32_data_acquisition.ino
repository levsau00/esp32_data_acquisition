#include <SPIFFS.h>
#include <FS.h>

const int analogPin = 32;
const char* logFileName = "/sensor_log.txt";

int record_duration_millis;
int record_period = 1;
File logFile;


void setup() {
  Serial.begin(115200);
 
  if (SPIFFS.begin(true)) {
    Serial.println("SPIFFS mounted successfully.");
  } else {
    Serial.println("SPIFFS mount failed. Check your filesystem.");
  }

  deleteFile(SPIFFS, logFileName);
}

void loop() {

  if (Serial.available() > 0) {
    String command = Serial.readString();
    // command.trim();
    char cmd[command.length()];
    command.toCharArray(cmd,sizeof(cmd));
    char *tok = strtok(cmd, ", ");

    if(!strcmp("R",tok)) {
      tok = strtok(NULL, ", ");
      if (tok != NULL)
        if(record_duration_millis = String((const char *)tok).toInt()) {
          Serial.println(record_duration_millis);
          delay(1000);
          record_millis(record_duration_millis);
          readLogFile();
        }
    }

  }
}

void record_millis(int dur_millis) 
{
  int sensorData;
  int num_records = dur_millis;

  deleteFile(SPIFFS, logFileName);
  logFile = SPIFFS.open(logFileName, "a");
  // Serial.println(dur_millis);
  if (logFile) {
    int start_time = float(micros())/1000;
    // Append the sensor data and a timestamp to the log file
    logFile.println("Time(ms), Sensor Data");
    // for(int i = 0; i < num_records; i++)
    while(millis()-start_time < dur_millis)
    {
      sensorData = analogRead(analogPin);

      logFile.print(float(micros())/1000-start_time);
      logFile.print(",");
      logFile.print(sensorData);
      logFile.println(); // Start a new line
      // delay(record_period);
    }
    
  } else {
    Serial.println("Failed to open log file for writing.");
  }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}
void readLogFile() {
  File logFile = SPIFFS.open(logFileName, "r");
  if (logFile) {
    Serial.println("Log file contents:");
    while (logFile.available()) {
      Serial.write(logFile.read());
    }
    logFile.close();
  } else {
    Serial.println("Failed to open log file for reading.");
  }
}