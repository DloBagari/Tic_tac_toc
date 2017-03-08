//since we are doing c++ we have to import each header we will needed 
//becouse the editor will not include anything 
//in normal shetch of this editor the editor will aautomaticly include the 
//header Arduino.h but with c++ does not include anything

#include <Arduino.h>
#include <ctype.h>
//this header contains toupper() function ehich we will needed
#include <telegraph.h>
//this to make our class declaration and its responding function 
//declaration available

//declaring two arrays for morse codes
//letter A-E, F-J ....
const char* LETTERS[] = {
  ".-","-...", "-.-.","-..",".", 
  "..-.", "--.", "....","..",".---",
  "-.-", ".-..", "--","-.","---",
  ".--.", "--.-", ".-.","...","-",
  "..-", "...-", ".--", "-..-", "-.--",
  "--.." };
  

//digits
const char* DIGITS[] = {
  "-----", ".----", "..---", "...--",
  "....-", ".....", "-....", "--...",
  "---..", "----."};
  

//implementing the constructor
Telegraph::Telegraph(const int output_pin, const int dit_length) {
  _output_pin = output_pin;
  _dit_length = dit_length;
  _dah_length = dit_length * 3;
  pinMode(_output_pin, OUTPUT);
}

//implements output_code
void Telegraph::output_code(const char* code) {
  const unsigned int conde_length = strlen(code);
  for (unsigned int i = 0; i < code_length; i++) {
    if (code[i] == '.') {
      dit();
    }else{
      dah();
    }
    //if it is not end of the code make delay 
    if (i != (code_length -1){
      delay(_dit_length);
    }
  }
}

void Telegraph::dit() {
  Serial.print(".");
  output_symbol(_dit_length);
}

void Telegraph::dah() {
  Serial.print("-");
  output_symbol(_dah_length);
}

void Telegraph::output_symbol(const int length) {
  digitalWrite(_output_pin, HIGH);
  delay(length);
  digitalWrite(_output_pin, LOW);
}


//implements the send_message()
void Telegraph::send_message(const char* message) {
  for (unsigned int i = 0; i < strlen(message); i++) {
    const char current_char = toupper(message[i]);
    if (isalpha(current_char)){
      output_code(LETTERS[current_char - 'A']);
      delay(_dah_length);
    }else if (isdigit(current_char)) {
      output_code(DIGITS[current_char - '0']);
      delay(_dah_length);
    }else if (current_char == ' ') {
      Serial.print(" ");
      delay(_dit_length * 7);
    }
  }
  Serial.println();
}

      
  
  
  


