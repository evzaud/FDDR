#include<LiquidCrystal.h>

//FingerDanceDanceRevolution

unsigned long prevMillis = 0;

boolean twoPlayer = false;

boolean player1Finished = false;

int player1Points = 0;

int player2Points = 0;

unsigned long curMillis = 0;

boolean endg = false;

boolean doubleLit = false;

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

int blueButtonPin = 3;
int yellowButtonPin = 4;
int redButtonPin = 6;

int turnCounter = 0;
int start = 0;

boolean once[3] = {false, false, false};

boolean notClicked[3] = {true, true, true};

boolean board[4][3] = {

  {false, false, false},

  {false, false, false},

  {false, false, false},

  {false, false, false}

};

int firstrow = A1;
int secondrow = 2;
int thirdrow = A3;
int fourthrow = 5;
int redcol = A2;
int bluecol = A0;
int yellowcol = A4;

int rowLED[4] = {firstrow, secondrow, thirdrow, fourthrow};

int columnLED[3] = {bluecol, yellowcol, redcol};




long delayVal = 0;

boolean hardMode = false;
boolean easyMode = false;

int totalPoints = 0;

void playGame() {
  lcd.print("2 Player:Red");
  lcd.setCursor(0, 1);
  lcd.print("1 Player:Yellow");


  while (digitalRead(yellowButtonPin) == LOW && digitalRead(redButtonPin) == LOW) {
    //donothing...assoonasbuttonispusheditwillexitoutofwhileloop
  }
  if (digitalRead(redButtonPin) == HIGH) {
    twoPlayer = true;
  }
  else {
    twoPlayer = false;
  }
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WhichLevel?Red");
  lcd.setCursor(0, 1);
  lcd.print("=HardBlue=Easy");
  //  delay(1000);
  // Serial.print(analogRead(redButtonPin) ); Serial.print("   "); Serial.println(analogRead(blueButtonPin) );
  //delay(1000);
  //Serial.print(analogRead(redButtonPin) ); Serial.print("   "); Serial.println(analogRead(blueButtonPin) );
  //delay(3000);
  while ((digitalRead(redButtonPin) == LOW) && (digitalRead(blueButtonPin) == LOW) ) {
    //donothing...assoonasbuttonispusheditwillexitoutofwhileloop
    Serial.print(analogRead(redButtonPin) ); Serial.print("   "); Serial.println(analogRead(blueButtonPin) );


  }
  if (digitalRead(redButtonPin) == HIGH) {
    hardMode = true;
  }
  else {
    easyMode = true;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TotalPoints:");
  lcd.setCursor(0, 1);
  lcd.print(totalPoints);
}


void addPoints() {

  totalPoints = totalPoints + 1;
  Serial.print("points  "); Serial.println(totalPoints);
  //  lcd.clear();
  //  lcd.setCursor(0, 0);
  //  lcd.print("TotalPoints:");
  //  lcd.setCursor(0, 1);
  //  lcd.print(totalPoints); lcd.print(" / "); lcd.print(turnCounter);
}
void deductPoints() {
  totalPoints = totalPoints - 1;
  Serial.print("lost points  "); Serial.println(totalPoints);
}

void turnLightsOff() {
  for (int  r = 0; r < 4; r++) {
    digitalWrite(rowLED, HIGH);
  }
  for (int  r = 0; r < 4; r++) {
    for (int  c = 0; c < 3; c++) {
      board[r][c] = false;
    }
  }
  
}

void setup() {
  //setuptheLCD'snumberofcolumnsandrows:
  Serial.begin(9600);
  lcd.begin(16, 2);

  pinMode(redButtonPin, INPUT);
  pinMode(blueButtonPin, INPUT);
  pinMode(yellowButtonPin, INPUT);
  pinMode(firstrow, OUTPUT);
  pinMode(secondrow, OUTPUT);
  pinMode(thirdrow, OUTPUT);
  pinMode(fourthrow, OUTPUT);
  pinMode(redcol, OUTPUT);
  pinMode(bluecol, OUTPUT);
  pinMode(yellowcol, OUTPUT);


  turnLightsOff();
  playGame();

  

  if (hardMode) {
    delayVal = 300;

  }
  else {
    delayVal = 1000;

  }


  prevMillis = millis();
  curMillis = prevMillis;
  randomSeed(analogRead(A5));


}

void loop() {
if (!endg){

  curMillis = millis();
  if (curMillis - prevMillis >= delayVal) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TotalPoints:");
    lcd.setCursor(0, 1);
    lcd.print(totalPoints); lcd.print(" / "); lcd.print(turnCounter);
    process();
    lightNewRow();
    printBoard();
    prevMillis = curMillis;
    start++;
    if ( start > 3){
      turnCounter = turnCounter + numLit();
    }  
    Serial.println();
    
  }

  if (digitalRead(redButtonPin) == HIGH) {
    if (once[2] && notClicked[2] && board[3][2]) {
      addPoints();
      notClicked[2] = false;
      once[2] = false;

    }
    else if (once[2]) {
      deductPoints();
      once[2] = false;
    }
  }

  if (digitalRead(yellowButtonPin) == HIGH) {
    if (once[1] && notClicked[1] && board[3][1]) {
      addPoints();
      Serial.println("  yellow  ");
      notClicked[1] = false;
      once[1] = false;

    }
    else if (once[1]) {
      deductPoints();
      once[1] = false;
    }
  }

  if (digitalRead(blueButtonPin) == HIGH) {
    if (once[0] && notClicked[0] && board[3][0]) {
      addPoints();
      Serial.println("  blue  ");
      notClicked[0] = false;
      once[0] = false;

    }
    else if (once[0]) {
      deductPoints();
      once[0] = false;
    }
  }

 turnOn();

  if (turnCounter > 150) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Your score: "); lcd.print(totalPoints); 
    delay(5000);
    lcd.setCursor(0, 1);
    if(!twoPlayer){
      endg = true;
    }
    else if(player1Finished){
      player2Points = totalPoints;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("1: "); lcd.print(player1Points); lcd.print(" 2: "); lcd.print(player2Points);
      lcd.setCursor(0,1);
      if(player1Points>player2Points){
        lcd.print("Player1 Wins!");  
        }
      else if(player2Points>player1Points){
        lcd.print("Player2 Wins!");  
        }
      else{
        lcd.print("Tie!");
      }
      endg = true;
    }
    else{
      player1Points = totalPoints;
      turnLightsOff();
      totalPoints = 0;
      start = 0; 
      turnCounter = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Player 2 turn");
      delay(3000);
      prevMillis = millis();
      player1Finished = true;
    } 
  } 
}



}

int numLit(){
  int counter = 0;
  for(int i=0; i<3; i++){
    if(board[3][i]){
      counter++;
    }
  }
  return counter;
}

void lightNewRow() {
  int  randNumber = (int)random(0, 3);
  board[0][randNumber] = true;
  randNumber = (int)random(0,2);
  if(hardMode && randNumber == 0){
    randNumber = (int)random(0, 3);
    board[0][randNumber] = true;
    randNumber = (int)random(0,3);
    if(randNumber == 0){
      board[0][randNumber] = true; 
    }
  }
}

void process() {
  doubleLit = false;
  for(int i =0; i<3; i++){
    notClicked[i] = true;
    once[i] = true;
  }
  for (int  r = 3; r > 0; r--) {
    for (int  c = 0; c < 3; c++) {
      board[r][c] = board[r - 1][c];

    }

  }
  for (int  c = 0; c < 3; c++) {
    board[0][c] = false;

  }
}

void printBoard() {
  for (int  r = 0; r < 4; r++) {
    for (int  c = 0; c < 3; c++) {
      Serial.print(board[r][c]);

    }
     Serial.println();


  }

}

void turnOn() {

  for (int  r = 0; r < 4; r++) {
    for (int  c = 0; c < 3; c++) {
      if (board[r][c]) {
        digitalWrite(rowLED[r], LOW);
        digitalWrite(columnLED[c], 1023);
        digitalWrite(rowLED[r], HIGH);
        digitalWrite(columnLED[c], 0);

      }
      else {
        digitalWrite(rowLED[r], HIGH);
        digitalWrite(columnLED[c], 0);

      }

    }

  }
}
