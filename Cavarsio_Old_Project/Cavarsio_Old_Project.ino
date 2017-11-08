#include <AccelStepper.h>
#include <dht.h>


//variaveis do motor
int velocidade_motor = 100, aceleracao_motor = 100, window_total = 400;
//variaveis de controle da janela
int window[2] = {0,0}, curtains = 0;
// Definicao pino ENABLE
int pino_enablew1 = 2, pino_enablew2 = 3;

int percentil = 0, percentil_c = 0,percentil_w1 = 0,percentil_w2 = 0, percentil_aux;
String string;
int sensorDHT = A0;
int sensorLDR = A1;
float luz;
float luzaux;
dht DHT;

// Definicao pinos STEP e DIR
AccelStepper motor1(1,50,52 );
AccelStepper motor2(1,46,68 );

void setup()
{
  Serial.begin(9600);
  pinMode(pino_enablew1, OUTPUT);
  pinMode(pino_enablew2, OUTPUT);
  // Configuracoes iniciais motor de passo
  motor1.setMaxSpeed(velocidade_motor);
  motor1.setSpeed(velocidade_motor);
  motor1.setAcceleration(aceleracao_motor);
  motor1.setCurrentPosition(0);
  motor2.setMaxSpeed(velocidade_motor);
  motor2.setSpeed(velocidade_motor);
  motor2.setAcceleration(aceleracao_motor);
  motor2.setCurrentPosition(0);
  digitalWrite(pino_enablew1, LOW);
  digitalWrite(pino_enablew2, LOW);
  //Serial.println("=================================================");
  delay(1000);
}

void loop()
{
  //os dois char a seguir tem que ser setado dentro do loop por motivos de Sei la... 
  char buf[12];
  char *p = buf;
  string = "";

  DHT.read11(sensorDHT);
  luz = analogRead(sensorLDR);
  luzaux = ((luz/1024)*100);
  Serial.print("U:");
  Serial.println(DHT.humidity);
  Serial.print("E:");
  Serial.println(DHT.temperature);
  Serial.print("L:");
  Serial.println(luzaux);
  if(Serial.available()>0){
    string = leStringSerial();
    string.toCharArray(buf, sizeof(buf));
    curtains =  separaValoresc(atoi(strtok_r(p, "|", &p)));
    window[0] = separaValoresw1(atoi(strtok_r(p, "|", &p)));
    window[1] = separaValoresw2(atoi(strtok_r(p, "|", &p)));
    /*
    Serial.print(percentil_w1);
    Serial.print(" | ");
    Serial.print(percentil_w2);
    Serial.print(" | ");
    Serial.println(percentil_c);
    */
    delay(1000);
  }

  //envia os valores de direção e passos para o motor
  motor1.moveTo(window[0]);
  motor2.moveTo(window[1]);
  motor1.run();
  motor2.run();
  //limpa o buffer da serial e as variaveis
  Serial.flush();
}  

String leStringSerial(){
  String conteudo = "";
  char caractere;
  // Enquanto receber algo pela serial
  while(Serial.available() > 0) {
    // Lê byte da serial
    caractere = Serial.read();
    // Ignora caractere de quebra de linha
    if (caractere != '\n'){
      // Concatena valores
      conteudo.concat(caractere);
    }
    // Aguarda buffer serial ler próximo caractere
    delay(10);
  }
  //Serial.print("Valor do serial ");
  //Serial.println(conteudo);
  return conteudo;
}
//metodos para setar direção dos motores - um pra cada variavel... nao teve outro jeito
int separaValoresw1(int value){
  //Serial.print("Window 1 Entrou com ");
  //Serial.println(value);
  int finalCalc;
  if(value != 0){
    percentil = value; //entra com valores
    //Serial.print("passou com ");
    //Serial.println(percentil);
    percentil_aux = percentil;//seta valor auxiliar
    if(percentil == percentil_w1){//verifica se o botao se manteve parado ou andou
      finalCalc = 0;
      //Serial.println("Sem movmentação");
    }else if(percentil != percentil_w1){
      finalCalc = setPercentil(percentil - percentil_w1);
      //Serial.print("Movimentou ");
      //Serial.println(finalCalc);
    }
    percentil_w1 = percentil_aux;//seta variavel antiga para verificação dos itens acima
  }
  //Serial.println(finalCalc);
  return finalCalc;
}
int separaValoresw2(int value){
  //Serial.print("Window 2 Entrou com ");
  //Serial.println(value);
  int finalCalc;
  if(value != 0){
    percentil = value; //entra com valores
    //Serial.print("passou com ");
    //Serial.println(percentil);
    percentil_aux = percentil;//seta valor auxiliar
    if(percentil == percentil_w2){//verifica se o botao se manteve parado ou andou
      finalCalc = 0;
      //Serial.println("Sem movmentação");
    }else if(percentil != percentil_w2){
      finalCalc = setPercentil(percentil - percentil_w2);
      //Serial.print("Movimentou ");
      //Serial.println(finalCalc);
    }
    percentil_w2 = percentil_aux;//seta variavel antiga para verificação dos itens acima
  }
  //Serial.println(finalCalc);
  return finalCalc;
}
int separaValoresc(int value){
  //Serial.print("Cortina Entrou com ");
  //Serial.println(value);
  int finalCalc;
  if(value != 0){
    percentil = value; //entra com valores
    //Serial.print("passou com ");
    //Serial.println(percentil);
    percentil_aux = percentil;//seta valor auxiliar
    if(percentil == percentil_c){//verifica se o botao se manteve parado ou andou
      finalCalc = 0;
      //Serial.println("Sem movmentação");
    }else if(percentil != percentil_c){
      finalCalc = setPercentil(percentil - percentil_c);
      //Serial.print("Movimentou ");
      //Serial.println(finalCalc);
    }
    percentil_c = percentil_aux;//seta variavel antiga para verificação dos itens acima
  }
  //Serial.println(finalCalc);
  return finalCalc;
}

//calcula porcentagem
float setPercentil(float value){
  return ((window_total*value)/100);
}
