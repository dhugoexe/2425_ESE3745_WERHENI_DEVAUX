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

## Configuration de pin
Nous utiliserons le Timer 1 (TIM1) pour générer les signaux PWM principaux sur les broches PA9 et PA8, ainsi que leurs signaux complémentaires sur les broches PA14 et PA13, comme le montre la figure ci-dessous

![image](https://github.com/user-attachments/assets/6e4ea078-e335-4641-81d5-670c1b0f5845)


## Configuration de Timer1

La configuration des broches dans STM32CubeMX est réalisée en activant le TIM1 et en assignant les canaux PWM (CH1, CH2, CH1N, CH2N) aux broches correspondantes, comme illustré dans la figure.

![image](https://github.com/user-attachments/assets/8e0e56e9-7708-4ce9-9416-2b8abd989fa8)

## Calcul de PRSC (Prescaler) pour une fréquence PWM de 20 kHz

L'équation de base pour calculer la fréquence d'une PWM est donnée par :

![image](https://github.com/user-attachments/assets/42374c96-c14c-43c1-8fbd-b617a2b02815)

où :

-fPWM est la fréquence de la PWM (20 kHz dans ce cas)

-fCLK est la fréquence de l'horloge (170 MHz)

-PSC est le prescaler (valeur recherchée)

-ARR est l'Auto-Reload Register (0 ici).

![image](https://github.com/user-attachments/assets/2243a5dc-26b7-46f2-8f1f-897aa8f3f9b2)

Finalement, PSC :  PSC=8500−1=8499

![image](https://github.com/user-attachments/assets/34842a06-e842-4b00-9adf-dc6d1f474cee)

## *Calcule de temps mort 
Nous allons commencer par calculer la période d'oscillation à partir de la fréquence, en utilisant la formule :
​
![image](https://github.com/user-attachments/assets/b015eafb-e4b8-4282-b36b-a8504f1a730f)

Ensuite, d'après la datasheet des transistors, nous prenons le temps de retard de conduction 𝑇dfg égal à 100 ns
Pour calculer le temps mort nous divisons le temps de retard 𝑇dfg par la période d'oscillation 𝑇

![image](https://github.com/user-attachments/assets/2295c7fb-d774-4ec0-b1f9-40547e2d3ec9)


![image](https://github.com/user-attachments/assets/c2d5c240-0cdb-4c6b-94b8-5139e6a4bb4f)




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



