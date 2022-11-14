#include <Arduino.h>
#include <U8g2lib.h>



/*
  U8g2lib Example Overview:
    Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
    Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
    U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.
    
*/

//#define FULL_FRAME

#ifdef FULL_FRAME
  U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
#else
  U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
#endif
unsigned long prev_elapsed_time = 0;


#define WIDTH 128
#define HEIGHT 64


struct Position { short x;  short y;};
struct Velocity { char x;   char y;};

#define PARTICLE_COUNT 79
Position position[] = {{62,2},{66,4},{70,6},{50,8},{74,8},{46,10},{54,10},{78,10},{42,12},{58,12},{82,12},{38,14},{62,14},{86,14},{36,16},{58,16},{54,18},{86,18},{36,20},{50,20},{74,20},{76,20},{46,22},{70,22},{76,22},{86,22},{36,24},{66,24},{74,24},{46,26},{62,26},{86,26},{36,28},{58,28},{72,28},{74,28},{46,30},{86,30},{36,32},{58,32},{72,32},{46,34},{70,34},{86,34},{36,36},{50,36},{58,36},{46,38},{54,38},{68,38},{70,38},{86,38},{36,40},{58,40},{46,42},{68,42},{86,42},{36,44},{58,44},{66,44},{68,44},{38,46},{46,46},{42,48},{58,48},{66,48},{46,50},{64,50},{78,50},{50,52},{58,52},{74,52},{54,54},{62,54},{64,54},{70,54},{58,56},{66,56},{62,58}};
Velocity velocity[] = {{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1}};

void setup(void) {
  u8g2.begin();
//  Serial.begin(9600);


  const int min_speed = 1;
  const int max_speed = 5;
  for(int i=0; i< PARTICLE_COUNT; i++)
  {
    position[i].y = HEIGHT - position[i].y;
    position[i].y += 2;
    position[i].x += 2;
//    Position pos;
//    pos.x = (i % 32)*4;
//    pos.y = floor(i / 32.0)*4;
//
//    position[i].x = pos.x;
//    position[i].y = pos.y;
    
    
      velocity[i].x = random(min_speed, max_speed+1) * (random(0,2)*2-1);
      velocity[i].y = random(min_speed, max_speed+1) * (random(0,2)*2-1);
    //velocity[i].x = 0;
//    velocity[i].y = 0;

  }

}


void loop(void) {
  const unsigned long elapsed_time = millis();
  float delta_time = (elapsed_time - prev_elapsed_time)*0.001;

  if(elapsed_time > 2000)
  {
    
//      const unsigned long start_time = millis();
    update(delta_time);
//      const unsigned long end_time = millis();
//    Serial.print(end_time - start_time);
//    Serial.print("\n");

  }

  render();

  prev_elapsed_time = elapsed_time;
}


void update(float delta_time)
{
  for(int i=0; i< PARTICLE_COUNT; i++)
  {
    if(position[i].x + velocity[i].x> WIDTH)
      velocity[i].x *= -1;

    if(position[i].y + velocity[i].y> HEIGHT)
      velocity[i].y *= -1;

    if(position[i].x + velocity[i].x< 0)
      velocity[i].x *= -1;

    if(position[i].y + velocity[i].y< 0)
      velocity[i].y *= -1;

    
    position[i].x += velocity[i].x;
    position[i].y += velocity[i].y;
  }  
}

void render()
{
#ifdef FULL_FRAME
  u8g2.clearBuffer();         
  draw();
  u8g2.sendBuffer();
#else
  u8g2.firstPage();
  do {
   draw();
  } while ( u8g2.nextPage() );
#endif
}

void draw()
{
   for(int i=0; i< PARTICLE_COUNT; i++)
  {
    u8g2.drawBox(position[i].x-2, HEIGHT - position[i].y,4,4);
  }
}
