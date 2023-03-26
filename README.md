# Contrôle de la lampe et du capteur avec Contiki

## Description
Ce projet permet de contrôler une lampe et un capteur de température/humidité à l'aide d'un centre de contrôle écrit en Python. Le centre de contrôle peut envoyer des commandes à la lampe pour l'allumer/ l'éteindre et régler sa luminosité, et peut recevoir des données du capteur pour obtenir la température et l'humidité. Le tout est réalisé avec ContikiOS.

## Configuration
La configuration matérielle nécessaire est la suivante :

- Une carte d'évaluation compatible avec Contiki
- Une lampe connectée à la carte
- Un capteur de température/humidité connecté à la carte

## Installation
1. Cloner le dépôt `
2. Ajouter les fichiers "lamp.c" et "temp_sensor.c" dans le dossier "examples/sensors/" du dépôt Contiki
3. Compiler le code Contiki pour la carte d'évaluation
4. Flasher le code compilé sur la carte d'évaluation
5. Exécuter le centre de contrôle en Python avec la commande `python control_center.py`

## Utilisation
Le centre de contrôle peut être utilisé avec les commandes suivantes :

- Allumer une lampe : `lamp01 on`
- Éteindre une lampe : `lamp02 off`
- Régler la luminosité d'une lampe : `lamp01 brightness 50`
- Obtenir la température actuelle d'un capteur : `temp01 get`
- Modifier la fréquence d'envoi de données d'un capteur : `temp02 interval 60`
- Obtenir la liste des dispositifs connectés : `list`
- Obtenir des informations sur un dispositif spécifique : `info lamp01`
- Quitter le serveur : `quit`

