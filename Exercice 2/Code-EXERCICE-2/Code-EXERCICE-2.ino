#define count1HzMax 500
volatile unsigned int count1Hz;

// Définir les broches pour les LEDs et les boutons
 #define LED1 13 
// Broche pour LED1
#define LED2 12 
// Broche pour LED2 
#define P1 8 
// Broche pour Poussoir1
#define P2 9 
// Broche pour Poussoir2
// Définir les variables pour suivre l'état précédent des boutons
bool ancienEtatP1 = HIGH; 
// État précédent du bouton P1 (HIGH lorsque non pressé) 
bool ancienEtatP2 = HIGH; 
// État précédent du bouton P2 (HIGH lorsque non pressé)
bool actuelEtatP1; 
// État présent du poussoirP1 
bool actuelEtatP2; 
// État présent du poussoir P2 // Variables pour suivre l'état des LEDs 
bool etatLED1 ; // État de LED1
bool etatLED2 ; // État de LED2

void setup() {
    
    pinMode(P1, INPUT_PULLUP); // Poussoir P1 avec pull-up interne
    pinMode(P2, INPUT_PULLUP); // Poussoir P2 avec pull-up interne

}

void setupTimer0() {


    pinMode(LED1, OUTPUT);    // LED1 comme sortie
    pinMode(LED2, OUTPUT);    // LED2 comme sortie

    DDRD |= (1 << DDD0) | (1 << DDD6) | (1 << DDD2); // OC0A sur PD6 en sortie, PD2 = SYNC, PD0 = LED
    DDRB |= (1 << DDB0); // PB0 = signal RUN en sortie

    cli(); // Désactiver les interruptions pendant la configuration
    OCR0A = 249; // Pour générer une interruption toutes les 1 ms (à 16 MHz, prescaler = 64)
    TCCR0A = (1 << WGM01); // Mode CTC
    TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler = 64
    TIMSK0 = (1 << OCIE0A); // Activer l'interruption sur match A
    sei(); // Activer les interruptions globales
}

ISR(TIMER0_COMPA_vect) {
    // PORTD |= (1 << PD2); // Mise à 1 de SYNC
   
    // Code pour faire clignoter la LED
    // ----- Gestion de poussoir P1 -------
    actuelEtatP1 = digitalRead(P1); // Lire l'état actuel poussoir P1
    if (actuelEtatP1 != ancienEtatP1) { // comparer avec l'ancien état
        ancienEtatP1 = actuelEtatP1; // mise à jour de l'état précédent
        if (actuelEtatP1 == LOW) { // transition négative ‾‾‾\___
            etatLED1 = !etatLED1; // Inverser l'état de LED1
            digitalWrite(LED1, etatLED1); // écrire sur la sortie LED1

        }
    }

    // ----- Gestion de poussoir P2 -------
    actuelEtatP2 = digitalRead(P2); // Lire l'état actuel poussoir P2
    if (actuelEtatP2 != ancienEtatP2) { // comparer avec l'ancien état
        ancienEtatP2 = actuelEtatP2; // mise à jour de l'état précédent
        if (actuelEtatP2 == LOW) { // transition négative ‾‾‾\___
            etatLED2 = !etatLED2; // Inverser l'état de LED2
            digitalWrite(LED2, etatLED2); // écrire sur la sortie LED2
        }
    }
    // PORTD &= ~(1 << PD2); // Mise à 0 de SYNC
}

void loop() {
    // Le code de loop est vide
}



int main(){ 
  setupTimer0(); 
  while(1){
  PORTB ^= (1 << PB0); //Génération du signal carré RUN
}

}