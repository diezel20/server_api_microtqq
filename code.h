#include <18F2550.h>
#device ADC=10
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=20000000) // Cristal de 20 MHz
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, parity=N, bits=8) // UART pour ESP8266

// Configuration des pins du LCD
#define LCD_RS PIN_B4
#define LCD_EN PIN_B5
#define LCD_D4 PIN_B0
#define LCD_D5 PIN_B1
#define LCD_D6 PIN_B2
#define LCD_D7 PIN_B3
#include <lcd.c> // Bibliothèque LCD pour CCS C

// Configuration ThingSpeak
char* ssid = "microtechqq"; // Remplacez par votre SSID WiFi
char* password = "123456789"; // Remplacez par votre mot de passe WiFi
char* api_key = "WZ9PXDCRSBPUQOXB"; // Remplacez par votre clé API ThingSpeak
char* server = "api.thingspeak.com";

// Variables globales
float temp1, temp2, temp3;

// Fonction pour initialiser l'ESP8266
void init_ESP8266() {
   printf("AT+RST\r\n"); // Réinitialisation de l'ESP8266
   delay_ms(2000);
   printf("AT+CWMODE=1\r\n"); // Mode station (client WiFi)
   delay_ms(2000);
   printf("AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password); // Connexion au WiFi
   delay_ms(10000); // Attendre la connexion (ajustez si nécessaire)
   printf("AT+CIPMUX=0\r\n"); // Mode connexion simple
   delay_ms(1000);
}

// Fonction pour envoyer les données à ThingSpeak
void send_to_thingspeak(float temp1, float temp2, float temp3) {
   char temp1_str[10], temp2_str[10], temp3_str[10];
   sprintf(temp1_str, "%3.1f", temp1); // Conversion de la température 1
   sprintf(temp2_str, "%3.1f", temp2); // Conversion de la température 2
   sprintf(temp3_str, "%3.1f", temp3); // Conversion de la température 3
   printf("AT+CIPSTART=\"TCP\",\"%s\",80\r\n", server); // Connexion TCP à ThingSpeak
   delay_ms(2000);
   printf("AT+CIPSEND=85\r\n"); // Longueur de la requête GET (ajustez si nécessaire)
   delay_ms(1000);
   printf("GET /update?api_key=%s&field1=%s&field2=%s&field3=%s\r\n", api_key, temp1_str, temp2_str, temp3_str); // Envoi des températures
   delay_ms(1000);
   printf("AT+CIPCLOSE\r\n"); // Fermer la connexion
   delay_ms(1000);
}

void main() {
   // Initialisation du microcontrôleur
   setup_adc_ports(sAN0|sAN1|sAN3); // Configurer RA0, RA1, RA3 comme entrées analogiques
   setup_adc(ADC_CLOCK_INTERNAL); // Horloge interne pour l'ADC
   setup_ccp1(CCP_OFF); // Désactiver CCP1
   setup_ccp2(CCP_OFF); // Désactiver CCP2
   setup_timer_0(RTCC_OFF); // Désactiver Timer0
   setup_timer_1(T1_DISABLED); // Désactiver Timer1
   setup_timer_2(T2_DISABLED, 0, 1); // Désactiver Timer2

   // Initialisation du LCD
   lcd_init(); // Initialiser le LCD
   lcd_putc("\f"); // Effacer l'écran

   // Initialisation de l'ESP8266
   init_ESP8266();

   while(TRUE) {
      // Lire la température du capteur 1 (RA0/AN0)
      set_adc_channel(0); // Sélectionner le canal AN0
      delay_ms(100); // Attendre la stabilisation de l'ADC
      int16 adc_value1 = read_adc(); // Lire la valeur ADC
      temp1 = (adc_value1 * 5.0 / 1023.0) * 100.0; // Conversion en °C

      // Lire la température du capteur 2 (RA1/AN1)
      set_adc_channel(1); // Sélectionner le canal AN1
      delay_ms(100); // Attendre la stabilisation
      int16 adc_value2 = read_adc(); // Lire la valeur ADC
      temp2 = (adc_value2 * 5.0 / 1023.0) * 100.0; // Conversion en °C

      // Lire la température du capteur 3 (RA3/AN3)
      set_adc_channel(3); // Sélectionner le canal AN3
      delay_ms(100); // Attendre la stabilisation
      int16 adc_value3 = read_adc(); // Lire la valeur ADC
      temp3 = (adc_value3 * 5.0 / 1023.0) * 100.0; // Conversion en °C

      // Afficher les températures sur le LCD (rotation pour afficher T1, T2, T3)
      lcd_putc("\f"); // Effacer l'écran
      lcd_gotoxy(1, 1); // Première ligne
      printf(lcd_putc, "T1:%3.1f T2:%3.1f", temp1, temp2);
      lcd_gotoxy(1, 2); // Deuxième ligne
      printf(lcd_putc, "T3:%3.1f C", temp3);
      delay_ms(2000); // Afficher pendant 2 secondes

      // Envoyer les températures à ThingSpeak
      send_to_thingspeak(temp1, temp2, temp3);

      // Attendre 20 secondes (ThingSpeak recommande un délai minimum de 15s)
      delay_ms(20000);
   }
}