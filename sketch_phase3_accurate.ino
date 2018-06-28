
// Setup Motor A (front and rear) pins
#define PIN_R1 5
#define PIN_R2 6
#define PIN_R_enable 3

// Setup Motor B (front and rear) pins
#define PIN_L1 9
#define PIN_L2 10
#define PIN_L_enable 11


// Setup Bluetooth
#define TX 1
#define RX 0


unsigned long timer0 = 2000;  //Stores the time (in millis since execution started)
unsigned long timer1 = 0;  //Stores the time when the last command was received from the phone

// Define Attributes
#define NO_COMMAND 'X'
#define RESET 'C'

#define FORWARD 'F'
#define FORWARD_RIGHT 'I'
#define RIGHT 'R'
#define BACKWORD_RIGHT 'J'
#define BACKWORD 'B'
#define BACKWORD_LEFT 'H'
#define LEFT 'L'
#define FORWARD_LEFT 'G'

#define BREAK ' '
#define FOLLOWLINE 'W'
#define STOPFOLLOWING 'w'


#define STOP 'S'
#define GEAR_1 '0'
#define GEAR_2 '3'
#define GEAR_3 '5'
#define GEAR_4 '8'
#define GEAR_5 'q'

char prevCommand = NO_COMMAND;
char command  = RESET;

int speed = 0;
char state = STOP;

void setup() {
  // The setup code goes here and runs once only

  Serial.begin(9600);

  // Configure the pin modes for each drive motor
  pinMode (PIN_R1, OUTPUT);
  pinMode (PIN_R2, OUTPUT);
  pinMode (PIN_L1, OUTPUT);
  pinMode (PIN_L2, OUTPUT);
  pinMode (PIN_R_enable, OUTPUT);
  pinMode (PIN_L_enable, OUTPUT);
  digitalWrite (PIN_L_enable, HIGH);
  digitalWrite (PIN_R_enable, HIGH);
  pinMode (r_sensor, INPUT);
  pinMode (m_sensor, INPUT);
  pinMode (l_sensor, INPUT);
  

}

void loop() {
 // Main code goes here and will run repeatedly:
 //all distamce in mm 
 //in a rectangle
  move_forward(2000);
  rotate_right(90);  //degrees
  
  move_forward(2000);
  rotate_right(90);
  
  move_forward(2000);
  rotate_right(90);
  
  move_forward(2000);
  rotate_right(90);

  //in a circle
  move_inacircle(1000,1); //radius & n of turns

  //infinity
  move_forward(1000);
  rotate_right(90);
  
  move_inacircle(1000,0.25);
  rotate_right(90);
  
  move_forward(2000);
  rotate_left(90);
  
  move_inacircle(1000,0.25);
  rotate_left(90);

  move_forward(1000);
  
  
  if (Serial.available()) {
    
    delay(10); //Delay added to make thing stable

    timer1 = millis();
    command = Serial.read();
    Serial.println(command);

    //Change pin mode only if new command is different from previous.
    if (command != prevCommand) {
      setState(command);
    }
  }

  else {
    timer0 = millis();  //Get the current time (millis since execution started).
    //Check if it has been 60000ms since we received last command.
    if ((timer0 - timer1) > 60000) {
      //More tan 60000ms have passed since last command received, car is out of range.
      //Therefore stop the car.
      breakRobot(1);
      coast(0);
    }
  }

}

void setSpeed(char gear) {
  if (gear == GEAR_1) {
    speed = 100;
  }  else if (gear == GEAR_2) {
    speed = 140;
  }  else if (gear == GEAR_3) {
    speed = 180;
  }  else if (gear == GEAR_4) {
    speed = 220;
  } else if (gear == GEAR_5) {
    speed = 255;
  }
}

void setState(char command) {
  switch (command) {
    case RESET:
      breakRobot(1);
      coast(0);
      speed = 0;
      state = STOP;
      break;
    case BREAK:
      state = BREAK;
      breakRobot(1);
      break;
    case STOP:
      state = STOP;
      coast(0);
      break;
    case GEAR_1:
      setSpeed(GEAR_1);
      setState(state);
      break;
    case GEAR_2:
      setSpeed(GEAR_2);
      setState(state);
      break;
    case GEAR_3:
      setSpeed(GEAR_3);
      setState(state);
      break;
    case GEAR_4:
      setSpeed(GEAR_4);
      setState(state);
      break;
    case GEAR_5:
      setSpeed(GEAR_5);
      setState(state);
      break;
    case FORWARD:
      state = FORWARD;
      forward(1, speed);
      break;
    case FORWARD_RIGHT:
      state = FORWARD_RIGHT;
      right(1, 0.5 * speed, speed);
      break;
    case RIGHT:
      state = RIGHT;
      right(1, speed, speed);
      break;
    case BACKWORD_RIGHT:
      state = BACKWORD_RIGHT;
      left(1, 0.5 * speed, speed);
      break;
    case BACKWORD:
      state = BACKWORD;
      backward(1, speed);
      break;
    case BACKWORD_LEFT:
      state = BACKWORD_LEFT;
      right(1, speed, 0.5 * speed);
      break;
    case LEFT:
      state = LEFT;
      left(1, speed, speed);
      break;
    case FORWARD_LEFT:
      state = FORWARD_LEFT;
      left(1, speed, 0.5 * speed);

      
  }
  prevCommand = command;
}

// Create motor functions

void motorAforward(int speed) {
  analogWrite (PIN_R1, speed);
  digitalWrite (PIN_R2, LOW);
}
void motorBforward(int speed) {
  digitalWrite (PIN_L1, LOW);
  analogWrite (PIN_L2, speed);
}
void motorAbackward(int speed) {
  digitalWrite (PIN_R1, LOW);
  analogWrite (PIN_R2, speed);
}
void motorBbackward(int speed) {
  analogWrite (PIN_L1, speed);
  digitalWrite (PIN_L2, LOW);
}
void motorAstop() {
  digitalWrite (PIN_R1, HIGH);
  digitalWrite (PIN_R2, HIGH);
}
void motorBstop() {
  digitalWrite (PIN_L1, HIGH);
  digitalWrite (PIN_L2, HIGH);
}
void motorAcoast() {
  digitalWrite (PIN_R1, LOW);
  digitalWrite (PIN_R2, LOW);
}
void motorBcoast() {
  digitalWrite (PIN_L1, LOW);
  digitalWrite (PIN_L2, LOW);
}

// Setup movement functions

void forward (int duration, int speed) {
  motorAforward(speed);
  motorBforward(speed);
  delay (duration);
}
void backward (int duration, int speed) {
  motorAbackward(speed);
  motorBbackward(speed);
  delay (duration);
}
void left (int duration, int speedA, int speedB) {
  motorAbackward(speedA);
  motorBforward(speedB);
  delay (duration);
}
void right (int duration, int speedA, int speedB) {
  motorAforward(speedA);
  motorBbackward(speedB);
  delay (duration);
}
void coast (int duration) {
  motorAcoast();
  motorBcoast();
  delay (duration);
}
void breakRobot (int duration) {
  motorAstop();
  motorBstop();
  delay (duration);
}
void move_inacircle(int radius_mm,float nofturns){ //radius should be in mm
  int d_btw_wheels=  ;          //distance between the right & the left wheels in mm.
  int w_wheel=       ;          //width of the wheel in mm.
  int mm_ber_sec =1050  ;
  int distanceA = radius_mm ;      //should be 2*pi*radius but we only need the ratio so it's ok
  int distanceB = (radius_mm - d_btw_wheels - w_wheel) ;
  float t = (1000*nofturns*(distanceA / mm_ber_sec)) ;
  right(t,255,(255*(distanceA/distanceB)));
}
void move_forward (int distance_mm){
  int mm_ber_sec =1050  ;
  float t= 1000*(distance_mm / mm_ber_sec)  ;
  forward(t,255)  ;
  
}
void rotate_left (int degree){
  int deg_ber_sec =  ;
  float t= 1000*(degree / deg_ber_sec);
  left(t,255,255)
}
void rotate_right (int degree){
  int deg_ber_sec =  ;
  float t= 1000*(degree / deg_ber_sec);
  right(t,255,255)
}
