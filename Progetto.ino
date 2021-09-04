#include <SoftwareSerial.h>
SoftwareSerial PortaSeriale = SoftwareSerial(10, 11); //rx, tx
volatile int carattere = 0; //usate per gestire i singoli caratteri
int vettore[702]; //usato per immagazinare la stringa
boolean datiUscita = false; //usato come flag per capire quando finisce la stringa in ricezione
int dimArray=0; //dimensione della stringa in input

// serial input and output character
void setup() {
  // put your setup code here, to run once:
  PortaSeriale.begin(9600);   // inizializza la porta seriale a 9600 bit al secondo
  while (!PortaSeriale) {
          ;  // attende la connessione del dispositivo allo spartphone
  }
} 
void loop() 
{
  ricevi();
}

void scambiaPosto(int vettore[]){ 
  //variabile usata per fare lo spostamento dei caratteri della stringa nell'array
  int varTemp; 
  //con un ciclo for inverte l'ordine dei caratteri della stringa
  //la dimensione del vettore viene disiva per 2 perchè altrimenti a fine ciclo il vettore tornerebbe 
  //ad essere quello iniziale
  for(int i = 0;i < dimArray/2; i++){ 
    varTemp = vettore[i];
    vettore[i] = vettore[dimArray-1-i];
    vettore[dimArray-1-i] = varTemp; 
  }
}

void cifraturaCarattere(){ //usato per cifrare e decifrare i singoli caratteri della stringa
  //I caratteri da A a M vengono convertiti in quelli da N a Z
  if (carattere >= 'A' && carattere <= 'M'){
            carattere += 13;
         } 
         // I caratteri da N a Z vengono convertiti in quelli da A a M
         else if (carattere >= 'N' && carattere <= 'Z'){
            carattere -= 13;
         } 
         // I caratteri da a a m vengono convertiti in quelli da n a z
         else if (carattere >= 'a' && carattere <= 'm'){
            carattere += 13;
         } 
         //I caratteri da n a z vengono convertiti in quelli da a a m
         else if (carattere >= 'n' && carattere <= 'z'){
            carattere -= 13;
         }
         else if (carattere == '.'){ //i simboli speciali vengono convertiti nelle corrispondenti cifre
            carattere = '0';
         }
          else if (carattere == '?'){
            carattere = '1';
         }
          else if (carattere == '!'){
            carattere = '2';
         }
          else if (carattere == '('){
            carattere = '3';
         }
          else if (carattere == ')'){
            carattere = '4';
         }
          else if (carattere == '"'){
            carattere = '5';
         }
          else if (carattere == ';'){
            carattere = '6';
         }
          else if (carattere == ':'){
            carattere = '7';
         }
          else if (carattere == ','){
            carattere = '8';
         }
          else if (carattere == '@'){
            carattere = '9';
         }
         else         
         asm (
          //carica nel registro r24 i dati contenuti nella variabile carattere
          "lds  r24, carattere  \n"  
          //carica nel registro r21 la costante di 8 bit 36 (corrispondente al 
          //simbolo $in codifica utf-8)
          "ldi  r21, 36  \n"  
          //compara il valore contenuto nel registro r24 con la costante di 8 bit 48
          //corrispondente al simbolo 0)
          "cpi r24, 48 \n" 
          //se i valori comparati prima sono uguali, salto le instruzioni fino all'etichetta 0
          "breq 0f       \n" 
          //il numero 49 in codifica utf8 corrisponde al simbolo 1, e così via il valore 
          //dei numeri aumenterà di un'unità fino ad arrivare al simbolo 9, corrispondente
          //al numero 57
          "cpi r24, 49 \n" 
          "breq 1f       \n"
          "cpi r24, 50 \n"
          "breq 2f       \n"
          "cpi r24, 51 \n"
          "breq 3f       \n"
          "cpi r24, 52 \n"
          "breq 4f       \n"
          "cpi r24, 53 \n"
          "breq 5f       \n"
          "cpi r24, 54 \n"
          "breq 6f       \n"
          "cpi r24, 55 \n"
          "breq 7f       \n"
          "cpi r24, 56 \n"
          "breq 8f       \n"
          "cpi r24, 57 \n"
          "breq 9f       \n" 
          "cpi r24, 36 \n"
          "breq 10f       \n" 
           
           "sts (carattere), r21 \n" // carico un byte di dati dal registro r21 nella variabile carattere
          "rjmp FINE       \n" //effettuo un salto fino all'etichetta FINE
          
          "0: ldi  r24, 46 \n" //il numero 46 in codifica utf-8 rappresenta il simbolo "."
          "sts (carattere), r24 \n"
          "rjmp FINE       \n"
          "1: ldi  r24, 63 \n" //il numero 63 in codifica utf-8 rappresenta il simbolo "?"
          "sts (carattere), r24 \n"
          "rjmp FINE       \n"
          "2: ldi  r24, 33 \n" //il numero 33 in codifica utf-8 rappresenta il simbolo "!"
          "sts (carattere), r24 \n"
          "rjmp FINE       \n"
          "3: ldi  r24, 40 \n" //il numero 24 in codifica utf-8 rappresenta il simbolo "("
          "sts (carattere), r24 \n"
          "rjmp FINE       \n"
          "4: ldi  r24, 41 \n" //il numero 41 in codifica utf-8 rappresenta il simbolo ")"
          "sts (carattere), r24 \n"
          "rjmp FINE       \n"
          "5: ldi  r24, 34 \n" //il numero 34 in codifica utf-8 rappresenta il simbolo " " "
          "sts (carattere), r24 \n"
          "rjmp FINE       \n"
          "6: ldi  r24, 59 \n" //il numero 59 in codifica utf-8 rappresenta il simbolo ";"
          "sts (carattere), r24 \n"
          "rjmp FINE       \n"
          "7: ldi  r24, 58 \n" //il numero 58 in codifica utf-8 rappresenta il simbolo ":"
          "sts (carattere), r24 \n"
          "rjmp FINE       \n"
          "8: ldi  r24, 44 \n" //il numero 44 in codifica utf-8 rappresenta il simbolo ","
          "sts (carattere), r24 \n"
          "rjmp FINE       \n"
          "9: ldi  r24, 64 \n" //il numero 64 in codifica utf-8 rappresenta il simbolo "@"
          "sts (carattere), r24 \n"
          "rjmp FINE       \n"
          "10: ldi  r24, 32 \n" //il numero 32 in codifica utf-8 rappresenta il simbolo " "
          "sts (carattere), r24 \n"
          "FINE:   \n"
          // lista finale con output, input e clobber list, ognuno divisi, dal simbolo :
          //in questo caso i registri r24 e r21 fanno parte della clobber list, cioè la lista
          //dei registri da riservare al programma
          ::: "r24", "r21" //
        );
}

void inviaArray(int vettore[]){
  if(datiUscita == true){
    Serial.write("messaggio che va indietro:  ");    
    for(int i = 0; i < dimArray; i++){
      PortaSeriale.write(vettore[i]);
      Serial.write(vettore[i]);
    }
    datiUscita=false;
  }
  Serial.println();
}



void ricevi(){
    static boolean datiIngresso = false;
    char limStringa = '|';
     while (PortaSeriale.available() > 0 && datiUscita == false) {
        carattere = PortaSeriale.read();  // read one byte of input 

        if (datiIngresso == true) {
            if (carattere != limStringa) {
                cifraturaCarattere();
                vettore[dimArray] = carattere;  //cifra il carattere e lo inserisce nell'array
                dimArray++;
            }
            else {
              datiUscita = true;
              scambiaPosto(vettore);
              inviaArray(vettore);
              datiIngresso = false;
              dimArray = 0;
            }
        }
        else if (carattere == limStringa) {
            datiIngresso = true;
        }
    }          
}


    


   
