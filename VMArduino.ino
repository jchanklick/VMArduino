boolean go = true;
String themode = "postest";
int centeronshelf = 0;
int centermotor = 0;
int rrshelf = 0;
int rrmotor = 0;

int pin_red[] = {23, 25, 27, 29, 31, 33};
int pin_black[] = {22, 24, 26, 28, 30, 32, 34, 36};
int indshelf[] = {53, 51, 49, 47, 45};
int indmotor[] = {52, 50, 48, 46, 44, 42, 40, 38};

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  for (int i = 0; i < 8; i++){ 
    pinMode ( pin_black[i], OUTPUT);
    digitalWrite(pin_black[i], LOW);
    pinMode (indmotor[i], INPUT);
  }
  for (int i = 0; i < 6; i++){ 
    pinMode ( pin_red[i], OUTPUT);
    digitalWrite(pin_red[i], LOW);
    pinMode (indshelf[i], OUTPUT);
    digitalWrite(indshelf[i], LOW);
  }

  Serial.println(" ");
  Serial.println("Centering Sketch");

  delay(666);
}

void pinslow() {
  //Serial.println("Setting pins LOW: ");
  for (int i = 0; i < 8; i++){
    //Serial.print("pb.");
    //Serial.print(pin_black[i]);
    //Serial.print("  ");
    digitalWrite(pin_black[i], LOW);
    delay(1);
  }
  for (int i = 0; i < 6; i++){
    //Serial.print("pr.");
    //Serial.print(pin_red[i]);
    //Serial.print("  ");
    digitalWrite(pin_red[i], LOW);
    digitalWrite(indshelf[i], LOW);
    delay(1);
  }
  delay(22);
}
void pinshigh() {
  for (int i = 0; i < 8; i++){ 
    digitalWrite(pin_black[i], HIGH);
  }
  for (int i = 0; i < 6; i++){ 
    digitalWrite(pin_red[i], HIGH);
  }
}

boolean docenter(int shelf, int motor) {
  int motorpin = pin_black[motor];       //22
  int shelfpin = pin_red[shelf];        //23
  int indmotorpin = indmotor[motor];   //52
  int indshelfpin = indshelf[shelf];    //53
  int timeout = 9000;
  boolean indstate = false;
  boolean prestate = false;
  boolean laststate = false;
  boolean debounceset = false;
  boolean sawlow = false;
  boolean sawhigh = false;
  boolean centering = false;
  boolean centered = false;
  int debouncedelay = 66;
  unsigned long bouncedat = millis();
  digitalWrite(motorpin, LOW);
  digitalWrite(shelfpin, LOW);
  digitalWrite(indshelfpin, LOW);
  if(!centered) {
    Serial.println("Begin centering process");
    int timeoutcount = 0;
    while(!centered && (timeoutcount < timeout)) {
      if(!centering) {
        digitalWrite(indshelfpin, HIGH); //turn on 5 volt pin
        //turn on motor
        digitalWrite(motorpin, HIGH);
        digitalWrite(shelfpin, HIGH);
        centering = true;
      }
      prestate = digitalRead(indmotorpin);
      if(prestate != laststate) {
        if(!debounceset) {
          bouncedat = millis() + debouncedelay;
          debounceset = true;
        }
        if(millis() > bouncedat) {
          //still different
          indstate = prestate;
          laststate = indstate;
          debounceset = false;
        }
      } else {
        debounceset = false;
      }
      if(!indstate && !sawlow) {
        sawlow = true;
        Serial.println("indstate LOW");
      }
      if(sawlow) {
        if(indstate && !sawhigh) {
         sawhigh = true;
         Serial.println("indstate HIGH");
        }
        if(sawhigh) {
         if(!indstate) {
          digitalWrite(indshelfpin, LOW); //turn off 5 volt pin
          //it's low again, shut off motor!
          digitalWrite(motorpin, LOW);
          digitalWrite(shelfpin, LOW);
          centered = true;
          centering = false;
          Serial.println("CENTERED");
          return true;
         }
        }
      }
      timeoutcount++;
      delay(1);
    }
    if(timeoutcount >= timeout) {
      return false;
    }
  }
}

void rotateandread(int shelf, int motor, int readshelf, int readmotor) {
  Serial.println("Rotate and Read...");
  int motorpin = pin_black[motor];       //22
  int shelfpin = pin_red[shelf];        //23
  int indmotorpin = indmotor[readmotor];   //52
  int indshelfpin = indshelf[readshelf];    //53
  boolean indstate = false;
  boolean laststate = false;
  boolean prestate = false;
  boolean lastdbstate = false;
  boolean debounceset = false;
  int debouncedelay = 66;
  unsigned long bouncedat = millis();
  digitalWrite(motorpin, LOW);
  digitalWrite(shelfpin, LOW);
  digitalWrite(indshelfpin, HIGH);
  int timeout = 9000;
  int timeoutcount = 0;
  while(timeoutcount < timeout) {
    prestate = digitalRead(indmotorpin);
    if(prestate != lastdbstate) {
      if(!debounceset) {
        bouncedat = millis() + debouncedelay;
        debounceset = true;
      }
      if(millis() > bouncedat) {
        //still different
        indstate = prestate;
        lastdbstate = indstate;
        debounceset = false;
      }
    } else {
      debounceset = false;
    }
    if(indstate != laststate) {
      Serial.print("State Changed: ");
      Serial.println(indstate);
      laststate = indstate;
    }
    timeoutcount++;
    delay(1);
  }
  digitalWrite(indshelfpin, LOW);
  Serial.println("Finished Rotate and Read");
}

void stepsketch() {
  Serial.println("Step Mode...waiting a few seconds");
  delay(1000);
  Serial.println("Begin...");
  for (int i = 0; i < 6; i++){
    Serial.print("i: ");
    Serial.print(i);
    Serial.print(", pin_red[]: ");
    Serial.println(pin_red[i]);
    delay(500);
    for (int j = 0; j < 8; j++){ 
      digitalWrite(pin_red[i], HIGH);
      digitalWrite(pin_black[j], HIGH);
      Serial.print("j: ");
      Serial.print(j);
      Serial.print(", pin_black[]: ");
      Serial.println(pin_black[j]);
      delay(1000);
      pinslow();
    }
  }
}

void postest() {
  //like stepsketch, except only manipulates positive pins
  Serial.println("Positive Gate Test Mode...waiting");
  delay(1000);
  Serial.println("Begin...");
  for (int i = 0; i < 6; i++){
    Serial.print("i: ");
    Serial.print(i);
    Serial.print(", pin_red[]: ");
    Serial.println(pin_red[i]);
    delay(33);
    digitalWrite(pin_red[i], HIGH);
    delay(5555);
    pinslow();
  }
  pinslow();
  delay(666);
}

void specificpin() {
  pinslow();
  for (int i = 0; i < 6; i++){
    digitalWrite(pin_red[i], HIGH);
  }
  digitalWrite(pin_black[0], HIGH);
}

void loop() {
  if(go) {
    if(themode == "c") {
      boolean centerresult = docenter(centeronshelf, centermotor);
      Serial.print("Center Result: ");
      Serial.println(centerresult);
    } else if (themode == "r") {
      rotateandread(centeronshelf, centermotor, rrshelf, rrmotor);
    } else if (themode == "s") {
      stepsketch();
    } else if (themode == "a") {
      Serial.print("Pins High");
      pinshigh();
    } else if (themode == "sp") {
      specificpin();
    } else if (themode == "postest") {
      postest();
    }
    go = false;
  }
  delay(1);
}