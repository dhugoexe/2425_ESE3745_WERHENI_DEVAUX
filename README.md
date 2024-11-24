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

## 2 Mesure du courant
## les fonctions de transfert des capteurs de mesure de courant (lecture datasheet)
Une fonction de transfert établit une relation entre le courant mesuré par un capteur et la tension de sortie qu'il génère, permettant ainsi de convertir les lectures analogiques en valeurs physiques exploitables. Dans ce cas, la tension 𝑈 est d'abord calculée à partir de la valeur brute de l'ADC grâce à la formule :

![image](https://github.com/user-attachments/assets/fa77fcd3-5660-427e-a0ed-6f742ce1548c)

où 3.3V représente la tension de référence et 4096 correspond à la résolution du convertisseur analogique-numérique (12 bits).
Ensuite, la tension 𝑈 est reliée au courant I via l'équation caractéristique du capteur :

![image](https://github.com/user-attachments/assets/c5869c58-db34-42a8-b9ad-a3ed7487eb29)

où 1.65V est l'offset pour un courant nul, et 0.05 V/A est la sensibilité du capteur.

## les pin du stm32 utilisés pour faire ces mesures de courant

La broche PA2 est utilisée pour mesurer le courant dans le circuit. Elle est configurée en entrée analogique et reliée à un convertisseur analogique-numérique (ADC) du STM32, permettant de convertir le signal analogique du capteur de courant en une valeur numérique exploitable par le microcontrôleur.

## Etablir une première mesure de courant avec les ADC en Pooling.

Ce code mesure le courant en utilisant un ADC en mode polling et transmet le résultat via l'UART. Après avoir démarré et attendu la fin de la conversion avec HAL_ADC_Start et HAL_ADC_PollForConversion, la valeur brute de l'ADC est lue et convertie en tension, puis en courant à l'aide de la fonction de transfert. Le courant calculé est ensuite formaté et transmis via HAL_UART_Transmit pour affichage

![image](https://github.com/user-attachments/assets/3c1bd830-430f-43c1-ae0a-4e8adc56eb34)


## Configuration de pin ADC1

![image](https://github.com/user-attachments/assets/c2cfcbbd-df29-41fb-baac-c036590696c2)


## Resultat sur Tera Term 

![image](https://github.com/user-attachments/assets/28457f7d-9714-4755-8dac-a68b5404c58f)

## Mesure à interval de temps régulier avec la mise en place d'une la chaine d'acquisition Timer/ADC/DMA.

## Configuratn de l'ADC

![image](https://github.com/user-attachments/assets/94fcf43a-2bba-48ca-b77c-dd3f561434e5)

![image](https://github.com/user-attachments/assets/0e9ddabe-9ec4-442e-9c68-936472224025)

![image](https://github.com/user-attachments/assets/4df5d405-624f-4e06-aef7-a48d51922341)

## Interuption 

Ce callback gère la fin d'une conversion ADC en mode DMA. Il calcule la moyenne des valeurs ADC stockées dans le buffer, met à jour une variable d'état pour signaler la fin de la conversion, et désactive le DMA. Une notification est également envoyée via l'UART pour signaler la complétion.

![image](https://github.com/user-attachments/assets/9ac4ee32-489c-4000-8f26-9582c7889bad)

## Resultat sur Tera Term 

![Capture d'écran 2024-11-20 172019](https://github.com/user-attachments/assets/1545228f-ff1e-41c7-ab40-882f2b31d2ae)

## 3-Mesure de vitesse

Pour Mesure de vitesse de Rotation Nous Allons utulisé la pin PA6 et PA4 a l'aide de Timer3 

## Configuration de Timer 3

![image](https://github.com/user-attachments/assets/bb9cc296-9eb1-4b7b-a41f-203978bde164)

## Code de mesure de Vitesse 

La fonction Encoder_Init initialise le mode encodeur du timer htim1 avec HAL_TIM_Encoder_Start, permettant de commencer à compter les impulsions provenant de l'encodeur. Elle enregistre également la dernière valeur du compteur et le temps correspondant pour des calculs ultérieurs. 

![image](https://github.com/user-attachments/assets/23721beb-723c-4f7d-8f95-ff2cc5db7bda)

La fonction Encoder_GetFilteredSpeed calcule une vitesse filtrée : elle récupère la vitesse brute avec Encoder_GetSpeed, stocke cette valeur dans un buffer circulaire de taille FILTER_SIZE, et calcule la moyenne des valeurs du buffer. Cette moyenne est ensuite retournée, fournissant une mesure de vitesse lissée pour minimiser l'impact des fluctuations instantanées.

![image](https://github.com/user-attachments/assets/ac897a73-5b19-4dfa-bc09-79fd7bfe50ec)

## Conclusion

Dans ce TP, nous allons commander le moteur en boucle ouverte tout en limitant l'accélération afin d'assurer un démarrage progressif et de minimiser les variations brusques de courant. Ensuite, nous mesurerons le courant aux points stratégiques du montage pour analyser la consommation en fonction des conditions de fonctionnement. Enfin, la vitesse sera mesurée à l'aide du Encodeur.

## TP3 Asservissement

L'objectif de cette partie est d'établir l'asservissement du moteur, tant en vitesse qu'en courant. Nous devons d'abord mettre en place un système de régulation de la vitesse du moteur afin de garantir qu'il suive la consigne définie, en utilisant un contrôleur adapté, comme un PID. Ensuite, il est nécessaire de réguler le courant pour éviter les surcharges et assurer un fonctionnement optimal du moteur, en ajustant la commande en fonction de la mesure du courant réel. Ces deux asservissements permettront de maintenir des performances stables et fiables du moteur dans différentes conditions de fonctionnement.

Pour ce faire, vous pouvez utiliser un contrôleur PID ou un contrôleur à deux boucles (vitesse et courant). Voici les grandes étapes :

## Asservissement en vitesse du moteur :

Nous Utiliseons le codeur du moteur pour obtenir la vitesse en temps réel (en comptant les impulsions sur une période donnée).

*Calcul de l'erreur de vitesse :
Comparez la vitesse mesurée avec la consigne de vitesse Vconsigne,L'erreur de vitesse 𝑒𝑣 sera :

![image](https://github.com/user-attachments/assets/dd2a0c92-b303-44bd-b077-791d3ba23c3a)

Un contrôleur PID est utilisé pour ajuster la commande du moteur en fonction de l'erreur de vitesse. Le contrôleur PID se compose de trois termes :

-Proportionnel (P) : Correction proportionnelle à l'erreur.

-Intégral (I) : Compensation de l'erreur accumulée au fil du temps.

-Dérivé (D) : Atténuation des variations rapides ou des oscillations.

L'algorithme PID ajuste la commande pour réduire l'erreur à zéro et stabiliser la vitesse du moteur, en offrant un contrôle précis et réactif.

## Asservissement en courant du moteur :

L’asservissement en courant est essentiel pour réguler la puissance délivrée au moteur, en particulier afin d'éviter les surcharges et de garantir un fonctionnement stable. En règle générale, l'asservissement en courant peut être effectué en boucle ouverte pour une régulation simple ou en boucle fermée avec un contrôleur PID, comme c'est le cas pour notre application.

*Calcul de l'erreur de courant :

Comparez le courant mesuré avec la consigne de courant Iconsigne. L'erreur de courant 𝑒𝑐 sera:

![image](https://github.com/user-attachments/assets/59ade4fe-754f-444d-9311-6c4a357eadb5)

Un contrôleur PID (ou un contrôleur proportionnel dans certains cas) est ensuite utilisé pour ajuster la commande de courant. Ce contrôleur permet de moduler le signal PWM appliqué au moteur, afin de réguler le courant et de maintenir la valeur désirée en fonction de l'erreur mesurée.

## Conclusion

La conclusion de cette partie est que l'asservissement en vitesse et en courant est essentiel pour assurer un contrôle précis et stable du moteur. L'asservissement en vitesse permet de maintenir la vitesse du moteur à la valeur souhaitée, en ajustant le signal de commande via un contrôleur PID. De même, l'asservissement en courant régule la puissance délivrée au moteur, évitant ainsi les surcharges et garantissant des performances optimales. Ces deux boucles de contrôle, basées sur les mesures de la vitesse et du courant, contribuent à un fonctionnement fiable et sécurisé du moteur dans diverses conditions d'exploitation.





