import socket
import json
import sys

# Adresses IP et numéros de port des différents équipements
lamp_address = ('fe80::1234:5678:9abc:def0%eth0', 5001)
temp_sensor_address = ('fe80::abcd:1234:5678:ef00%eth0', 5002)

# Fonction pour envoyer une commande à la lampe
def send_lamp_command(command):
    # Création d'un socket TCP et connexion à la lampe
    with socket.socket(socket.AF_INET6, socket.SOCK_STREAM) as s:
        s.connect(lamp_address)
        # Construction du message au format JSON
        message = {
            "header": {
                "recipient": "lamp_01",
                "message_id": 123456
            },
            "body": {
                "command": command
            },
            "footer": {
                "checksum": "",
                "flow_control": "start"
            }
        }
        # Conversion du message en chaîne de caractères JSON
        json_message = json.dumps(message)
        # Envoi du message à la lampe
        s.sendall(json_message.encode())
        # Réception de la réponse de la lampe
        response = s.recv(1024)
        # Traitement de la réponse au format JSON
        json_response = json.loads(response.decode())
        if json_response["body"]["status"] == "success":
            print("Commande envoyée avec succès à la lampe")
        else:
            print("Erreur lors de l'envoi de la commande à la lampe")

# Fonction pour recevoir des données du capteur de température
def receive_temp_data():
    # Création d'un socket UDP et liaison sur l'adresse du capteur de température
    with socket.socket(socket.AF_INET6, socket.SOCK_DGRAM) as s:
        s.bind(temp_sensor_address)
        # Réception du message du capteur de température
        data, addr = s.recvfrom(1024)
        # Traitement du message au format JSON
        json_data = json.loads(data.decode())
        # Affichage des données de température et d'humidité
        print("Température: {}°C".format(json_data["body"]["data"]["temperature"]))
        print("Humidité: {}%".format(json_data["body"]["data"]["humidity"]))

# Fonction pour modifier la fréquence d'envoi de données du capteur de température
def set_temp_interval(interval):
    # Création d'un socket TCP et connexion au capteur de température
    with socket.socket(socket.AF_INET6, socket.SOCK_STREAM) as s:
        s.connect(temp_sensor_address)
        # Construction du message au format JSON
        message = {
            "header": {
                "recipient": "temp_01",
                "message_id": 123456
            },
            "body": {
                "command": "set_interval",
                "interval": interval
            },
            "footer": {
                "checksum": "",
                "flow_control": "start"
            }
        }
        # Conversion du message en chaîne de caractères JSON
        json_message = json.dumps(message)
        # Envoi du message au capteur de température
        s.sendall(json_message.encode())
        # Réception de la réponse du cap
# Boucle principale pour traiter les commandes utilisateur
while True:
    # Attendre une commande utilisateur
    command = input("> ")

    # Analyser la commande utilisateur
    if command.startswith("lamp"):
        words = command.split()
        if len(words) < 3:
            print("Erreur: commande invalide")
            continue
        lamp_id = words[0]
        lamp_command = words[1]
        if lamp_command == "on":
            send_lamp_command(lamp_id, "on")
        elif lamp_command == "off":
            send_lamp_command(lamp_id, "off")
        elif lamp_command == "brightness":
            brightness = int(words[2])
            send_lamp_command(lamp_id, "brightness", brightness)
        else:
            print("Erreur: commande invalide")
            continue
    elif command.startswith("temp"):
        words = command.split()
        if len(words) < 2:
            print("Erreur: commande invalide")
            continue
        temp_id = words[0]
        temp_command = words[1]
        if temp_command == "get":
            receive_temp_data(temp_id)
        elif temp_command == "interval":
            interval = int(words[2])
            set_temp_interval(temp_id, interval)
        else:
            print("Erreur: commande invalide")
            continue
    elif command == "list":
        print_devices()
    elif command.startswith("info"):
        device_id = command.split()[1]
        print_device_info(device_id)
    elif command == "quit":
        print("Arrêt du centre de contrôle")
        break
    else:
        print("Erreur: commande invalide")
        continue

