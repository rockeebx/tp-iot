#include "contiki.h"
#include "net/rime.h"
#include "random.h"
#include <stdio.h>
#include "dev/temperature-sensor.h"
#include "protocol.h"

PROCESS(temp_sensor_process, "Temperature Sensor Process");
AUTOSTART_PROCESSES(&temp_sensor_process);

static struct broadcast_conn broadcast;
static const struct broadcast_callbacks broadcast_call = {};

PROCESS_THREAD(temp_sensor_process, ev, data)
{
    static struct etimer timer;
    static struct message msg;
    static uint8_t msg_buffer[MAX_MESSAGE_LEN];

    PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

    PROCESS_BEGIN();

    // Initialisation de la connexion broadcast
    broadcast_open(&broadcast, CHANNEL, &broadcast_call);

    while(1) {
        // Attente d'une temporisation pour l'envoi du message
        etimer_set(&timer, CLOCK_SECOND * SEND_INTERVAL);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Récupération de la température
        uint16_t temperature = (uint16_t) ((temperature_sensor.value() / 10) + 0.5);

        // Création du message
        msg.header.recipient = SERVER_ID;
        msg.header.message_id = random_rand() % MAX_MESSAGE_ID;
        msg.body.command = TEMPERATURE_UPDATE;
        msg.body.temperature = temperature;
        msg.footer.checksum = 0;

        // Calcul du checksum
        uint8_t *ptr = (uint8_t *) &msg;
        uint16_t len = sizeof(struct message) - sizeof(msg.footer.checksum);
        uint8_t checksum = 0;
        for (uint16_t i = 0; i < len; i++) {
            checksum ^= ptr[i];
        }
        msg.footer.checksum = checksum;

        // Copie du message dans le buffer pour l'envoi
        memcpy(msg_buffer, &msg, sizeof(struct message));

        // Envoi du message
        packetbuf_copyfrom(msg_buffer, sizeof(struct message));
        broadcast_send(&broadcast);
    }

    PROCESS_END();
}
