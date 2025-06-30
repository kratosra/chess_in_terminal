

# Jeu d'échecs by JADI Ahmed Amine

1. [Compilation](#compilation)
    1. [Installation](#installation)
    2. [Documentation](#documentation)
    3. [Tests](#tests)
2. [Exécution](#exécution)
    1. [Démarrage](#démarrage)
    2. [Fonctionnement du jeu](#fonctionnement-du-jeu)
        1. [Déplacements classiques](#déplacements-classiques)
        2. [Roque](#roque)
        3. [Promotion](#promotion)
        4. [En passant](#en-passant)
        5. [Détection de l'échec](#détection-de-léchec)
        6. [Détection de l'échec et mat](#détection-de-léchec-et-mat)
        7. [Détection de pat](#détection-de-pat)
3. [Conception ](#conception)
      1. [Architecture](#architecture)
      2. [Conception du programme](#conception-du-programme)
      




## Compilation

### Installation

Pour compiler le programme, vous pouvez utiliser le Makefile fourni en exécutant la commande suivante :

```bash
make
```
Cela entraînera la compilation du programme et le placement de l'exécutable résultant dans le répertoire bin.


### Documentation

Pour générer la documentation du programme, exécutez simplement la commande suivante :

```bash
make doc
```
Une fois la documentation générée, un dossier HTML sera créé et automatiquement ouvert dans votre navigateur par défaut. Si ce n'est pas le cas, vous pouvez toujours accéder à la documentation en ouvrant le fichier html/index.html dans votre navigateur.

### Tests

Pour exécuter les tests du programme à l'aide des fichiers de tests fournis, vous pouvez utiliser la commande suivante :

```bash
make tests
```
> Le programme sera compilé et testé et les résultats seront affichés dans le terminal.

## Exécution
### Démarrage

Pour démarrer le programme, exécutez la commande suivante dans votre terminal :

```bash
./bin/echecs
```
 
### Fonctionnement du jeu

Une fois le programme démarré, l'échiquier initial sera affiché et il attendra la saisie d'un coup.
les coups sont sous la forme :

 [a-h][1-8][a-h][1-8]

par exemple, pour déplacer le pion en e2 à e4, vous saisirez e2e4.

il faut s'assurer que le coup est légal, sinon le programme vous demandera de ressaisir un coup.

#### Déplacements classiques
Pour effectuer un déplacement classique, saisissez une chaîne de caractères dans le terminal au format e2e4, où e2 représente la case de départ et e4 la case d'arrivée.

#### En passant
Si un pion atteint la case d'un pion adverse ayant effectué un double saut lors du tour précédent, vous pouvez effectuer un en passant. Le programme détectera automatiquement cette possibilité.

#### Roque
Si vous souhaitez effectuer un roque, saisissez dans le terminal O-O pour un petit roque ou O-O-O pour un grand roque.

#### Promotion
Lorsqu'un pion atteint la dernière ligne, il peut être promu en une autre pièce. Vous devrez alors saisir le caractère correspondant à la pièce souhaitée dans le terminal.

pour choisir le choix de la promotion, vous devez saisir le caractère correspondant à la pièce souhaitée parmi les suivants :

N pour un cavalier;
B pour un fou;
R pour une tour;
Q pour une dame;

#### Détection de l'échec
À la fin de chaque tour, le programme détectera si votre roi est en échec et vous en informera dans le terminal.

#### Détection de l'échec et mat
En cas d'échec, le programme vérifiera s'il s'agit également d'un échec et mat. Si tel est le cas, il vous informera également dans le terminal.

#### Détection de pat
Le programme détectera également les situations de pat, c'est-à-dire lorsque le joueur ne peut effectuer aucun mouvement légal.

## Conception

### Architecture
Le programme est conçu en utilisant la programmation orientée objet en C++.

Le programme est structuré en plusieurs classes, chacune représentant un élément du jeu d'échecs. Voici un aperçu des classes principales :

- **square** : classe représentant une case de l'échiquier
- **Piece** : classe abstraite représentant une pièce d'échecs
- **Pion, Tour, Cavalier, Fou, Dame, Roi** : classes héritant de la classe Piece et représentant les différentes pièces du jeu
- **Echiquier** : classe représentant l'échiquier et contenant les pièces
- **Jeu** : classe gérant le déroulement du jeu et les règles associées

### Conception du programme

Le programme est conçu pour offrir une expérience interactive permettant à l'utilisateur de jouer contre un adversaire. Il implémente les règles fondamentales du jeu d'échecs, tout en prenant en charge les déplacements spéciaux tels que le roque, la promotion et l'en passant.

Le développement du programme a suivi une approche méthodique. J'ai débuté par la conception et l'implémentation des classes de base représentant les pièces d'échecs et l'échiquier lui-même. Ensuite, j'ai ajouté les règles de déplacement spécifiques à chaque type de pièce, en utilisant la méthode virtuelle est_mouvement_legal() héritée par chaque pièce.

Par la suite, j'ai étendu le programme pour inclure la fonctionnalité de déplacement des pièces, en veillant à ce qu'elles respectent les règles établies. J'ai ensuite intégré la détection de l'échec et des mouvements menaçant le roi, renforçant ainsi la gestion des situations critiques du jeu.

Plus tard, j'ai enrichi le programme en ajoutant la prise en passant, la promotion des pions et la gestion du roque, offrant ainsi une expérience de jeu plus complète et fidèle aux règles officielles.

Enfin, j'ai complété le programme en implémentant la gestion de la fin de partie, incluant la détection de l'échec et mat ainsi que celle du pat. Ceci permet aux joueurs de conclure les parties de manière appropriée selon les règles du jeu d'échecs.
