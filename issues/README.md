## Issues ?

1. ```CtcPolar``` trop pessimiste par rapport à codac1
1. ```CtcInverse``` template deduction : sometimes list of Intervals, sometimes IntervalVector
1. Pas d'inverse de la fonction modulo ?
1. Problème opérations scalaires / Eigen à l'instanciation : ```error: static assertion failed: BITWISE OPERATIONS MAY ONLY BE PERFORMED ON PLAIN DATA TYPES```
1. ```CtcRelaxed``` Nombre de boites connue au moment de la compilation -> pas réaliste
    - Imposer un maximum au lieu d'un nombre précis
    - Reprendre l'algorithme : 
        - Argument par défaut = ensemble vide (vérifier que ça n'impacte pas le résultat)
        - Méthode pour définir le nombre de mesures réelles (si nécessaire), ou bien renseigner la différence n - q au lieu de q
