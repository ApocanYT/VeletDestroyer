//http://forum.arduino.cc/index.php?topic=396450
// Example 5 - Receive with start- and end-markers combined with parsing
#include <Servo.h>
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

// variables to hold the parsed data
char messageFromPC[numChars] = {0};
int alt_coord;
int ust_coord;
int komut;

Servo alt;
Servo ust;
int laser = 10;

boolean newData = false;

//============

void setup() {
    Serial.begin(9600);
    Serial.println("3 rakam bekleniyor: komut,  x koordinati ve y koordinati");
    Serial.println("Bu sekilde gonderilir <-1, 10, 15>  ");
    Serial.println();
    alt.attach(8);
    ust.attach(9);
    pinMode(laser, OUTPUT);
}

//============

void loop() {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        showParsedData();
        newData = false;
    }
}

//============

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    komut = atoi(strtokIndx); // copy it to messageFromPC
    
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    alt_coord = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    ust_coord = atoi(strtokIndx);     // convert this part to a float

}

//============

void showParsedData() {
    Serial.print("Komut ");
    Serial.println(komut);
    Serial.print("alt_coord ");
    Serial.println(alt_coord);
    Serial.print("ust_coord ");
    Serial.println(ust_coord);
    // Buraya servo hareket kodunu yazmalisiniz
    if (komut == 0){
      digitalWrite(laser, LOW);
    }
    else if (komut == -1){
       for (int i=0; i<5; i++){
                      digitalWrite(laser, HIGH);
                      delay(300);
                      digitalWrite(laser, LOW);
                      delay(300);
                    }
    }
    else {
            alt.write(alt_coord);
            ust.write(ust_coord);
            if (komut == 1) {
                digitalWrite(laser, HIGH);
            }
            else if (komut == 2) {
                  for (int i=0; i<5; i++){
                      digitalWrite(laser, HIGH);
                      delay(300);
                      digitalWrite(laser, LOW);
                      delay(300);
                    }
            }
    }
}
