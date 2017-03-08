import processing.serial.*;
Serial myPort;
PImage imgO;
PImage imgX;
PFont f;

void redraw(int c){
  switch(c){
    case 60: background(255, 255, 255);
             break;
    case 61: background(255,0,0);
             break;
    case 62: background(0,255,0);
             break;

  }

  stroke(128,128,128);
  strokeWeight(4);
  line(200,50,200,360);
  line(400,50,400,360);
  line(100,150,500,150);
  line(100,260,500,260);
}


void setup() {
  myPort = new Serial(this, Serial.list()[0], 9600);
  size(640, 400);
  redraw(60);
  imgO = loadImage("o.gif");
  imgX = loadImage("x.gif");
  f = createFont("Arial",16,true);


   
}

int fillSquare(int s){
  switch (s){
     case 1: image(imgO,100,45);  break;
     case 11: image(imgX,100,45); break;
     case 2: image(imgO,260,45);  break;
     case 12: image(imgX,260,45); break;
     case 3: image(imgO,420,45);  break;
     case 13: image(imgX,420,45); break;
     case 4: image(imgO,100,155); break;
     case 14: image(imgX,100,155);break;
     case 5:  image(imgO,260,155);break;
     case 15: image(imgX,260,155);break;
     case 6: image(imgO,420,155); break;
     case 16: image(imgX,420,155);break;
     case 7: image(imgO,100,265); break;
     case 17: image(imgX,100,265);break;
     case 8: image(imgO,260,265); break;
     case 18: image(imgX,260,265);break;
     case 9:  image(imgO,420,265);break;
     case 19: image(imgX,420,265);break;
     case 60: 
     case 61: 
     case 62: redraw(s);
  }
  return s;
} 

void win(int w){
  stroke(0,0, 0);
  strokeWeight(10);
  switch(w){
    case 200: line(110,100,495,100); /* ---  */
              return;
    case 201: line(110,210,495,210); /* ---   */
              return;
    case 202: line(110,320,495,320); /* ___   */
              return;
    case 203: line(140,90,140,320); /* |     */
              return;
    case 204: line(300, 90,300,320); /*  |    */
              return;
    case 205: line(460, 90,460,320); /*   |   */
              return;
    case 206: line(140, 90,470,320); /* \     */
              return;
    case 207: line(140,320,450,90); /* /     */
              return;
  }
} 

void draw(){
  textFont(f,16);                  
  fill(0);                         
  text("RESET",550,30);   
  while (myPort.available()>0){
    int val = myPort.read();
    win(fillSquare(val));
    println(val);   
  }
}

void mouseClicked() {
  if (mouseX > 100 && mouseX < 190 && mouseY > 60 && mouseY < 140) {
   myPort.write(1);
  }
  else if (mouseX > 210 && mouseX < 390 && mouseY > 60 && mouseY < 140) {
   myPort.write(2);
  }
 else if (mouseX > 410 && mouseX < 500 && mouseY > 60 && mouseY < 140) {
   myPort.write(3);
  }
  else if (mouseX > 100 && mouseX < 190 && mouseY > 160 && mouseY < 250) {
   myPort.write(4);
  }
  else if (mouseX > 210 && mouseX < 390 && mouseY > 160 && mouseY < 250) {
   myPort.write(5);
  }
 else if (mouseX > 410 && mouseX < 500 && mouseY > 160 && mouseY < 250) {
   myPort.write(6);
  }
  else if (mouseX > 100 && mouseX < 190 && mouseY > 275 && mouseY < 400) {
   myPort.write(7);
  }
  else if (mouseX > 210 && mouseX < 390 && mouseY > 275 && mouseY < 400) {
   myPort.write(8);
  }
 else if (mouseX > 410 && mouseX < 500 && mouseY > 275 && mouseY < 400) {
   myPort.write(9);
  }
  else if (mouseX > 550 && mouseX < 600 && mouseY > 20 && mouseY < 35) {
   myPort.write(10);
  }
  
}

