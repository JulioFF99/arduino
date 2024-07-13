#include <math.h>
#include <Braccio.h>
#include <Servo.h>

#define PI 3.1415926535897932384626433832795

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;

// Definimos las longitudes del robot
float L1 = .0715;
float L2 = .125;
float L3 = L2;
float L5 = .192; //L5 es realmente la suma de L4+L5 
                  float angle=-60*PI/180; //angulo de aproximación 

//Definimos las posiciones de los cubos,usando cinemática directa para realizar ajustes finos
                  float pc[5][6]={{125,75,5,35,90,13},{118, 85, 0,5,110,13},{105, 80, 0,30,90,13},{87, 80, 0,3,80,13},{77, 80, 0,35,80,13}};
// float pc[5][6]={{125,70,5,30,110,13},{110, 85, 0,10,90,13},{100, 80, 0,30,90,13},{90, 80, 0,10,90,13},{80, 80, 0,35,80,13}};
//Definimos una posición intermedia 
float pinta[6]={100,90,20,10,90,13}; //posicion intermedia abierta 
float pintc[6]={100,90,20,10,90,73}; //posicion intermedia cerrada
//Definimos la posición de las zonas/columnas donde dejar los cubos
                  float zona[4][3]={{-0.05,-0.28,-0.03},{0.02,-0.25,-0.03},{0.18,-0.25,-0.03},{0.09,-0.25,-0.03}}; //1=rojo,2=azul,3=blanco,4=negro coordenadas de las columnas
 float col0[3]={};
 float col1[3]={}; //columnas donde se apilan los bloques 
 float col2[3]={};
 float col3[3]={}; // columna de desecho o de colores no identificados
   

//definimos la posición de la tolva
float ptolvaabierta[6] = {65, 103, 0,0,62,13}; //suelta el cubo en la tolva
float ptolva[6] = {65, 103, 0,0,62,73}; //deja el cubo en la tolva 


// offset para apilar cubos
float offset=0.03; // cm de offset

// cinematica inversa arreglo de valores articulares
float q[6] = {90, 90, 90, 90, 90, 73}; //q[0]=q1,....q[3]=q4, q[4]= ángulo de inclinación del último eslabón(L4)+ pinza  
float qan[6]; //posición anterior del robot en caso de que se llegue a una posición singular

//guarda los datos ingresados por el usuario que indican que color le corresponde a cada columna
int cc[4] = {2, 4, 1,3};

//contadores cubos en columna
int Cubo_C[4]={0,0,0,0};

// sensor de color
      //VCC——5V   
      //GND——GND
      //S0——D3 s3e cambio el pin D3 por el pin D13   
      //S1——D4
      //S2——D7   
      //S3——D8
      //OUT——D2
      // Pin  central  del  Potenciómetro‐ A0
    
      const int s0 = 13;
      const int s1 = 4;
      const int s2 = 7;
      const int s3 = 8;
      const int out = 2;
      int  Pot  =  0;
      int Rp;
      int Gp;
      int Bp;
      int Rc;   //cubos de color rojo
      int Bc;   //cubos de color blanco
      int Ac;   //cubos de color azul
      int Nc;   //cubos de color negro
      int Nic;   //cubos de color no identificado
     
      int Rca; //cubos de color rojo cantidad anterior
      int Bca;//cubos de color blanco cantidad anterior
      int Aca;//cubos de color azul cantidad anterior
      int Nca;//cubos de color negro cantidad anterior
      int Nica;   //cubos de color no identificado
      int Cmediciones=0;
                                                        const int p1=9; //pulsador 1
                                                        const int p2=10; //pulsador 2
    
      int p1s=0; //pulsador 1 estado
      int p2s=0; //pulsador 2 estado
    
      byte countRed = 0;
      byte countGreen = 0;
      byte countBlue = 0;
                                                        int tc=0; //variable auxiliar que almacena el resultado del sensor de color;    


void setup()
{
 
   Serial.begin(9600);
   
   //inicializar sensor de color
       pinMode(s0, OUTPUT);
       pinMode(s1, OUTPUT);
       pinMode(s2, OUTPUT);
       pinMode(s3, OUTPUT);
       pinMode(out, INPUT);
       pinMode(p1, INPUT_PULLUP);
       pinMode(p2, INPUT_PULLUP);  
      digitalWrite(s0,  HIGH); 
      digitalWrite(s1,  HIGH);
        Rc=0;
        Ac=0;
        Nc=0;
        Bc=0;
        Rca=0;
        Aca=0;
        Nca=0;
        Bca=0;
        Cmediciones=100; // cantidad de veces a repetir la prueba de obtener color.
      
       //Inicializar braccio
          Braccio.begin();
          float qt[6]={90,90,90,90,90,13}; 
          // Ponemos el robot en posición vertical
          mover_braccio(20,qt);
}


void loop(){
  float temp[6]={}; //arreglo temporal para valores articulares
  //float p[3] = {.3, .25, .1}; //posicion cartesiana (para cinematica inversa)
  
  clasificarColumnas();
  
  for (int a=0;a<5;a++){ // cantidad de veces que se va a aplicar el ciclo completo
        for (int j=0;j<6;j++){
        temp[j]=pc[a][j]; //guarda las posiciones de los cubos en un arreglo temporal
        }
        Serial.println("----------");
        Serial.print(" Cubo: "); Serial.println(a+1);
        Serial.println("Posiciones cartesianas:");  //imprime las posiciones de los cubos 
        cuboatolva(temp); //mueve los cubos de su posición inicial a la posición de la tolva 
        dkine(temp); //muestra las posiciones del cubo usando cinematica directa
        delay(3000);
        clasificarCubos();      
  }
  Serial.print("La cantidad de cubos en cada columna son "); Serial.print(Cubo_C[0]);Serial.print(" en la columna 0, ");Serial.print(Cubo_C[1]);Serial.print(" en la columna 1,");
        Serial.print(Cubo_C[2]);Serial.print(" en la columna 2, ");Serial.print(Cubo_C[3]);Serial.println(" en la columna 3");
}

void clasificarColumnas(){
  //valores que relacionan la columna con el color
  int i = 0; 
  
  // zonas/columnas donde depositar los bloques
  while(true){
    while (Serial.available() > 0)
  {
    cc[i++] = Serial.parseFloat(SKIP_ALL, '\n'); //indica el color de la columna 
    if (i>3)
      break;
  }
  if (i==3){
    cc[3]=10-cc[0]-cc[1]-cc[2]-1;
    for(int o=0;o<3;o++){
      cc[o]=cc[o]-1; //dado que los indices empiezan en 0 para simplificar los colores quedan igual pero empiezan en cero 
    }
     Serial.println("Coordenadas cartesianas de las columnas elegidas (x y z)");   
        col0[0]=zona[cc[0]][0];col0[1]=zona[cc[0]][1];col0[2]=zona[cc[0]][2]; //col es la variable que guarda las posiciones de las diferentes columnas de colores, el número que tienen no indica el color.
        col1[0]=zona[cc[1]][0];col1[1]=zona[cc[1]][1];col1[2]=zona[cc[1]][2];
        col2[0]=zona[cc[2]][0];col2[1]=zona[cc[2]][1];col2[2]=zona[cc[2]][2]; 
        col3[0]=zona[cc[3]][0];col3[1]=zona[cc[3]][1];col3[2]=zona[cc[3]][2];
    Serial.print("columna 0: ");     
    for (int j=0;j<3;j++){Serial.print(col0[j]);Serial.print(" ");  }
    Serial.print("\n"); 
    Serial.print("columna 1: ");
    for (int j=0;j<3;j++){Serial.print(col1[j]);Serial.print(" ");}
    Serial.print("\n");
    Serial.print("columna 2: ");
    for (int j=0;j<3;j++){Serial.print(col2[j]);Serial.print(" ");} 
    Serial.print("\n");
    Serial.print("columna 3: ");
    for (int j=0;j<3;j++){Serial.print(col3[j]);Serial.print(" ");}
    Serial.print("\n"); 
    break;    
    }
    }
}



void clasificarCubos(){
  float qtemp[6]={0,0,0,0,0,0};
  delay(100);
  tc= promedio(Cmediciones);//sensor de color
  delay(100);
    if(cc[0]==tc) //compara el valor introducido con el valor obtenido del sensor de color 
    {           // si el valor coincide el cubo se lleva a las coordenadas cartesianas de la columna por medio de cinematica inversa
      Serial.println("posiciones articulares col 0");
        ikine(col0,qtemp,angle);
        print_q(qtemp);     
        Serial.print("\n"); 
      tolvaacolumna(col0); 
      col0[2]=col0[2]+offset; // se le agrega un offset a la coordenada z para poder apilar los cubos
      Cubo_C[0]++;
    }
      else if (cc[1]==tc){
        Serial.println("posiciones articulares col 1");
          ikine(col1,qtemp,angle);
          print_q(qtemp);     
          Serial.print("\n");
        tolvaacolumna(col1);
        col1[2]=col1[2]+offset;
        Cubo_C[1]++;
      }
      else if(cc[2]==tc){ // tres zonas cada una con un color distinto
        Serial.println("posiciones articulares col 2");
          ikine(col2,qtemp,angle);
          print_q(qtemp);     
          Serial.print("\n");
         tolvaacolumna(col2);
         col2[2]=col2[2]+offset;
         Cubo_C[2]++;
      }
        else{ // la cuarta zona es para el color sobrante o colores no identificados
          Serial.println("posiciones articulares col 3");
            ikine(col3,qtemp,angle);
            print_q(qtemp);     
            Serial.print("\n");
          tolvaacolumna(col3);
          col3[2]=col3[2]+offset;
          Cubo_C[3]++;
        }
        
        switch (tc) {
            case 0:
             Rc++; //agrega 1 a la cuenta RC cubos rojos
            break;
            case 1:
            Ac=Ac+1; //cubos azules
            break;
            case 2:
            Bc=Bc+1; // cubos blancos
            break;
            case 3:
             Nc=Nc+1; // cubos negros
            break;
            default:
            Nic=Nic+1; // cubos de color no identificado
            
          } 
          //contadores de cubos
        if(Rc>Rca||Ac>Aca||Bc>Bca||Nc>Nca||Nic>Nica){ // indica la cantidad de cubos hasta el momento
        Serial.print("La cantidad de cubos son "); Serial.print(Rc);Serial.print(" rojos, ");Serial.print(Ac);Serial.print(" azules, ");
        Serial.print(Nc);Serial.print(" negros, ");Serial.print(Bc);Serial.print(" blancos, ");Serial.print(Nic);Serial.println(" de color no identificado");
        Rca=Rc;
        Aca=Ac;
        Nca=Nc;
        Bca=Bc;
        Nica=Nic;
        }
}


void mover_braccio(uint8_t retardo,float *q){
  //Visualización de la posición articular
   Braccio.ServoMovement(retardo,180-q[0],q[1]-7,q[2]+3,q[3]-7,180-q[4],q[5]); //movimiento con offset
   /*Cuando se ralizaron los cálculos D-H la suposición de la orientación de los ejes del motor 1 y 5 son inversas a las 
   reales. Por lo tanto se debe invertir el sentido de giro. Dado que el robot sólo va de 0 a 180, en vez de poner -q 
   debemos restárselo a 180, que hace de 0. */
   //Se han incluido offsets calculados de forma experimental, para que tenacitas quede bien recto.
}
