#include <IRremote.h>
#include <LiquidCrystal.h>
#define P1TURN "O TURN "
#define P2TURN "X TURN "
#define DRAW   "GAME OVER DRAW"
#define BAUD_RATE 9600
#define P1SHIFT 4
#define P2SHIFT 5
#define P1 0
#define P2 1

int RECV_PIN = A5;
IRrecv irrecv(RECV_PIN);
decode_results results;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

byte ground[3]   = {B110, B101, B011};//ground cases
int reserved[2] = {0, 0};
int winCases[8] = {0x7, 0x38, 0x1c0, 0x49, 0x92, 0x124, 0x111, 0x54}; //wincases
byte whoIsTurn = 1; // which player has to play in next rotation
char statuss[16];// string to save the status of the game when the game is over
byte gameOver = 0;
unsigned long newInput = 0;

//declare reset function @ address 0
void (* resetFunc)(void) = 0;//The function starts to execute at addres 0


void setup() {
  DDRB  = B11111111;//set all pins in DDRB as output
  PORTB = B00000000; 
  DDRC  = PORTC = B000111; //enable pull ups on pins 1, 2, 3
  Serial.begin(BAUD_RATE); // start Serial 
  irrecv.enableIRIn(); // Start the receiver
  lcd.begin(16, 2);
}

//reseting the micorColtoller and Processing 
void resetProcessing() {
    PORTC = B000000;
    Serial.write(61); //set Processing backround color to red
    PORTB = B010101; //display red LEDS
    delay(300);
    PORTB = B101010;// display green LEDS
    Serial.write(62);//set Processing backgound color to green
    delay(300);
    PORTB = B111111;// display all LEDS
    Serial.write(60);//set Processing backgound color to white
    delay(200);
    resetFunc();
 
}

//convert player input to a singl digit
byte getButtonInfo(unsigned long buttonValue){
  switch(buttonValue) {
    case 0x9716BE3F : return 1;
    case 0x3D9AE3F7 : return 2;
    case 0x6182021B : return 3;
    case 0x8C22657B : return 4;
    case 0x488F3CBB : return 5;
    case 0x449E79F  : return 6;
    case 0x32C6FDF7 : return 7;
    case 0x1BC0157B : return 8;
    case 0x3EC3FC1B : return 9;
    case 10:
    case 0x20FE4DBB : resetProcessing(); //this case is to reset the microContoller and Processing
  }
  return 0;
}
 
//convert player input to coresspinding cell number and reserve that cell     
byte OnButtonPressed(unsigned long buttonValue, byte player) {
   //if player's input comes from IR Remote the method getButtonInfo will be called
   byte button = (buttonValue > 9 )? getButtonInfo(buttonValue) : buttonValue;
   if (button) {
     int one = 1;
     int wantedBit = one << (button - 1);// which bit the player wants to reserve
     
     //if both player have not reserved wantedBit, the player can reserve that bit
     if (!((reserved[0] & wantedBit) || (reserved[1] & wantedBit))) {
       reserved[player - 1] |= wantedBit; //write wantedBit to player's storage.
       // notify Serial which cell is reserved by which player.
       (player == 1)? Serial.write(button) : Serial.write(button + 10);
       return 1;
     }
   }
  return 0; 
}

//compare the player's storage with each win case, if there is match
//send a notification to Serial
byte isWon(byte player) {
  for (byte i = 0; i < 8 ; i++) {
    if ((reserved[player] & winCases[i]) == winCases[i]) {
      reserved[player] = winCases[i];
      reserved[whoIsTurn % 2] = 0;
      Serial.write(200 + i); //write to Serial the win case 
      return 1;
    }
  }
  return 0;
}


byte isFull() {
  //return Non-zero Value if all cells are reserved and the board is full
  return ((reserved[0] | reserved[1]) == 0x1ff);
}

void oneLedOn(byte player, byte targetBit, byte shiftAmount, byte _delay) {
         byte bitOn = (reserved[player] >> targetBit) & B1;// read the targetBit from player's storage
         if(bitOn){                                        
          byte col = targetBit % 3; //targetBit is in which column
          // PORTB = 1 * ( shiftAmount - ( col * 2)) ;
          PORTB = 1 << (shiftAmount - (col << 1)); // make the corresponding pin high in PORTB
          //targetBit / 3 : targetBit in which row
          PORTC = ground[targetBit / 3]; //make the corresponding pin low in PORTC
          delay(_delay);
          PORTB = B00000000; // reset PORTB 
          PORTC = B000111; //reset PORTC
         }
}
  
void ledsOn(byte _delay) {
    //for each bit in each player's storage if the bit is on display the corresponding LED on
    for ( byte i = 0 ; i < 9; i++) {
         oneLedOn(P1, i, P1SHIFT, _delay); // for player 1
         oneLedOn(P2, i, P2SHIFT, _delay); // for player 2
  }
}
   

void loop() {
   //listen for reset request
   if (irrecv.decode(&results) || Serial.available()) {
     if (results.value == 0x20FE4DBB || Serial.read() == 10) {
        resetProcessing();
     }
     irrecv.resume();
   }
   lcd.setCursor(0, 0);//set coursor at row 0 colum 0
   lcd.print(statuss);//display the game status on LCD
   while(whoIsTurn) {
     newInput = 0;
     lcd.setCursor(0, 0);//set coursor at row 0 colum 0
     (whoIsTurn == 1)? lcd.print(P1TURN):lcd.print(P2TURN); //display on LCD which layer has to play
     //if player input from both input methods then ignore this request
     if ( irrecv.decode(&results) && Serial.available()) {
       //empty the serial buffer
        while (Serial.available()) {
          Serial.read();   
        }
       results.value = 0;
       irrecv.resume();
       continue;
     }
     //player input from IR Remote
     else if (irrecv.decode(&results)) {
         newInput = results.value;
         irrecv.resume();
     }
     
     //player input from mouseclick on Porcessing
     else if( Serial.available()) {
        newInput = Serial.read();
     } 
     
     if (newInput && OnButtonPressed(newInput, whoIsTurn) ) {
        if (isWon(whoIsTurn - 1)) {
          // status = which player has won the game
          (whoIsTurn == 1)? sprintf(statuss, "%s won !", "O"): sprintf(statuss, "%s won !", "X"); 
          whoIsTurn = 0;
        }
        else if (isFull()) {
          // status = draw
          sprintf(statuss, "%s", DRAW);
          reserved[0] = reserved[1] = 0x1ff;
          whoIsTurn = 0;
        }
        else{
        (whoIsTurn == 1)? whoIsTurn = 2 :  whoIsTurn = 1;
        }
      }       
    ledsOn(1);    
   }
  ledsOn(15);
}
