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

![image](https://github.com/user-attachments/assets/c2d5c240-0cdb-4c6b-94b8-5139e6a4bb4f)

![image](https://github.com/user-attachments/assets/2295c7fb-d774-4ec0-b1f9-40547e2d3ec9)


Pour obtenir un rapport cyclique de 60 %, nous avons modifié les registres CCR1 du Timer 1 et CCR2 du Timer 2. Les résultats de cette configuration sont illustrés dans la figure ci-dessous :

![image](https://github.com/user-attachments/assets/dfb90150-d83b-4bfb-b6d1-ac2a709d8126)

Démonstation de temps mort egale a 100ns sur l'oscilloscope:

![image](https://github.com/user-attachments/assets/75ddcc5c-a24f-4d21-9e86-191b17867080)


##  Commande de vitesse 

Pour controler la vitesse du moteur, nous allons envoyer une séquence via la liaison UART (par l'USB) de la forme :speed XXXX

Le traitement de cette chaine de caractère se faire de la manière suivant :
-Détection des premiers caractères "speed"
-Conversion de tous les caractères représentant des chiffres XXXX en nombre entier
-Vérification de la valeur (si la valeur est supérieur au maximum autorisé (bien spécifier cette valeur), on l'abaisse à cette valeur),
-Application de cette vitesse au moteur à travers le registre gérant le rapport cyclique de la PWM


![Capture d'écran 2024-11-06 184250](https://github.com/user-attachments/assets/53832d32-45e6-4761-9a1e-c18a8bd5741d)

## Les Problèmes potentiels que nous allons observés :

-Instabilité : Le changement brutal de puissance peut provoquer des oscillations ou des comportements imprévisibles.

-Surchauffe : Si le courant augmente trop rapidement, cela peut provoquer une surchauffe dans certains composants (transistors, résistances, etc.).

-À-coups : Pour un moteur, une variation brutale du rapport cyclique peut entraîner des mouvements brusques, causant des contraintes mécaniques ou un manque de précision.

## Solution :

Nous allons générer une montée progressive du rapport cyclique afin d'atteindre la vitesse cible. Nous commençons par tester si la vitesse spécifiée respecte les limites définies par les constantes MAX_SPEED et MIN_SPEED. Si la valeur dépasse la limite maximale, elle est plafonnée à MAX_SPEED. Inversement, si elle est inférieure à la limite minimale, elle est ajustée pour correspondre à MIN_SPEED. Une fois validée, la vitesse est appliquée au système en utilisant la fonction setSpeed, et la vitesse réellement appliquée est stockée dans la variable currentSpeed.

![image](https://github.com/user-attachments/assets/b58f7d49-51f4-44d9-97b2-e627a53c2a3e)

La fonction setSpeed(uint8_t speed) ajuste progressivement la vitesse actuelle (currentSpeed) vers la vitesse cible (speed). Si la vitesse cible est supérieure, elle augmente currentSpeed étape par étape, en ajustant les registres CCR1 et CCR2 pour moduler le rapport cyclique. Si la vitesse cible est inférieure, elle réduit currentSpeed de manière similaire. À chaque incrément ou décrément, un délai de 100 ms est ajouté pour garantir une transition fluide. Une fois la vitesse cible atteinte, la fonction retourne la vitesse actuelle.

![image](https://github.com/user-attachments/assets/9c9b9361-4951-49da-9f64-aa3d4bde8794)


## Conclusion

À partir de ce TP, nous allons générer quatre signaux PWM en complémentaire décalée pour contrôler le moteur en boucle ouverte, tout en respectant le cahier des charges. Cela inclut l'ajout du temps mort et la mise en place d'une commande progressive de la vitesse du moteur.

![Capture d'écran 2024-11-13 162355](https://github.com/user-attachments/assets/716adabf-5032-41f8-aae9-cf2fdf899cb9)



![Capture d'écran 2024-11-13 162355](https://github.com/user-attachments/assets/6edce119-dbec-453e-9168-16da37e74e04)



![IMG_0243](https://github.com/user-attachments/assets/e43b6472-9f8c-4ed9-a330-3deb1754be1c)
![IMG_0238](https://github.com/user-attachments/assets/e5b2945f-b053-441c-bef5-8e218e16560f)


## TP2 Commande en boucle ouverte, mesure de Vitesse et de courant

Dans cette partie Nous devons :

-Commander en boucle ouverte le moteur avec une accélération limitée,
-Mesurer le courant aux endroits adéquat dans le montage,
-Mesurer la vitesse à partir du codeur présent sur chaque moteur.

![image](https://github.com/user-attachments/assets/b58f7d49-51f4-44d9-97b2-e627a53c2a3e)

### Commandes de vitesse 

Rajouter quelques fonctionnalités à votre projet :

*Commande start : permet de fixer le rapport cyclique à 50% (vitesse nulle) et d'activer la génération des pwm (HAL_TIM_PWM_Start et HAL_TIMEx_PWMN_Start),

![image](https://github.com/user-attachments/assets/5e4c713b-fcf4-49fb-83b0-99988303f4f4)

->Ce code démarre un système PWM lorsque la commande "start" est détectée. Il active les signaux PWM sur deux canaux du timer htim1 avec un rapport cyclique de 50 %. Ensuite, il met à jour une variable currentSpeed pour indiquer une vitesse de fonctionnement de 50 %.

*Commande stop : permet de désactiver la génération des PWM.

![image](https://github.com/user-attachments/assets/c0de0c24-eb29-46ea-a523-8329ff082d5d)

Ce code arrête le système PWM lorsque la commande "stop" est détectée. Il désactive les signaux PWM sur les canaux 1 et 2 du timer htim1

Commande speed XXXX : permet de définir le rapport cyclique à XXXX/PWM_MAX, mais afin de réduire l'appel à courant, Nous devons établir une montée progressive à cette vitesse en quelques secondes.

![image](https://github.com/user-attachments/assets/b58f7d49-51f4-44d9-97b2-e627a53c2a3e)

Resultat sur Tera Terminal:
![image](https://github.com/user-attachments/assets/69a8a22f-f437-4978-85f0-92d08a0418be)

## Pour Start:

![image](https://github.com/user-attachments/assets/e09cf92d-87dc-4532-ad1a-7b1a397ee0ce)

## Pour Stop:

![image](https://github.com/user-attachments/assets/50ebab21-297f-4133-a7c3-931b29f07395)

## Pour Speed=40

![image](https://github.com/user-attachments/assets/0bb81bde-7592-49ba-810a-9ff3cad2b149)

## Pour Speed=60

![image](https://github.com/user-attachments/assets/99a2c38c-5148-4f79-8df3-6c879cb4802a)









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



