## L'objectifs

A partir d'un hacheur complet et d'une carte Nucleo-STM32G474RE, nous devons :

-Réaliser un shell pour commander le hacheur.

-Réaliser la commande des 4 transistors du hacheur en commande complémentaire décalée.

-Faire l'acquisition des différents capteurs.

-Réaliser l'asservissement en temps réel.



## TP1 Commande MCC basique

L'objectif est de générer quatre signaux PWM en commande complémentaire décalée afin de contrôler le moteur en boucle ouverte tout en respectant les spécifications du cahier des charges. Il s'agit également d'inclure un temps mort entre les commutations des transistors pour éviter les courts-circuits. Les signaux de commande devront être vérifiés à l'oscilloscope pour s'assurer de leur conformité. Parallèlement, une prise en main du hacheur sera effectuée, suivie d'un premier essai de commande du moteur.

## 1-Génération de 4 PWM

Nous allons générer quatre signaux PWM sur les bras de pont U et V pour contrôler le hacheur, en utilisant le timer déjà assigné à ces broches. Conformément au cahier des charges, la fréquence de la PWM doit être de 20 kHz, avec un temps mort minimum à déterminer en fonction des spécifications de la datasheet des transistors (la valeur devra être validée). De plus, une résolution minimale de 10 bits est requise pour assurer un contrôle précis.


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
### Commandes de vitesse 

Rajouter quelques fonctionnalités à votre projet :

Commande start : permet de fixer le rapport cyclique à 50% (vitesse nulle) et d'activer la génération des pwm (HAL_TIM_PWM_Start et HAL_TIMEx_PWMN_Start),
Commande stop : permet de désactiver la génération des PWM.
Commande speed XXXX : permet de définir le rapport cyclique à XXXX/PWM_MAX, mais afin de réduire l'appel à courant, vous devez établir une montée progressive à cette vitesse en quelques secondes. Vous pouvez effectuer une rampe entre la valeur actuelle et la valeur cible avec un incrément bien réfléchi de la PWM à un intervalle de temps régulier. Par la suite votre asservissement fera cela tout seul.

![image](https://github.com/user-attachments/assets/0539bb5e-770f-4dc4-9d0a-02c8bcea4e99)

Resultat:
![image](https://github.com/user-attachments/assets/69a8a22f-f437-4978-85f0-92d08a0418be)

#### Command stop:
![IMG_0255](https://github.com/user-attachments/assets/289d3670-bf3f-4150-846c-fbd5f82d5eea)
![IMG_0254](https://github.com/user-attachments/assets/32c70d61-40ad-4e78-b799-5b86022b2652)
![IMG_0253](https://github.com/user-attachments/assets/debf6df4-90cc-4812-b42c-67375ba65a98)
![IMG_0252](https://github.com/user-attachments/assets/4d588991-52a2-487e-943b-050a3d951e08)

#### Visualisation du temps mort (deadtime):
![IMG_0251](https://github.com/user-attachments/assets/f6ee5d4b-93b6-4b1c-8225-6bceb2372b45)
![IMG_0250](https://github.com/user-attachments/assets/c0e4051a-b270-4556-bc33-ba1938673bc8)

### Mesures du courant

Pour mesurer le courant, on utilise l'ADC. Selon les schématics de la carte, la mesure du courant se fait grace aux composants xxx, qui dans notre cas, est branché sur le PIN__ (ADC1)

## TP3



