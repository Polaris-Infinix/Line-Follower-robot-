// Sensor pins
int S1 =27 ;  // Leftmost sensor (GPIO 5)
int S2 = 26;  // Second from the left (GPIO 4)
int S3 = 25;  // Middle sensor (GPIO 0)
int S4 = 33;  // Second from the right (GPIO 2)
int S5 = 32;  // Rightmost sensor (GPIO 14)


// Motor pins
int RF = 23;  // Right forward (GPIO 15)
int RB = 22;  // Right backward (GPIO 16)
int LF = 19;  // Left forward (GPIO 12)
int LB = 18;  // Left backward (GPIO 13)

int forw_speed=60;

int a1, a2, a3, a4, a5, b1, b2;

void updateSensors() {
  a1 = digitalRead(S1);
  a2 = digitalRead(S2);
  a3 = digitalRead(S3);
  a4 = digitalRead(S4);
  a5 = digitalRead(S5);
}

void forward(int speed) {
  analogWrite(RF,2.55*speed);
  analogWrite(LF,2.55*speed);
}
void backward(int speed) {
  analogWrite(RB,2.55*speed);
  analogWrite(LB,2.55*speed);
}

void left(int speed, int boole) {
  if (boole==0){
    analogWrite(RF, 2.55*speed);
    analogWrite(LB, 2.55*speed);
  }
  if(boole==1){
    analogWrite(RF, 2.55*speed);
    analogWrite(LB,0);
  }
}
void right(int speed, int boole) {
  if(boole==0){
    analogWrite(LF, 2.55*speed);
    analogWrite(RB, 2.55*speed);
  }
  if(boole==1){
    analogWrite(LF, 2.55*speed);
    analogWrite(RB,0);
  }
}

void turn_off() {
  analogWrite(LB, 0);
  analogWrite(RB, 0);
  analogWrite(LF, 0);
  analogWrite(RF, 0);
}


void setup() {
  Serial.begin(115200);
  
  // Set motor pins as OUTPUT
  pinMode(RF, OUTPUT);
  pinMode(RB, OUTPUT);
  pinMode(LF, OUTPUT);
  pinMode(LB, OUTPUT);
  
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
  a1 = digitalRead(S1);
  a2 = digitalRead(S2);
  a3 = digitalRead(S3);
  a4 = digitalRead(S4);
  a5 = digitalRead(S5);

  if(a3==0 && a2==0 && a4==0 && a5==1 && a1==1||a3==0 && a2==1 && a4==1){
    forward(forw_speed);
  }


  else if(a3==0 && a4==0 && a5==1 && a2==1||a4==0 && a5==1 && a3==1 && a2==1  ){// slight right 
    right(35,1);
    while(a3==0 && a4==0 && a5==1 && a2==1||a4==0 && a5==1 && a3==1 && a2==1 ){//false--a3==0 && a5==1 && a4==1||a2==0 && a4==0||a2==1 && a4==1
      updateSensors();
      }
    turn_off();
  }


  else if(a3==0 && a2==0 && a1==1 && a4==1|| a2==0&& a1==1 &&a3==1 && a4==1){//slight left
    left(35,1);
    while(a3==0 && a2==0 && a1==1 && a4==1|| a2==0&& a1==1 &&a3==1 && a4==1){// false-a3==0&& a1==1||a2==0 && a4==0||a2==1 && a4==1
    updateSensors();
    }
      turn_off();
  }

  else if(a5==0 && a3==1 &&  a2==1 ){//hard right 
    right(35,0);
    while(a5==0 && a3==1 &&  a2==1 ){
      updateSensors();
    }  
    turn_off();
  }


  else if(a1==0 && a3==1 && a4==1){//hard left 
    left(40,0);
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
      left(40,0);
    }
    else if(a5==0){
      right(40,0);
    }
    while(a1==0 && a3==0 && a5==1||a3==0 && a5==0 && a1==1){
      updateSensors();

    }
    turn_off();
  }



  else if(a1==1 && a2==1 && a3==1 && a4==1 &&a5==1){//if nothing is detected 
    forward(30);
    while(a1==1 && a2==1 && a3==1 && a4==1 &&a5==1){
      updateSensors();
    }
    turn_off();

}
}