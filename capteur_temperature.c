#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include <stdio.h>
#include <string.h>
#include "dht11.h"
#include "sha1.h"

#define SERVER_IP_ADDR "fd00::1" // adresse IPv6 du serveur

static uip_ipaddr_t server_ipaddr;
static struct uip_udp_conn *udp_conn;

PROCESS(udp_client_process, "UDP client process");

AUTOSTART_PROCESSES(&udp_client_process);

static void calculate_checksum(char *msg, int len, char *checksum) {
  sha1_context ctx;
  sha1_starts(&ctx);
  sha1_update(&ctx, (uint8_t*)msg, len);
  sha1_finish(&ctx, (uint8_t*)checksum);
}

PROCESS_THREAD(udp_client_process, ev, data) {
  PROCESS_BEGIN();

  uip_ip6addr(&server_ipaddr, 0xfd00, 0, 0, 0, 0, 0, 0, 0x0001); //adresse IPv6 du serveur
  udp_conn = udp_new(&server_ipaddr, UIP_HTONS(3000), NULL); //creation d'une nouvelle connexion UDP

  while(1) {
    static char buf[100];
    int16_t temperature, humidity;
    int res = dht11_read(&temperature, &humidity);
    if(res == DHT11_SUCCESS) {
      sprintf(buf, "{\"header\":{\"recipient\":\"server\",\"message_id\":123456},\"body\":{\"device_type\":\"temperature\",\"device_id\":\"temp_sensor01\",\"data\":{\"temperature\":%d,\"humidity\":%d}},\"footer\":{\"checksum\":\"\",\"flow_control\":\"start\"}}", temperature, humidity); //construction du message dans une chaîne de caractères avec les valeurs de température et d'humidité récupérées depuis le capteur
      char checksum[20];
      calculate_checksum(buf, strlen(buf), checksum); //calcul du checksum du message
      sprintf(buf, "{\"header\":{\"recipient\":\"server\",\"message_id\":123456},\"body\":{\"device_type\":\"temperature\",\"device_id\":\"temp_sensor01\",\"data\":{\"temperature\":%d,\"humidity\":%d}},\"footer\":{\"checksum\":\"%s\",\"flow_control\":\"start\"}}", temperature, humidity, checksum); //ajout du checksum dans le champ "checksum" du message
      uip_udp_packet_send(udp_conn, buf, strlen(buf)); //envoi du message par UDP
      printf("Message sent: %s\n", buf); //affichage sur la console
    } else {
      printf("Failed to read temperature and humidity\n"); //affichage sur la console en cas d'erreur de lecture
    }
    PROCESS_PAUSE();
  }

  PROCESS_END();
}
