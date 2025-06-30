#include "../include/Jeu.h"
#include <iostream>
#include <regex>
#include <string>
using namespace std;

/**
 * @brief Fonction principale du programme.
 *
 * Cette fonction initialise le jeu, gère les mouvements des joueurs, vérifie les règles
 * du jeu (échec, échec et mat, pat) et affiche le résultat.
 *
 * @return 0 si le programme s'exécute avec succès.
 */
int main()
{
    Jeu monjeu; // initialise le jeu

    string mouvement;
    bool stop(false);
    monjeu.affiche();
    do {
        cout << "Coup (eg. a1a8) ? " << endl;
        cin >> mouvement;
        // vérifier si le mouvement est valide, sinon demander un autre mouvement
        if (monjeu.check_move(mouvement)) {
            if (monjeu.valid_move == false) {
                continue;
            }
            // vérifier si le prochain mouvement est un roque
            if (monjeu.roque_next_move) {
                // vérifier si le roque est valide, sinon demander un autre mouvement
                if (!monjeu.deplace_roque(mouvement)) {
                    cout << "Coup invalide" << endl;
                    monjeu.roque_next_move = false;
                    continue;
                }
                monjeu.roque_next_move = false;
                monjeu.affiche();
                continue;
            }
            // découper case origine et destination
            string orig = mouvement.substr(0, 2);
            string dest = mouvement.substr(2, 2);

            // vérifier si le roi est en échec ou si le mouvement met le roi en echec,
            // sinon demander un autre mouvement
            if (monjeu.king_checked ||
                !monjeu.check_move_echec(monjeu.echiquier, orig, dest)) {

                if (!monjeu.check_move_echec(monjeu.echiquier, orig, dest)) {
                    if (monjeu.king_checked) {
                        // si le roi est déjà en échec et le mouvement menace un echec,
                        // afficher "Le roi est en echec"
                        cout << "Le roi est en echec" << endl;
                    } else {
                        cout << "Le roi est en menace d'echec" << endl;
                    }
                    continue;
                }
            }
            monjeu.deplace(orig, dest);

            monjeu.affiche();

            monjeu.last_move = mouvement;

            // vérifier si le roi est en échec et mat
            if (monjeu.check_echec(monjeu.echiquier)) {
                if (monjeu.check_mat(monjeu.echiquier)) {
                    cout << "Echec et mat" << endl;
                    if (monjeu.joueur == Blanc) {
                        monjeu.game_result = "0-1";
                    } else {
                        monjeu.game_result = "1-0";
                    }
                    break;
                }
                monjeu.king_checked = true;
            }
            // vérifier si le jeu est en pat
            if (monjeu.check_pat(monjeu.echiquier)) {
                cout << "Pat" << endl;
                monjeu.game_result = "1/2-1/2";
                break;
            }

        } else
            stop = true;
    } while (!stop);
    cout << monjeu.affichage_canonique() << " " << monjeu.game_result << endl;

    return 0;
}
