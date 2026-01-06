## Issues ?

1. ```CtcPolar``` trop pessimiste par rapport à codac1
1. ```CtcInverse``` template deduction : sometimes list of Intervals, sometimes IntervalVector
1. Pas d'inverse de la fonction modulo ?
1. Problème opérations scalaires / Eigen à l'instanciation : ```error: static assertion failed: BITWISE OPERATIONS MAY ONLY BE PERFORMED ON PLAIN DATA TYPES```
1. ```CtcRelaxed``` Nombre de boites connue au moment de la compilation -> pas réaliste
    - Imposer un maximum au lieu d'un nombre précis ? Renseigner la différence n - q au lieu de q (mais il faut toujours un maximum pour initialiser Ctc) ?
    - Fonction contract avec d'autres arguments ?? **Problématique pour le CRTP...**
    - **Ajouter des tests avec différentes dimensions**
1. ```CtcQInter_clara``` : ajout de setters à la class CtcQInter
