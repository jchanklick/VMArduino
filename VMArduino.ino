boolean go = true;

int pin_centers[] = {false, false, false, false, false, false, false, false};
int pin_centering[] = {false, false, false, false, false, false, false, false};
int pin_black[] = {22, 24, 26, 28, 30, 32, 34, 36};
int pin_red[] = {23, 25, 27, 29, 31, 33};

int indshelf[] = {53, 51, 49, 47, 45};
int indmotor[] = {52, 50, 48, 46, 44, 42, 40, 38};

int indshelfpin = 53;
int indmotorpin = 52;
int motorpin = 22;
int shelfpin = 23;

boolean woky = false;
boolean sawlow = false;
boolean sawhigh = false;
boolean centering = false;
boolean centered = false;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(indmotorpin, INPUT);
  pinMode(indshelfpin, OUTPUT);
  
  pinMode(motorpin, OUTPUT);
  pinMode(shelfpin, OUTPUT);
  
  digitalWrite(motorpin, LOW);
  digitalWrite(shelfpin, LOW);
  digitalWrite(indshelfpin, LOW);
  
  Serial.println("Centering Sketch");
}

// the loop routine runs over and over again forever:

void wicky(int target) {
  if(!centered) {
    if(!centering) {
      Serial.println("Begin centering process");
      digitalWrite(indshelfpin, HIGH); //turn on 5 volt pin
      //turn on motor
      digitalWrite(motorpin, HIGH);
      digitalWrite(shelfpin, HIGH);
      centering = true;
    }
    
    woky = digitalRead(indmotorpin); //start reading the low pin
    if(!woky && !sawlow) {
      sawlow = true;
    }
    if(sawlow) {
      if(woky && !sawhigh) {
       sawhigh = true;
      }
      if(sawhigh) {
       if(!woky) {
         digitalWrite(indshelfpin, LOW); //turn off 5 volt pin
        //it's low again, shut off motor!
        digitalWrite(motorpin, LOW);
        digitalWrite(shelfpin, LOW);
        centered = true;
        centering = false;
        Serial.println("CENTERED");
       }
      } 
    }
  }
  
}

void loop() {
  if(go) {
    wicky();
  }
  delay(1);
}



