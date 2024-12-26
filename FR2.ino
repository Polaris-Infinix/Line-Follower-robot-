// Sensor pins
int S1 =27 ;  // Leftmost sensor (GPIO 5)
int S2 = 35;  // Second from the left (GPIO 4)
int S3 = 34;   // Middle sensor (GPIO 0)
int S4 = 33;  // Second from the right (GPIO 2)
int S5 = 32;  // Rightmost sensor (GPIO 14)


// Motor pins
int RF = 23;  // Right forward (GPIO 15)
int RB = 22;  // Right backward (GPIO 16)
int LF = 19;  // Left forward (GPIO 12)
int LB = 18;  // Left backward (GPIO 13)
int LS=26;// Left Speed 
int RS=25;// Right Speed 
int forw_speed=80;
int turns_speed=70;
int hard_turn_speed=80;

int a1, a2, a3, a4, a5, b1, b2;

void updateSensors() {
  a1 = digitalRead(S1);
  a2 = digitalRead(S2);
  a3 = digitalRead(S3);
  a4 = digitalRead(S4);
  a5 = digitalRead(S5);
}

void speedDAC(int LSc=0, int RSc=0){
  dacWrite(LS,2.55*LSc);
  dacWrite(RS,2.55*RSc);
}
void forward(int speed) {
  digitalWrite(RF,HIGH);
  digitalWrite(LF,HIGH);
  speedDAC(speed,speed);
}
void backward(int speed) {
  digitalWrite(RB,HIGH);
  digitalWrite(LB,HIGH);
  speedDAC(speed,speed);
}

void left(int speed, int boole) {
  if (boole==0){
    speedDAC(speed,speed);
    digitalWrite(RF, HIGH);
    digitalWrite(LB, HIGH);
  }
  if(boole==1){
    speedDAC(speed,speed);
    digitalWrite(RF, HIGH);
    digitalWrite(LB,LOW);
  }
}
void right(int speed, int boole) {
  if(boole==0){
    digitalWrite(LF, HIGH);
    digitalWrite(RB, HIGH);
    speedDAC(speed,speed);
  }
  if(boole==1){
    digitalWrite(LF, HIGH);
    digitalWrite(RB,LOW);
    speedDAC(speed,speed);
  }
}

void turn_off() {
  digitalWrite(LB, LOW);
  digitalWrite(RB, LOW);
  digitalWrite(LF, LOW);
  digitalWrite(RF, LOW);
}


void setup() {
  Serial.begin(115200);
  
  // Set motor pins as OUTPUT
  pinMode(RF, OUTPUT);
  pinMode(RB, OUTPUT);
  pinMode(LF, OUTPUT);
  pinMode(LB, OUTPUT);
  pinMode(LS,OUTPUT);
  pinMode(RS,OUTPUT);
  
  // Set sensor pins as INPUT
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);  
  turn_off();
}

void loop() {
  // Read sensor values
  updateSensors();

  if(a3==0 && a2==0 && a4==0 && a5==1 && a1==1||a3==0 && a2==1 && a4==1){
    forward(forw_speed);
  }


  else if(a3==0 && a4==0 && a5==1 && a2==1||a4==0 && a5==1 && a3==1 && a2==1  ){// slight right 
    right(turns_speed,1);
    while(a3==0 && a4==0 && a5==1 && a2==1||a4==0 && a5==1 && a3==1 && a2==1 ){//false--a3==0 && a5==1 && a4==1||a2==0 && a4==0||a2==1 && a4==1
      updateSensors();
      }
    turn_off();
  }


  else if(a3==0 && a2==0 && a1==1 && a4==1|| a2==0&& a1==1 &&a3==1 && a4==1){//slight left
    left(turns_speed,1);
    while(a3==0 && a2==0 && a1==1 && a4==1|| a2==0&& a1==1 &&a3==1 && a4==1){// false-a3==0&& a1==1||a2==0 && a4==0||a2==1 && a4==1
    updateSensors();
    }
      turn_off();
  }

  else if(a5==0 && a3==1 &&  a2==1 ){//hard right 
    right(hard_turn_speed,0);
    while(a5==0 && a3==1 &&  a2==1 ){
      updateSensors();
    }  
    turn_off();
  }


  else if(a1==0 && a3==1 && a4==1){//hard left 
    left(hard_turn_speed,0);
    while(a1==0 && a3==1 && a4==1){
      updateSensors();
    }
    turn_off();
  }



  else if(a1==0 && a2==0 && a3==0 && a4==0 && a5==0){//intersection 
    forward(forw_speed);
    while(a1==0 && a2==0 && a3==0 && a4==0 && a5==0){
      updateSensors();
      }
    turn_off();
  }



  //Fail safe conditions 
  else if(a1==0 && a3==0 && a5==1||a3==0 && a5==0 && a1==1){ //90 degree 
    if(a1==0){
      left(hard_turn_speed,0);
    }
    else if(a5==0){
      right(hard_turn_speed,0);
    }
    while(a1==0 && a3==0 && a5==1||a3==0 && a5==0 && a1==1){
      updateSensors();

    }
    turn_off();
  }



  else if(a1==1 && a2==1 && a3==1 && a4==1 &&a5==1){//if nothing is detected 
    forward(70);
    while(a1==1 && a2==1 && a3==1 && a4==1 &&a5==1){
      updateSensors();
    }
    turn_off();

}
}