// Projeto Useless Box by Fabio Kimura fkimura@gmail.com
#include <Servo.h> 

const int maoFora = 15;
const int maoDentro = 180;
const int tampaAberta = 25;
const int tampaFechada = 60;
const int centro = 90;
const int direita = 130;
const int esquerda = 50;
const int olhar = 45;

Servo servoMao;  
Servo servoTampa;
Servo servoHead;
// velocidade servo mg995
//  0,20 seg/60ยบ (sem carga) = 360 millis para 180 = 3,34 millis por grau
const int velocidadeServoMao = 7;
// velocidade microservo 9g:
//  0,12 seg/60ยบ (sem carga) = 360 millis para 180 = 2 millis por grau
const int velocidadeServoTampa = 7;
const int velocidadeServoHead = 4;
// qual movimento sera executado
int movimento = 1;

void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  servoTampa.attach(9);  
  servoMao.attach(10);
  servoHead.attach(11);  
  pararTudo();
  Serial.begin(9600); 
  Serial.println("comecou");
//  attachInterrupt(digitalPinToInterrupt(2), verificarLuz, CHANGE);
}

void loop() {
  if (interruptorLigado()) {
    ligarLuz();
    if (servosParados()) {
      movimentar();
    }  
  } else {
    desligarLuz();
  }
  
  delay(100);
}//fim loop

void moverHead() {
   for (int i = centro; i <= direita; i += 1) {
    servoHead.write(i);
    delay(velocidadeServoHead);   
  }
  servoHead.write(direita);
  delay(500);
  for (int i = direita; i >= esquerda; i -= 1) {
    servoHead.write(i);
    delay(velocidadeServoHead);   
  }
  servoHead.write(esquerda);
  delay(500);
  for (int i = esquerda; i <= centro; i += 1) {
    servoHead.write(i);
    delay(velocidadeServoHead);   
  }
  servoHead.write(centro);
  delay(500);
}

void moverHead(int pos) {
  int inicio = servoHead.read();
  for (int i = 0; i <= 45; i++) {
    int j = map(i, 0, 45, inicio, pos);
    servoHead.write(j);
    delay(10);  
  }
}

void moverHead2() {
  for (int t = 1; t<=1; t++) {
  servoHead.write(direita);
  for (int i = direita; i >= esquerda; i -= 1) {
    servoHead.write(i);
    delay(velocidadeServoHead * 2);   
  }
  servoHead.write(esquerda);
  for (int i = esquerda; i <= direita; i += 1) {
    servoHead.write(i);
    delay(velocidadeServoHead * 2);   
  }
  }
  for (int i = direita; i >= centro; i -= 1) {
    servoHead.write(i);
    delay(velocidadeServoHead * 2);   
  }
  delay(500);
}


boolean servosParados() {
  if (servoMao.read() == maoDentro
     && servoTampa.read() == tampaFechada) {
    return true;
  }
  return false;
}
void pararTudo() {
  servoMao.write(maoDentro); 
  servoTampa.write(tampaFechada);
  servoHead.write(centro);
}
void movimentar() {
  if (movimento == 1) {
    movimento1();
  }
  if (movimento == 2) {
    movimento2();
  }
  if (movimento == 3) {
    movimento3();
  }
  if (movimento == 4) {
    movimento4();
  }
  if (movimento == 5) {
    movimento5();
  }    
  movimento++;
  if (movimento > 5) {
    movimento = 1;
  }
}
// aciona a tampa:
//const int tampaAberta = 20;
//const int tampaFechada = 80;
void fecharTampa(int atraso, int passo) {
  Serial.println("fechando tampa...");
// 40 pra 0
  int totalAtraso = velocidadeServoTampa * (tampaFechada - tampaAberta);
  Serial.println(totalAtraso);
  for (int i = tampaAberta; i <= tampaFechada; i += passo) {
    servoTampa.write(i);
    delay(atraso);   
    totalAtraso -= atraso;
  }
  //Serial.println(totalAtraso);
  servoTampa.write(tampaFechada);
  if (totalAtraso > 0) {
    delay(totalAtraso);
  }
  Serial.println("tampa fechada");
}
void abrirTampa(int atraso, int passo) {
  Serial.println("abrindo tampa...");
// 0 pra 40
  int totalAtraso = velocidadeServoTampa * abs(tampaFechada - tampaAberta);
  Serial.println(totalAtraso);
  int fresta = 0;
  if (passo > 10) {
    fresta = random(15,30);
  }
  for (int i = tampaFechada; i >= (tampaAberta + fresta); i -= passo) {
    servoTampa.write(i);
    delay(atraso);   
    totalAtraso -= atraso;
  }
  servoTampa.write(tampaAberta + fresta);
  Serial.println(totalAtraso);
  delay(totalAtraso);
}

//// aciona as maos:
//const int maoFora = 170;
//const int maoDentro = 0;
void moverMaoFora(int atraso, int passo) {
//  servoHead.write(olhar);
  Serial.println("movendo mao pra fora...");
  // 0 -> 170
  int totalAtraso = velocidadeServoMao * (maoDentro - maoFora);
  Serial.println(totalAtraso);
  
  for (int i = maoDentro; i >= maoFora; i -= passo) {
    servoMao.write(i);
    delay(atraso);
    int j = map(i, maoDentro, maoFora, centro, olhar); 
    servoHead.write(j);
    totalAtraso -= atraso;
  }
  servoMao.write(maoFora);
  verificarLuz(totalAtraso);
}
void moverMaoDentro(int atraso, int passo) {
  Serial.println("movendo mao pra dentro...");

  // 170 -> 0
  int totalAtraso = velocidadeServoMao * (maoDentro - maoFora);
  Serial.println(totalAtraso);
  for (int i = maoFora; i <= maoDentro; i += passo) {
    servoMao.write(i); 
    int j = map(i, maoFora, maoDentro, olhar, centro); 
    servoHead.write(j);
    delay(atraso);   
    totalAtraso -= atraso;
  }
  servoMao.write(maoDentro);
  Serial.println(totalAtraso);
  delay(totalAtraso);
}

void baterMaoFora(int atraso, int vezes) {
  Serial.println("batendo mao pra fora...");
  for (int i = 1; i<= vezes; i++) {
    int angulo = random(10,40) + 20;
    int totalAtraso = velocidadeServoMao * (maoFora + angulo + random(30));
    servoMao.write(maoFora + angulo ); 
    delay(totalAtraso);
    servoMao.write(maoFora + 10);
    delay(totalAtraso);
    if (servoHead.read() != centro) {
      moverHead(centro);
    }
  }
  servoMao.write(maoFora + 10);
}

// movimento padrao:
void movimento1() {
  Serial.println("movimento1");
  delay(random(500,1000));
  abrirTampa(0, 2);
  moverHead();
  delay(random(200,1000));
  moverMaoFora(0, 10);
  moverMaoDentro(0, 10); 
  fecharTampa(0, 2);
  pararTudo();
}
// deixa a mao em cima do botao
void movimento2() {
    Serial.println("movimento2");
  delay(random(500,1000));
//  abrirFecharTampa(0, 2);
  abrirTampa(0, 2);
  delay(random(200,900));
  moverMaoFora(10, 10);
  moverHead();
  delay(random(500,1000));
  moverMaoDentro(0, 10); 
  fecharTampa(0, 2);
  pararTudo();
}

// bate a mao na alavanca desligada
void movimento3() {
  Serial.println("movimento3");
  delay(random(500,1000));
  abrirTampa(0, 2);
  delay(random(200,1000));
  moverMaoFora(0, 10);
  baterMaoFora(10,3);
  moverHead(centro);
  delay(random(500,1000));
  moverMaoDentro(0, 10); 
  fecharTampa(0, 2);
  pararTudo();
}
// abre e fecha a tampa rapidamente antes de desligar
void movimento4() {
  Serial.println("movimento4");
  delay(random(500,1000));
  abrirTampa(0,  25);
  fecharTampa(0, 25);
  abrirTampa(0,  25);
  fecharTampa(0, 25);
  abrirTampa(0,  25);
  fecharTampa(0, 25);
  abrirTampa(0,  5);
  delay(random(200,1000));
  moverMaoFora(0, 10);
  baterMaoFora(10,4);
    moverHead2();
  delay(random(1000,2000));
  moverMaoDentro(0, 10); 
  fecharTampa(0, 2);
  pararTudo();
}
// abre e fecha a tampa rapidamente antes de desligar
void movimento5() {
  Serial.println("movimento5");
  delay(random(500,1000));
  abrirTampa(0,  15);
  fecharTampa(0, 15);
  abrirTampa(0,  15);
  fecharTampa(0, 15);
  abrirTampa(0,  15);
  fecharTampa(0, 15);
  abrirTampa(0,  15);
  fecharTampa(0, 15);
  abrirTampa(0,  5);

  delay(random(200,1000));
  moverMaoFora(0, 10);
  baterMaoFora(10,4);
  moverHead2();
  delay(random(1000,2000));
  moverMaoDentro(0, 10); 
  fecharTampa(0, 2);
  delay(1000);
  abrirTampa(10,5);
  delay(1000);
  fecharTampa(10,1);
  delay(1000);
  abrirTampa(0,5);
  delay(1000);
  fecharTampa(30,1);
  delay(1000);
  pararTudo();
}

void desligarLuz() {
   digitalWrite(13,LOW);
   analogWrite(3, 0);
   analogWrite(5, 0);
   analogWrite(6, 0);

   noTone(12);
   //Serial.println("Desligou!");
}
void ligarLuz() {
   digitalWrite(13,HIGH);
   //Serial.println("Ligou!");
   analogWrite(3, 255);//blue
   analogWrite(5, 220);//red
   analogWrite(6, 255); //green   
   tone(12, 200);
}
boolean interruptorLigado() {
  if (digitalRead(2) == LOW) {
    return true;
  }
  return false;
}
void verificarLuz(int d) {
  for (int i = 1; i < d/10; i++) {
    delay(10);
    if (interruptorLigado()) {
      ligarLuz();
    } else {
      desligarLuz();
    }
  }
}
