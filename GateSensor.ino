#define OnBoardLED 13
#define pingPin 7
#define echoPin 6
int Reset;


void setup()
{
    pinMode(pingPin, OUTPUT);
    pinMode(echoPin, INPUT);
    Serial.begin(9600);
}
void loop()
{
    Serial.println(HH_MM_SS(millis()/1000));
    delay(100);
    Serial.println("Distanz: " + String(distanceCM()) + "cm");
}

String Status(){
    if(distanceCM() > 100){
        return "open";
    }
    else{
        return "closed";
    }
}
long distanceCM(){
    long duration,cm;

    // Trigger the ping
    
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPin, LOW);
    duration = pulseIn(echoPin, HIGH);

    cm = duration/34/2;
    return cm;
}
String HH_MM_SS(long sec_num){
    String h;
    String m;
    String s;

    long hours   = floor(sec_num / 3600);
    long minutes = floor((sec_num - (hours * 3600)) / 60);
    long seconds = sec_num - (hours * 3600) - (minutes * 60);

    if (hours   < 10) {
        h = "0"+ String(hours);
        }
    else{
        h = String(hours);
    }
    if (minutes < 10) {
        m = "0"+ String(minutes);
        }
    else{
        m = String(minutes);
    }
    if (seconds < 10) {
        String second = "0"+ String(seconds);
        }
    else{
        s = String(seconds);
    }
    if (s == ""){
        s = "00";
    }
    return h + ':' + m + ':' + s;
}