#include "contiki.h"
#include "net/rime/rime.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "dev/temperature-sensor.h"
#include "dev/light-sensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the device name and type
#define DEVICE_NAME "lamp01"
#define DEVICE_TYPE "lamp"

// Define the message structure
struct message {
  struct header {
    char recipient[10];
    int message_id;
  } header;
  struct body {
    char command[10];
    int value;
  } body;
  struct footer {
    char checksum[10];
    char flow_control[10];
  } footer;
};

static struct message msg;

// Callback function for receiving messages
static void recv_uc(struct unicast_conn *c, const linkaddr_t *from) {
  printf("unicast message received from %d.%d\n", from->u8[0], from->u8[1]);

  // Extract the command from the message body
  char command[10];
  strncpy(command, msg.body.command, sizeof(msg.body.command));

  // Execute the command
  if (strcmp(command, "on") == 0) {
    leds_on(LEDS_RED);
  } else if (strcmp(command, "off") == 0) {
    leds_off(LEDS_RED);
  }
}

// Define the unicast connection
static const struct unicast_callbacks unicast_callbacks = {recv_uc};
static struct unicast_conn uc;

PROCESS(lamp_process, "Lamp Process");
AUTOSTART_PROCESSES(&lamp_process);

PROCESS_THREAD(lamp_process, ev, data) {
  PROCESS_EXITHANDLER(unicast_close(&uc);)

  PROCESS_BEGIN();

  // Initialize the unicast connection
  unicast_open(&uc, 146, &unicast_callbacks);

  // Set the device name and type in the message header
  strncpy(msg.header.recipient, DEVICE_NAME, sizeof(DEVICE_NAME));
  msg.header.message_id = 123456;

  while (1) {
    // Wait for a command from the server
    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);

    // Set the command and value in the message body
    strncpy(msg.body.command, "on", sizeof("on"));
    msg.body.value = 0;

    // Send the message to the server
    linkaddr_t server_addr = {1, 0};
    packetbuf_copyfrom(&msg, sizeof(msg));
    unicast_send(&uc, &server_addr);

    printf("Command sent to server: %s\n", msg.body.command);
  }

  PROCESS_END();
}
