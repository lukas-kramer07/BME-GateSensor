#define OnBoardLED 13
#define pingPin 7

void setup()
{
}
void loop()
{
    pinMode(OnBoardLED, HIGH);
    delay(1000);
    pinMode(OnBoardLED, LOW);
    delay(1000);
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
    pinMode(pingPin, OUTPUT);
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPin, LOW);

    pinMode(pingPin, INPUT);
    duration = pulseIn(pingPin, HIGH);

    cm = duration/34/2;
    return cm;
}