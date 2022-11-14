#include "lcdgfx.h"

#include "Wire.h"
#include <MPU6050_light.h>

MPU6050 mpu(Wire);


DisplaySH1106_128x64_I2C display(-1); // This line is suitable for most platforms by default // or (-1,{busId, addr, scl, sda, frequency})

#define WIDTH 128
#define HEIGHT 64

NanoCanvas<WIDTH,HEIGHT,1> canvas;





#define PADDING 100
#define TOTAL_WIDTH PADDING+WIDTH
#define TOTAL_HEIGHT PADDING+HEIGHT
struct Position { byte x;  byte y;};
struct Velocity { char x;   char y;};

#define PARTICLE_COUNT 79
Position position[] = {{62,2},{66,4},{70,6},{50,8},{74,8},{46,10},{54,10},{78,10},{42,12},{58,12},{82,12},{38,14},{62,14},{86,14},{36,16},{58,16},{54,18},{86,18},{36,20},{50,20},{74,20},{76,20},{46,22},{70,22},{76,22},{86,22},{36,24},{66,24},{74,24},{46,26},{62,26},{86,26},{36,28},{58,28},{72,28},{74,28},{46,30},{86,30},{36,32},{58,32},{72,32},{46,34},{70,34},{86,34},{36,36},{50,36},{58,36},{46,38},{54,38},{68,38},{70,38},{86,38},{36,40},{58,40},{46,42},{68,42},{86,42},{36,44},{58,44},{66,44},{68,44},{38,46},{46,46},{42,48},{58,48},{66,48},{46,50},{64,50},{78,50},{50,52},{58,52},{74,52},{54,54},{62,54},{64,54},{70,54},{58,56},{66,56},{62,58}};
Velocity velocity[] = {{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1}};


//#define PARTICLE_COUNT 69
//Position position[] = {{82,12},{38,14},{62,14},{86,14},{36,16},{58,16},{54,18},{86,18},{36,20},{50,20},{74,20},{76,20},{46,22},{70,22},{76,22},{86,22},{36,24},{66,24},{74,24},{46,26},{62,26},{86,26},{36,28},{58,28},{72,28},{74,28},{46,30},{86,30},{36,32},{58,32},{72,32},{46,34},{70,34},{86,34},{36,36},{50,36},{58,36},{46,38},{54,38},{68,38},{70,38},{86,38},{36,40},{58,40},{46,42},{68,42},{86,42},{36,44},{58,44},{66,44},{68,44},{38,46},{46,46},{42,48},{58,48},{66,48},{46,50},{64,50},{78,50},{50,52},{58,52},{74,52},{54,54},{62,54},{64,54},{70,54},{58,56},{66,56},{62,58}};
//Velocity velocity[] = {{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1},{0,1}};


const byte min_x = 38;
const byte min_y = 8;
  
const byte max_x = 88;
const byte max_y = 64;

const byte logo_width = max_x - min_x;
const byte logo_height = max_y - min_y;

short logo_pos_x = min_x;
short logo_pos_y = min_y;

short logo_vel_x = 0.0;
short logo_vel_y = 0.0;

float last_angle_x = 0.0;
bool offsets_computed = false;
void setup()
{
//    Serial.begin(9600);
    Wire.begin();



  
  
    display.begin();

    canvas.clear();
    canvas.setColor( 0xFF );

    const int min_speed = 1;
    const int max_speed = 5;
    for(int i=0; i< PARTICLE_COUNT; i++)
    {
      position[i].y = HEIGHT - position[i].y;
      position[i].y += 2;
      position[i].x += 2;

    
      velocity[i].x = random(min_speed, max_speed+1) * (random(0,2)*2-1);
      velocity[i].y = random(min_speed, max_speed+1) * (random(0,2)*2-1);
    
  
    }
    render();
    
    byte status = mpu.begin();
    while(status!=0){ } // stop everything if could not connect to MPU6050

//    delay(1000);
    mpu.setFilterGyroCoef(1.0);
    mpu.calcOffsets();

}

void loop()
{
    const unsigned long elapsed_time = millis();
//    const unsigned long start_time = millis();

    

//       update_logo(elapsed_time);
      if(elapsed_time < 6000)
        update_logo(elapsed_time);
      else
        update_particles();
    
//    const unsigned long end_time = millis();
//    Serial.print(end_time - elapsed_time);
//    Serial.print("\n");

    render();
    
}


void update_logo(unsigned long elapsed_time)
{
  mpu.update();
  float angle_delta = mpu.getAngleX()- last_angle_x;
//  Serial.print(last_angle_x);
//  Serial.print("\n");

  if(elapsed_time > 100)
  {
    move_logo(angle_delta *-3.0);
  }

  last_angle_x = mpu.getAngleX();
}

void move_logo(float angle_delta)
{
  logo_vel_x += angle_delta;
  logo_vel_x = constrain(logo_vel_x, -3, 3);
  
  if(logo_pos_x + logo_width + logo_vel_x > WIDTH-1 )
  {
    logo_vel_x *= 0;
  }

  if(logo_pos_x + logo_vel_x < 1 )
  {
    logo_vel_x *= 0;
  }

  logo_pos_x += logo_vel_x;
  
  for(int i=0; i< PARTICLE_COUNT; i++)
  {
    position[i].x += logo_vel_x;
  }
}
void update_particles()
{


  for(int i=0; i< PARTICLE_COUNT; i++)
  {
    position[i].x += PADDING;
    position[i].y += PADDING;
        
    if(position[i].x + velocity[i].x > TOTAL_WIDTH)
    {
      velocity[i].x *= -1;
      position[i].x = TOTAL_WIDTH-1;
    }
      

    if(position[i].y + velocity[i].y > TOTAL_HEIGHT)
    {
      velocity[i].y *= -1;
      position[i].y = TOTAL_HEIGHT-1;
    }
      

    if(position[i].x + velocity[i].x < PADDING)
    {
      velocity[i].x *= -1;
      position[i].x = PADDING+1;
    }
      

    if(position[i].y + velocity[i].y < PADDING)
    {
      velocity[i].y *= -1;
      position[i].y = PADDING+1;
    }
      

    
    position[i].x += velocity[i].x;
    position[i].y += velocity[i].y;


    position[i].x -= PADDING;
    position[i].y -= PADDING;
  }  
}

void render()
{
  
    canvas.clear();   

    for(int i=0; i< PARTICLE_COUNT; i++)
    {
      canvas.fillRect(position[i].x-2, HEIGHT - position[i].y,position[i].x-2 + 3, HEIGHT - position[i].y + 3);
    }
//      canvas.fillRect(logo_pos_x-2, HEIGHT - logo_pos_y, logo_pos_x-2 + 3, HEIGHT - logo_pos_y + 3);
//      canvas.fillRect(logo_pos_x+logo_width-2, HEIGHT - logo_pos_y, logo_pos_x+logo_width-2 + 3, HEIGHT - logo_pos_y + 3);
//
//      canvas.fillRect(logo_pos_x-2, HEIGHT - logo_pos_y - logo_height, logo_pos_x-2 + 3, HEIGHT - logo_pos_y - logo_height + 3);
//      canvas.fillRect(logo_pos_x+logo_width-2, HEIGHT - logo_pos_y- logo_height, logo_pos_x+logo_width-2 + 3, HEIGHT - logo_pos_y - logo_height + 3);
    display.drawCanvas(0, 0, canvas);
}
