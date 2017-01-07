# Porte automatique de poulailler
Ouverture et fermeture automatiques d'une porte de poulailler via un Raspberry Pi A+


## Remarques
Le projet n'est pas optimal, ni même le moins cher, mais il a le mérite d'être un bon projet pour prendre en main un raspberry Pi pour en faire une utilisation d'automate programmable. Et puis un poulailler qui tourne en C++, ça ne court pas les rues! ;-)

## Choix de la technologie
C++ a été préféré à Python de façon totalement arbitraire, parce que je préfère le compilé à l'interprété, pour des raisons de performances, d'absence d'erreur de syntaxe détectable à la compilation, etc...
Mais je suis certain qu'on peut faire la même chose en python ou dans un autre langage.

## Consommation électrique
L'objectif est de rendre le système autonome énergétiquement: panneau solaire + batterie 12 V
Le Raspberry Pi A+ consomme très peu et c'est la raison principale de son choix. Le raspberry Pi Zero n'a pas été retenu parce qu'il est très difficile de s'en procurer un.
La connexion des circuits électroniques annexes a été optimisée pour consommer le moins possible.

## Spécifications
Le schéma du diagramme d'états de l'automate d'états finis explique le fonctionnement du programme C++ (a été réalisé avec yEd -> https://www.yworks.com/products/yed )
