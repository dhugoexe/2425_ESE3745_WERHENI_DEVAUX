## TP1 Commande MCC basique
## Génération de 4 PWM

![image](https://github.com/user-attachments/assets/595a7202-ebe4-4573-9a65-71ace011c0eb)

Fréquence de la PWM : 20kHz
Temps mort minimum : à voir selon la datasheet des transistors (faire valider la valeur)
Résolution minimum : 10bits.
Pour les tests, fixer le rapport cyclique à 60%.
![image](https://github.com/user-attachments/assets/6e4ea078-e335-4641-81d5-670c1b0f5845)

## *Configuration de Timer1

![image](https://github.com/user-attachments/assets/9334986f-084b-472b-9abd-fdaea6aed4ae)

## *Calcule de temps mort 

![image](https://github.com/user-attachments/assets/97f3a64e-5bbd-4c1a-8b16-40a9c450a555)


##  Commande de vitesse

Pour controler la vitesse du moteur, nous allons envoyer une séquence via la liaison UART (par l'USB) de la forme :

speed XXXX
Le traitement de cette chaine de caractère se faire de la manière suivant :
Détection des premiers caractères "speed"
Conversion de tous les caractères représentant des chiffres XXXX en nombre entier
Vérification de la valeur (si la valeur est supérieur au maximum autorisé (bien spécifier cette valeur), on l'abaisse à cette valeur),
Application de cette vitesse au moteur à travers le registre gérant le rapport cyclique de la PWM

![image](https://github.com/user-attachments/assets/fc4c3920-518f-4363-8949-afcde5f18c99)
