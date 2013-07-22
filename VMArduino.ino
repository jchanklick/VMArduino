boolean go = true;
String themode = "r";
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
  Serial.println(" ");
  Serial.println("Centering Sketch");
  delay(666);
}

// the loop routine runs over and over again forever:

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
  pinMode(indmotorpin, INPUT);
  pinMode(indshelfpin, OUTPUT);
  pinMode(motorpin, OUTPUT);
  pinMode(shelfpin, OUTPUT);
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
  pinMode(indmotorpin, INPUT);
  pinMode(indshelfpin, OUTPUT);
  pinMode(motorpin, OUTPUT);
  pinMode(shelfpin, OUTPUT);
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

void loop() {
  if(go) {
    if(themode == "c") {
      boolean centerresult = docenter(centeronshelf, centermotor);
      Serial.print("Center Result: ");
      Serial.println(centerresult);
    } else if (themode == "r") {
      rotateandread(centeronshelf, centermotor, rrshelf, rrmotor);
    }
    go = false;
  }
  delay(1);
}