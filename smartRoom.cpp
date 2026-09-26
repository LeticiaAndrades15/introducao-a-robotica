//Usados para a tela OLED
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//instalar as bibliotecas Adafruit SSD1306 e Adafruit GFX Library na ide 

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


//Usados para o sensor de temp e umidade
#include <DHT.h>
#define DHTPIN 7
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);


//nomeacao das portas 
#define estudoB 13
#define gamerV 12
#define descansoA 11
#define dormirP 10
#define ventilador 9
#define vermelho 3
#define azul 5
#define verde 6
#define porta 2
#define buzzer 8

int modoAtual = 0;
bool ligado = false;

void setup()
{
    pinMode(porta, INPUT_PULLUP);
    pinMode(estudoB, INPUT_PULLUP);
    pinMode(gamerV, INPUT_PULLUP);
    pinMode(descansoA, INPUT_PULLUP);
    pinMode(dormirP, INPUT_PULLUP);
    pinMode(ventilador, OUTPUT);
    pinMode(vermelho, OUTPUT);
    pinMode(verde, OUTPUT);
    pinMode(azul, OUTPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(temp, INPUT);
    Serial.begin(9600);
    dht.begin();

    //usado para a tela OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("OLED nao encontrada");
        while (true);
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Quarto iniciado!");
    display.display();
}	

//funcoes criadar para diferenciar os modos
void modoEstudo(){
    analogWrite(vermelho, 255);
    analogWrite(azul, 255);
    analogWrite(verde, 255);
}

void modoGamer(){
    analogWrite(vermelho, 255);
    analogWrite(azul, 20);
    analogWrite(verde, 120);   
    
    mostrarModo("GAMER");
}

void modoDescanso(){
    analogWrite(vermelho, 255);
    analogWrite(azul, 80);
    analogWrite(verde, 10);

    mostrarModo("DESCANSO");
}

void modoDormir(){
    analogWrite(vermelho, 0);
    analogWrite(azul, 0);
    analogWrite(verde, 0);

    mostrarModo("DORMIR");    
}

//funcao para abrir a porta e ligar o quarto
void abrirPorta(){
    analogWrite(vermelho, 255);
    analogWrite(azul, 255);
    analogWrite(verde, 255);

    float h = dht.readHumidity();
    float t = dht.readTemperature();


    if(isnan(h) || isnan(t)) {
        return;
    }


    if(t > 25 && h > 20){
        digitalWrite(ventilador, HIGH);
    }else{
        digitalWrite(ventilador, LOW);
    }


    Serial.print("Temperatura: ");
    Serial.println(t);
    Serial.print("Umidade: ");
    Serial.println(h);  

    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
}


//funcao para desligar o quarto
void fecharQuarto(){
    analogWrite(vermelho, 0);
    analogWrite(azul, 0);
    analogWrite(verde, 0);

    digitalWrite(ventilador, LOW);

    digitalWrite(buzzer, HIGH);
    delay(2000);
    digitalWrite(buzzer, LOW);

    desligarPC();

}

//exibe qual modo esta selecionado no quarto
void mostrarModo(String modo) {

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    display.clearDisplay();

    // Nome do quarto
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("QUARTO");

    // Modo atual
    display.setTextSize(2);
    display.setCursor(0, 12);
    display.println(modo);

    // Temperatura
    display.setTextSize(1);
    display.setCursor(0, 36);
    display.print("Temp: ");
    display.print(t);
    display.println(" C");

    // Umidade
    display.setCursor(0, 50);
    display.print("Umid: ");
    display.print(h);
    display.println("%");

    display.display();
}

//desliga a tela OLED
void desligarPC(){
    display.clearDisplay();
    display.display();
}

void loop()
{

    if(digitalRead(porta) == LOW){
        ligado = !ligado;

        if(ligado){
            abrirPorta();
            modoAtual = 0;
            mostrarModo("NENHUM MODO!");            
        }else{
            fecharQuarto();
            modoAtual = 0;
        }
        delay(200);
    }

    if(ligado){
        if(digitalRead(estudoB) == LOW){
                modoAtual = 1; 
            }

            if(digitalRead(gamerV) == LOW){
                modoAtual = 2; 
            }

            if(digitalRead(descansoA) == LOW){
                modoAtual = 3; 
            }

            if(digitalRead(dormirP) == LOW){
                modoAtual = 4; 
            }

            switch(modoAtual){
                case 1:
                modoEstudo();
                mostrarModo("ESTUDO");
                break;

                case 2:
                modoGamer();
                mostrarModo("GAMER");
                break;

                case 3:
                modoDescanso();
                mostrarModo("DESCANSO");
                break;

                case 4:
                modoDormir();
                mostrarModo("DORMIR");
                break;

        }
    }
   

}