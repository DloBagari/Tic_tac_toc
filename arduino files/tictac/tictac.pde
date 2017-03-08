import processing.serial.*;

Serial myPort;
PImage imgO;
PImage imgX;

void redraw(int c){
  switch(c){
    case 60: background(255, 255, 255);
             break;
    case 61: background(255,0,0);
             break;
    case 62: background(0,255,0);
             break;
    case 63: background(0,0,0);
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


   
}

int fillSquare(int s){
  
  switch (s){
     case 1: image(imgO,100,45);
               break;
     case 11: image(imgX,100,45);
              break;
     case 2: image(imgO,260,45); break;
     case 12: image(imgX,260,45); break;
     case 3: image(imgO,420,45); break;
     case 13: image(imgX,420,45); break;
     case 4: image(imgO,100,155); break;
     case 14: image(imgX,100,155); break;
     case 5:  image(imgO,260,155); break;
     case 15: image(imgX,260,155); break;
     case 6: image(imgO,420,155); break;
     case 16: image(imgX,420,155); break;
     case 7: image(imgO,100,265); break;
     case 17: image(imgX,100,265); break;
     case 8: image(imgO,260,265); break;
     case 18: image(imgX,260,265); break;
     case 9:  image(imgO,420,265); break;
     case 19: image(imgX,420,265); break;
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
    case 100: line(150,100,450,300); /* \     */
              return;
    case 101: line(150,300,450,100); /* /     */
              return;
    case 20: line(150,100,150,300); /* |     */
              return;
    case 209: line(300,100,300,300); /*  |    */
              return;
    case 208: line(450,100,450,300); /*   |   */
              return;
    case 200: line(110,100,495,100); /* ���   */
              return;
    case 201: line(110,210,495,210); /* ---   */
              return;
    case 202: line(110,320,495,320); /* ___   */
              return;
  }
} 

void draw(){
  while (myPort.available()>0){
    int val = myPort.read();
    win(fillSquare(val));
    println(val);
  }
}
