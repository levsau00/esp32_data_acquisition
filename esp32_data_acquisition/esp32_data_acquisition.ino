const int analogPin = 32;

int record_duration_millis;
int record_period = 1;


void setup() {
  Serial.begin(115200);
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
          // Serial.println(record_duration_millis);
          delay(500);
          record_millis(record_duration_millis);
        }
    }
  }
}

void record_millis(int dur_millis) 
{
  int sensorData;
  int num_records = dur_millis;

  // Serial.println(dur_millis);
  int start_time = float(micros())/1000;
  // Append the sensor data and a timestamp to the log file
  Serial.println("Time(ms), Sensor Data");
  // for(int i = 0; i < num_records; i++)
  while(float(micros())/1000-start_time < dur_millis)
  {
    sensorData = analogRead(analogPin);

    Serial.print(float(micros())/1000-start_time);
    Serial.print(",");
    Serial.print(sensorData);
    Serial.println("\0");
    // delay(record_period);
  }
}