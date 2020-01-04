
#include <SoftwareSerial.h>
SoftwareSerial bt(10,11);




int dataIn[5] {0,0,0,0}; //array para almacenar toda la informacion (255, btn, x, y)
int in_byte = 0;
int array_index = 0;


#define enA 5
#define in1 2
#define in2 3
#define enB 6
#define in3 7
#define in4 8

int xAxis, yAxis;
int x=0;
int y=0;
int motorSpeedA=0;
int motorSpeedB=0;

void setup() {

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  Serial.begin (9600); // starts the serial monitor
  bt.begin(38400);
}

void loop() {
  xAxis = 510;
  yAxis = 510;

  /*while(bt.available())
  {  */  
    if (bt.available() > 0) //recieve byte from phone
    {  
      in_byte = bt.read(); //store in byte into a variable
      
      if (in_byte == (255)) // if the variable is 0 stet the array inxed to 0. this will make sure that every number goes into the correct index
      { 
        array_index = 0;
      }
            
      dataIn[array_index] = in_byte;  //almacena cada valor en una posicion del array
      // dataIn[0] ==> 255 con ese valor inician los datos recibidos
      // dataIn[1] ==> estado del boton que se presionó(2 o 1)
      // dataIn[2] ==> posicion de x
      // dataIn[3] ==> posicion de y
      array_index = array_index +1; 
    }
     
    x = dataIn[2];
    delay(10);
    y = dataIn[3];

    delay(10);

    // Makes sure we receive corrent values
    if (x > 0 & x < 250) 
    {
      xAxis = map(x, 250, 0, 1023, 0); // Convert the smartphone X and Y values to 0 - 1023 range, suitable motor for the motor control code below
    }
    
    if (y > 0 & y < 250) 
    {
      yAxis = map(y, 250, 0, 0, 1023);
    }



    // EJE Y PARA AVANZAR O RETROCEDER
    if(yAxis < 470)
    {

      //movoer el motor B hacia atras
      digitalWrite(in3, HIGH);
      
      digitalWrite(in4, LOW);
      
      // mover el motor A hacia atras
      digitalWrite(in1, HIGH);
      
      digitalWrite(in2, LOW);

      //convertir las lecturas decrecientes del eje-y  para ir hacia atras desde 100 a 0 en el rango de 0 a 255 el cual es el rango 
      //con el que trabaja PWM para aumentar la velocidad   del motor 
      motorSpeedA = map (yAxis, 470,0,0,255);
      
      motorSpeedB = map (yAxis, 470,0,0,255);
      
    } else if (yAxis > 550)
    {
        // mover el motor A hacia atras
      digitalWrite(in1, LOW);
      
      digitalWrite(in2, HIGH);


      //movoer el motor B hacia atras
      digitalWrite(in3, LOW);
      
      digitalWrite(in4, HIGH);


      //convertir las lecturas decrecientes del eje-y  para ir hacia atras desde 100 a 0 en el rango de 0 a 255 el cual es el rango 
      //con el que trabaja PWM para aumentar la velocidad   del motor 
      motorSpeedA = map (yAxis, 550,1023,0,255);
      
      motorSpeedB = map (yAxis, 550,1023,0,255);
    }
    else
    {
      motorSpeedA = 0;
      motorSpeedB = 0;
    }


    



    // EJE X PARA GIRAR A LA DERECHA O IZQUIERDA
    if(xAxis < 470)
    {
      // converitr las lecturas decrecientes del eje x que son desde 470 a 0 a 
      // valores crecientes de 0 a 255 para poder subir o bajar la velocidad de los motores y asi poder voltear
      int  xMapped = map (xAxis, 470, 0, 0, 255);

      //mover a la derecha aumentando la velocidad del motor izquierdo, y menorando la velocidad del motor derecho
      motorSpeedA = motorSpeedA + xMapped; 
      motorSpeedB = motorSpeedB - xMapped;

      // Confine the range from 0 to 255
      if (motorSpeedA < 0) 
      {
        motorSpeedA = 0;
      }
      
      if (motorSpeedB > 255)
      {
        motorSpeedB = 255;
      }
      
    }
    
    if (xAxis > 550) 
    {
       // Convertir las lecturas de incremento del eje x desde 550 - 1023 a valores entre 0 y 255 para poder controlar la velocidad de los motores
       int xMapped = map(xAxis, 550, 1023, 0, 255);
       
       // Move right - decrease right motor speed, increase left motor speed
       motorSpeedA = motorSpeedA - xMapped;
       motorSpeedB = motorSpeedB + xMapped;
       
       // Confine the range from 0 to 255
       if (motorSpeedA > 255) 
       {
         motorSpeedA = 255;
       }
       
       if (motorSpeedB < 0) 
       {
         motorSpeedB = 0;
       }
     }




    // prevenir el zumbido cuando el motor esta a bajas velocidades
    if(motorSpeedA < 70)
    {
      motorSpeedA = 0;
    }
    
    if (motorSpeedB < 70)
    {
      motorSpeedB = 0;  
    }



    

    // SETEAR LA VELOCIDAD FINAL
    analogWrite(enA, motorSpeedA);
    
    analogWrite(enB, motorSpeedB);

}
