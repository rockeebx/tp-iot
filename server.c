#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"

#define MESSAGE_SIZE 50

/* Structure du message */
typedef struct {
  int temperature;
  char sender[10];
} message_t;

/* Callback appelée lorsqu'un message est reçu */
void receive_callback(const void *data, uint16_t len, const linkaddr_t *source, const linkaddr_t *destination) {
  /* Vérification de la taille du message */
  if (len != sizeof(message_t)) {
    printf("Message invalide\n");
    return;
  }

  /* Conversion des données en structure message_t */
  message_t message;
  memcpy(&message, data, sizeof(message_t));

  /* Affichage du message reçu */
  printf("Message reçu de %s : température = %d\n", message.sender, message.temperature);

  /* Contrôle de la lampe en fonction de la température */
  if (message.temperature >= 25) {
    /* Allumage de la lampe */
    printf("Allumage de la lampe\n");
  } else {
    /* Extinction de la lampe */
    printf("Extinction de la lampe\n");
  }
}

/* Fonction d'initialisation */
void init() {
  /* Initialisation de NullNet */
  nullnet_buf = (uint8_t *)&message;
  nullnet_len = sizeof(message_t);
  nullnet_set_input_callback(receive_callback);
}

/* Programme principal */
PROCESS(server_process, "Serveur");
AUTOSTART_PROCESSES(&server_process);
PROCESS_THREAD(server_process, ev, data) {
  PROCESS_BEGIN();
  init();
  while (1) {
    PROCESS_WAIT_EVENT();
  }
  PROCESS_END();
}
