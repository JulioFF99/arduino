void print_q (float *q)
{
  //Serial.println("----------");
  for (int i=0;i<4;i++)
  {
    Serial.print("q");
    Serial.print(i+1);
    Serial.print(": ");
    Serial.println(q[i]);
  }
  //Serial.println("----------");
}
void dkine(float *qt){
  float q[6]={};
  //print_q(qt);
 //Mover el robot a la posición indica por monitor serie
  for(int i=0;i<6;i++) {q[i]=qt[i]*PI/180;} // pasar de grados a radianes
 //Cálculo de la cinemática directa
 float x=cos(q[0])*(L3*sin(q[1] + q[2]) + L2*cos(q[1]) - L5*cos(q[1] + q[2] + q[3]));
 float y=sin(q[0])*(L3*sin(q[1] + q[2]) + L2*cos(q[1]) - L5*cos(q[1] + q[2] + q[3]));
 float z= L1 - L3*cos(q[1] + q[2]) + L2*sin(q[1]) - L5*sin(q[1] + q[2] + q[3]);

 Serial.print("pos X: "); Serial.println(x);
 Serial.print("pos Y: "); Serial.println(y);
 Serial.print("pos Z: "); Serial.println(z);
}

void ikine(float p[3], float q[6], float angle){ 
     // L4 y L5 es considerado un único eslabón. El ángulo respecto al suelo se define mediante un ángulo alfa. El vector v relaciona
     //la posición del efector final respecto al punto x,y,z final considerados en el cálculo de la cinemática inversa.
  float v[3];
  float x,y,z;
  v[0] = -L5 * cos(angle)*( p[0]/sqrt(p[0]*p[0] + p[1]*p[1]));
  v[1] = -L5 * cos(angle)*( p[1]/sqrt(p[0]*p[0] + p[1]*p[1]));
  v[2] =  -L5 * sin(angle);
  
  x = p[0] + v[0];
  y = p[1] + v[1];
  z = p[2] + v[2]- L1;
  
  // guardamos la posición anterior en caso de que la nueva posición no sea alcanzable
  for (int i=0;i<6;i++){
    qan[i] = q[i];
  }


// TODO
// Calculamos q_1
  q[0] = atan2(y,x);

// TODO
// Calculamos q_3
  float cos_q3, sin_q3;
  float p2 = x*x + y*y + z*z; //Ver apuntes tomados.
  cos_q3 =(p2-L2*L2-L3*L3)/(2*L2*L3) ;
 
 //Aquí es donde valoramos si quedarnos con la solución positiva o la negativa:
  sin_q3 =-sqrt(1-cos_q3*cos_q3);
  q[2] = atan2(sin_q3,cos_q3);

// Calculamos q_2
  float alpha, beta,pr; //Siendo pr el nombre que en mis apuntes es P, es decir la proyección del P sobre plano XY, calculado por pitágoras.
  pr=sqrt(x*x+y*y);
  beta =atan2(z,pr) ;
  alpha=atan2(L3*sin_q3, L2+L3*cos_q3);
  q[1] = beta-alpha;

// Ajustamos q_3 para el rango de movimiento de nuestro robot.
  q[2] = PI/2 + q[2];
// Definimos q_4 
  q[3] = PI-(q[1]+q[2])+angle;

        if(q[0]<0 || q[0]>PI){ // permite trabajar en el tercer y cuarto cuadrante
         q[0] = PI-abs(q[0]);
         q[1] = PI-q[1];
         q[2] = PI-q[2];
         q[3] = PI-q[3];
         }

  // Comprobamos si las magnitudes en las articulaciones son números reales (detección de singularidades)
if(isnan(q[0])||isnan(q[1])||isnan(q[2])||isnan(q[3])){
  
 Serial.println("La posición no puede ser alcanzada (posición singular)");
 Serial.println("Regresando a la posición anterior"); 
  for (int i=0;i<5;i++){
    Serial.println(q[i]);
    q[i] = qan[i];
    }
}else{

// pasamos los ángulos a grados
  for (int i=0;i<4;i++){q[i] = q[i]*180/PI;}
    }

}
