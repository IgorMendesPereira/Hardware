/*
 * IN1 - pin 19
 * IN2 - pin 18
 * IN3 - pin 15
 * IN4 - pin 14
 * IN5 - pin 13
 * IN6 - pin 12
 * OUT1 - pin 7 
 * OUT2 - pin 8
 * OUT3 - pin 9
 * OUT4 - pin 10
 * OUT5 - pin 11
 */
#define LIGAR 0x01
#define DESLIGAR 0x02
#define LIGADO 0x03
#define DESLIGADO 0x04
#define ERROPRESSURIZADOR 0x09 
#define ERRO 0xFF
                                         
#define ON 7                                          
#define OFF 8 
#define pressurizador 19                                                                      
#define Pivo 18                                              


void LigaPivo();
void DesligaPivo();
byte comando = 2;
boolean estadopivo = false;
boolean estadopressurizador = false;
boolean estadoON = false;
boolean flag = true;
volatile unsigned long tempoInicial;


void setup() {

  Serial.begin(9600);
  pinMode(ON, OUTPUT);
  pinMode(OFF, OUTPUT);
  pinMode(pressurizador, INPUT_PULLUP);
  pinMode(Pivo, INPUT_PULLUP);

  digitalWrite(OFF,LOW);
  digitalWrite(ON,LOW);                      
}

void loop(){
  if (Serial.available()) {                                     
    comando = Serial.read();                                                        //Recebe comando para pivo
    if (comando == LIGAR)  {                                                        //Se o comando for para ligar      
      LigaPivo();                                                                   //Entra na função de ligar pivo
    }
    else if (comando == DESLIGAR) {                                                 //Se o comando for para desligar 
      DesligaPivo();                                                                //Entra na função de desligar pivo
    }
  } 
  
  
  estadopressurizador = digitalRead(pressurizador);                                 //Lê pressurizador
  if (estadoON && estadopressurizador){ 
    DesligaPivo();
    Serial.print(ERROPRESSURIZADOR);
  }
}

void LigaPivo() {
  tempoInicial = millis(); 
  while (((millis() - tempoInicial)<5000) && flag) {                                //Enquanto espera 5 min
    digitalWrite(ON,HIGH);                                                          //Aciona contato ON
    estadoON = true;
    if (Serial.available()) {                                                       //Se receber alguma mensagem
      comando = Serial.read();                                                      //Recebe comando para pivo
      if(comando == DESLIGAR){
        DesligaPivo();
        flag = false;                                                               //Flag false para sair do loop
      }
    } 
    estadopressurizador = digitalRead(pressurizador);                               //Lê pressurizador
    if (estadopressurizador == LOW){                                                //Se pressurizador OK
      flag = false;                                                                 //Flag false para sair do loop
      digitalWrite(ON,LOW);                                                         //Desaciona contato ON
      Serial.print(LIGADO);                                                         //Retorna mesagem de pivo ligado
    }                                                         
  }
  
  if (flag == true){
    digitalWrite(ON,LOW);
    Serial.print(ERROPRESSURIZADOR);
    DesligaPivo();
  }
  flag = true;
}

void DesligaPivo() {
  digitalWrite(OFF,HIGH); 
  tempoInicial = millis();
  while ((millis() - tempoInicial)<(2000)){
  }
  Serial.print(DESLIGADO); 
  digitalWrite(OFF,LOW);
  estadoON = false;
}

/*
  0xFF - Mensagem de erro
  0x01 - Comando para ligar pivo
  0x02 - Comando para desligar pivo
  0x03 - Pivo ligado
  0x04 - Pivo desligado
  0x05 - Comando para avancar
  0x06 - Comando para reverter 
  0x07 - Pivo Avancado
  0x08 - Pivo Revertido
  0x09 - Erro do pressurizador
  0x0A - 
  0x0B - 
*/
