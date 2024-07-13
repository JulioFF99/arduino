
int promedio(int a){

  for(int i=0;i<a;i++){
   getColor(); 
   Rp=countRed+Rp;
   Gp=countGreen+Gp;
   Bp=countBlue+Bp;
  }
  Rp=Rp/a;
  Gp=Gp/a;
  Bp=Bp/a;
  
    Serial.print("Color  rojo promedio: ");Serial.print(Rp);Serial.print("de "); Serial.print(a);Serial.println(" medidas");
    Serial.print("Color  verde promedio: ");Serial.print(Gp);Serial.print("de "); Serial.print(a);Serial.println(" medidas");
    Serial.print("Color  azul promedio: ");Serial.print(Bp);Serial.print("de "); Serial.print(a);Serial.println(" medidas");
    
    //intervalos de color 
    if (Rp  > 0   &&   Rp  <  30   &&   Gp  >  20   &&   Gp  <  80 && Bp  > 20   &&   Bp  <  100)  { Serial.println("Cubo es de color rojo");return 0;}         // Serial.println(" color rojo promedio");
    else  if (Rp  >  10   &&   Rp  <  50   &&   Gp  >  0   && Gp  <  50   &&   Bp  >  0   &&   Bp  < 20)  {Serial.println("Cubo es de color azul");  return 1;}      //Serial.println(" color azul promedio");
    else  if (Rp  >  0   &&   Rp  <  10   &&   Gp  >  0   && Gp  <  10   &&   Bp  >  0   &&   Bp  <  10){Serial.println("Cubo es de color blanco");return 2;}   //Serial.println("color blanco promedio");
    else  if (Rp  >  30   &&   Rp  <  100   &&   Gp  >  30   && Gp  <  100   &&   Bp  >  0   &&   Bp  <  100)  {Serial.println("Cubo es de color negro");  return 3;}    //Serial.println(" color negro promedio");
    else  { Serial.println("Color no identificado"); return 4;}
  Rp=0;
  Gp=0;
  Bp=0;
  delay(100);
}

void getColor()
{
  Pot  =  analogRead(A0); //aprox 395
  Serial.print("Pot:  "); Serial.print(Pot); Serial.print("\t");
   digitalWrite(s2, LOW);
   digitalWrite(s3, LOW);
   countRed = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
   digitalWrite(s3, HIGH);
   countBlue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
   digitalWrite(s2, HIGH);
   countGreen = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
   delay(20);
}
