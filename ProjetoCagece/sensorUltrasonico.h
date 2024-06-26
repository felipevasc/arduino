long getDistance(int TRIG_PIN, int ECHO_PIN) {
    

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);


    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH);
    long distance = (duration / 2) / 29.1;
    return distance;
}
