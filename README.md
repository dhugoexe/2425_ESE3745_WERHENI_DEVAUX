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


##  Premiers tests

Pour controler la vitesse du moteur, nous allons envoyer une séquence via la liaison UART (par l'USB) de la forme :

speed XXXX
Le traitement de cette chaine de caractère se faire de la manière suivant :
Détection des premiers caractères "speed"
Conversion de tous les caractères représentant des chiffres XXXX en nombre entier
Vérification de la valeur (si la valeur est supérieur au maximum autorisé (bien spécifier cette valeur), on l'abaisse à cette valeur),
Application de cette vitesse au moteur à travers le registre gérant le rapport cyclique de la PWM

![Capture d'écran 2024-11-13 162355](https://github.com/user-attachments/assets/6edce119-dbec-453e-9168-16da37e74e04)



![IMG_0243](https://github.com/user-attachments/assets/e43b6472-9f8c-4ed9-a330-3deb1754be1c)
![IMG_0238](https://github.com/user-attachments/assets/e5b2945f-b053-441c-bef5-8e218e16560f)


## tp2
## Commandes de vitesse 

Rajouter quelques fonctionnalités à votre projet :

Commande start : permet de fixer le rapport cyclique à 50% (vitesse nulle) et d'activer la génération des pwm (HAL_TIM_PWM_Start et HAL_TIMEx_PWMN_Start),
Commande stop : permet de désactiver la génération des PWM.
Commande speed XXXX : permet de définir le rapport cyclique à XXXX/PWM_MAX, mais afin de réduire l'appel à courant, vous devez établir une montée progressive à cette vitesse en quelques secondes. Vous pouvez effectuer une rampe entre la valeur actuelle et la valeur cible avec un incrément bien réfléchi de la PWM à un intervalle de temps régulier. Par la suite votre asservissement fera cela tout seul.

![image](https://github.com/user-attachments/assets/0539bb5e-770f-4dc4-9d0a-02c8bcea4e99)

Resultat:
![image](https://github.com/user-attachments/assets/69a8a22f-f437-4978-85f0-92d08a0418be)

