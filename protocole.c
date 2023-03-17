// Code pour le protocole de communication

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

// Fonction pour envoyer un message JSON
void send_message(char* recipient, char* command) {
    // Création de l'en-tête
    struct json_object *header = json_object_new_object();
    json_object_object_add(header, "recipient", json_object_new_string(recipient));
    json_object_object_add(header, "message_id", json_object_new_int(rand())); // génération d'un identifiant de message aléatoire

    // Création du corps
    struct json_object *body = json_object_new_object();
    json_object_object_add(body, "command", json_object_new_string(command));

    // Création du pied de page
    struct json_object *footer = json_object_new_object();
    json_object_object_add(footer, "checksum", json_object_new_string("a1b2c3d4e5")); // exemple de checksum
    json_object_object_add(footer, "flow_control", json_object_new_string("start"));

    // Création du message complet
    struct json_object *message = json_object_new_object();
    json_object_object_add(message, "header", header);
    json_object_object_add(message, "body", body);
    json_object_object_add(message, "footer", footer);

    // Conversion du message en chaîne JSON
    const char *json_string = json_object_to_json_string(message);

    // Envoi du message
    printf("Envoi du message : %s\n", json_string);

    // Libération de la mémoire allouée pour les objets JSON
    json_object_put(header);
    json_object_put(body);
    json_object_put(footer);
    json_object_put(message);
}

int main() {
    // Exemple d'utilisation : envoi d'un message à une lampe
    send_message("lamp01", "on");

    return 0;
}
