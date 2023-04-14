  
void cuboatolva(float *pcubo){
  const int d=100; //delay 
  float  ptemp[6]={};
  for(int i=0;i<6;i++) {ptemp[i]=pinta[i];}
  //el brazo se mueve a la posición intermedia
  mover_braccio(20,pinta);
  delay(d);
  //el brazo se mueve a la q1 del cubo
  ptemp[0]=pcubo[0];
  mover_braccio(20,ptemp);
  delay(d);
  // el brazo se mueve a la posición del cubo, garra abierta 
  mover_braccio(20,pcubo);
  delay(d);
 //el brazo en la posición del cubo cierra la garra
 for(int i=0;i<6;i++) {ptemp[i]=pcubo[i];}
  ptemp[5]=73;
  mover_braccio(20,ptemp);
  delay(d);
  // el brazo se eleva un poco para evitar chocar con los demás cubos 
  ptemp[2]=ptemp[2]+25;
  mover_braccio(20,ptemp);
  delay(d);
  for(int i=0;i<6;i++) {ptemp[i]=pintc[i];}
  mover_braccio(20,ptemp);
  delay(d);
  //el brazo se mueve a la q1 de la tolva 
  ptemp[0]=ptolva[0];
  mover_braccio(20,ptemp);
  delay(d);  
  //el brazo se mueve a la tolva
  mover_braccio(20,ptolva);
  delay(d);
  mover_braccio(20,ptolvaabierta);
  delay(d);
  mover_braccio(20,pinta);  
}

void tolvaacolumna(float *pc){ //lleva el cubo desde la tolva a su columna correspondiente usando la cinemática inversa 
  float ptemp[6]={};
  float qtemp[3]={};
  float q[6] = {90, 90, 90, 90, 90, 73}; //q[0]=q1,....q[3]=q4, q[4]= ángulo de inclinación del último eslabón(L4)+ pinza  
  //uso de cinemática inversa para colocar el cubo en la columna
  ikine(pc, q,angle);
  //el brazo se mueve a la tolva agarrando al cubo
  for(int i=0;i<6;i++) {ptemp[i]=pinta[i];}
  ptemp[0]=ptolva[0];
  mover_braccio(20,ptemp);
  delay(100);
  for(int i=0;i<6;i++) {ptemp[i]=ptolvaabierta[i];}
  mover_braccio(20,ptemp);
  delay(100);
  mover_braccio(20,ptolva);
  delay(100);
  for(int i=0;i<6;i++) {ptemp[i]=ptolva[i];}
  ptemp[2]=ptemp[2]+30;
  mover_braccio(20,ptemp);
  delay(100);
  //el brazo se mueve a la zona/columna indicada
  mover_braccio(20,q);
  delay(100);
  for(int i=0;i<6;i++) {ptemp[i]=q[i];}
    //el brazo abre la garra
  ptemp[5]=13;
  mover_braccio(20,ptemp);
  delay(100); 
  ptemp[2]=80;
  mover_braccio(20,ptemp);
  delay(100); 
  //el brazo regresa a una posición intermedia
  mover_braccio(20,pinta);
  delay(100); 
}
