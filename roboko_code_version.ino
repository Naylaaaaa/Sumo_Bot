// motors pins
int motorright1 = 8;
int motorright2 = 9;
int enbmotorright = 10;
int motorleft1 = 5;
int motorleft2 = 3;
int enbmotorleft = 6;

//IR's pins
int front_r;
int front_l ;
int back_r ;
int back_l ;
//speed variable
int maxspeed = 255;
int spinspeed = 250;
int skipSpeed = 30;
int leftSpeed;
int rightSpeed;
int enaLeft;
int enaRight;

//Ultrasonic sensor pins
int TsonicR = 4;
int EsonicR = 7;
int TsonicM = 11;
int EsonicM = 13;
int TsonicB = A1;
int EsonicB = A4;


void setup() {
  // put your setup code here, to run once:

  pinMode(enbmotorright, OUTPUT);
  pinMode(motorright1, OUTPUT);
  pinMode(motorright2, OUTPUT);
  pinMode(enbmotorleft, OUTPUT);
  pinMode(motorleft1, OUTPUT);
  pinMode(motorleft2, OUTPUT);
  pinMode(front_r, INPUT);
  pinMode(front_l, INPUT);
  pinMode(back_r , INPUT);
  pinMode(back_l , INPUT);
  pinMode(EsonicR, INPUT);
  pinMode(EsonicM, INPUT);
  pinMode(EsonicB , INPUT);
  pinMode(TsonicR, OUTPUT);
  pinMode(TsonicM, OUTPUT);
  pinMode(TsonicB, OUTPUT);
  Serial.begin(9600);
  stoop();
  delay(4700); //to stop and test it


}



long readUltrasonic(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  return pulseIn(echoPin, HIGH, 30000) * 0.034 / 2; // Convert duration to cm
}


}
void readIR() {
  front_r = analogRead(A3);//most right
  front_l = analogRead(A2);
  back_r = digitalRead(12);
  back_l = digitalRead(2);
  front_r = (front_r < 100 ) ? 1 : 0;  //REVERSED LOGIC //0 IS BLACK 1 IS WHITE
  front_l = (front_l < 50) ? 1 : 0;

}



void moveForward() {
  analogWrite(enbmotorright, maxspeed); //RIGHTMOTOR
  digitalWrite(motorright1, LOW );
  digitalWrite(motorright2, HIGH ) ;

  analogWrite(enbmotorleft, maxspeed);
  digitalWrite(motorleft1, LOW);
  digitalWrite(motorleft2, HIGH);

}

void turnRight() {

  analogWrite(enbmotorright, 0);  //RIGHTMOTOR
  digitalWrite(motorright1, LOW);
  digitalWrite(motorright2, HIGH);

  analogWrite(enbmotorleft, maxspeed);
  digitalWrite(motorleft1, LOW);
  digitalWrite(motorleft2, HIGH);
}
void moveBackward() {
  analogWrite(enbmotorright, maxspeed);//RIGHTMOTOR
  digitalWrite(motorright1, HIGH );
  digitalWrite(motorright2, LOW ) ;

  analogWrite(enbmotorleft, maxspeed);
  digitalWrite(motorleft1, HIGH);
  digitalWrite(motorleft2, LOW);
  // delay(1000);
}
void stoop() {
  analogWrite(enbmotorright, 0);//RIGHTMOTOR
  analogWrite(enbmotorleft, 0);//RIGHTMOTOR
  delay(500);
}
void spin()
{

  analogWrite(enbmotorright, spinspeed);//RIGHTMOTOR
  digitalWrite(motorright1, LOW );
  digitalWrite(motorright2, HIGH ) ;

  analogWrite(enbmotorleft, spinspeed);
  digitalWrite(motorleft1, HIGH);
  digitalWrite(motorleft2, LOW);


}
void turnSkip()
{

  analogWrite(enbmotorright, spinspeed);//RIGHTMOTOR
  digitalWrite(motorright1, LOW );
  digitalWrite(motorright2, HIGH ) ;

  analogWrite(enbmotorleft, skipSpeed);
  digitalWrite(motorleft1, LOW);
  digitalWrite(motorleft2, HIGH);


}


void spinAndSearch() {
  //Serial.println("IM searching");
  long startTime = millis(); // Timeout to avoid spinning forever
  while (millis() - startTime < 3000) { // Spin for a maximum of 5 seconds
    readIR();         // Continuously check IR sensors
    distanceR = readUltrasonic(TsonicR, EsonicR);
    distanceM = readUltrasonic(TsonicM, EsonicM);
    distanceL = readUltrasonic(TsonicL, EsonicL);

    if (front_r || front_l) {
      moveBackward();
      delay(600);//to be changed experimentally
      spin();
      delay(1000);//to be changed experimentally
      moveForward();
    }
    if (distanceB < 30) {
      Serial.println(" opponent detected back ");
      turnSkip();
      delay (400);
      do {
        Serial.println(" opponent detected back ");
        turnSkip();
        distanceM = readUltrasonic(TsonicM, EsonicM);

      } while (distanceM > 40);
      return;
    }
    if (distanceM < 40) {
      Serial.println(" opponent detected middle ");
      moveForward();
      return;
    }
    if (distanceR < 40) {
      Serial.println(" opponent detected right ");
      turnRight();
      delay(800);
      return;
    }
    spin(); // Keep spinning to search
    Serial.println("no opponent is detected");
  }

  // If no opponent found after timeout, move forward
  moveForward();
  delay(3000); // Move forward for a bit before spinning again
}

void attack() {
  Serial.println("attacking");
  moveForward();

}

void loop() {

  readIR();         // Check IR sensors to stay inside the ring
  distanceR = readUltrasonic(TsonicR, EsonicR);
  distanceM = readUltrasonic(TsonicM, EsonicM);
  distanceL = readUltrasonic(TsonicL, EsonicL);

  // Check if the robot is near the boundary
  if (front_r || front_l) {//1 for white
    moveBackward();
    delay(600);
    spin();
    delay(1000);
    moveForward();
    //delay(1000);
  }
  else if (!back_r || !back_l)//0 for white
  {
    moveForward();

  }

  if (distanceM > 40) {

    spinAndSearch();
  }

  else {
    // Opponent detected, attack!

    attack();
  }

}
