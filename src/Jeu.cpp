#include "../include/Jeu.h"


#include <iostream>
#include <ostream>
#include <regex>
#include <string>
using namespace std;

bool Jeu::saisie_correcte(string const &cmd)
{
    regex mouvmtpattern("[a-h][1-8][a-h][1-8]");
    return regex_match(cmd, mouvmtpattern);
}

bool Jeu::saisie_correcte_petitroque(string const &cmd)
{
    regex mouvmtpattern("(O|o|0)-(O|o|0)");
    return regex_match(cmd, mouvmtpattern);
}

bool Jeu::saisie_correcte_grandroque(string const &cmd)
{
    regex mouvmtpattern("(O|o|0)-(O|o|0)-(O|o|0)");
    return regex_match(cmd, mouvmtpattern);
}
bool Jeu::check_move(string move)
{
    if (saisie_correcte(move) || saisie_correcte_petitroque(move) ||
        saisie_correcte_grandroque(move)) {

        if (saisie_correcte_petitroque(move) || saisie_correcte_grandroque(move)) {
            roque_next_move = true;
        }
        valid_move = true;
        return true;
    } else {
        if (move == "/quit") {
            return false;
        } else if (move == "/draw") {
            game_result = "1/2-1/2";
            return false;
        } else if (move == "/resign") {
            if (joueur == Blanc) {
                game_result = "0-1";
            } else {
                game_result = "1-0";
            }
            return false;
        } else {
            cerr << "Mouvement illegal : //  wrong format  //" << endl;
            valid_move = false;
            return true;
        }
    }
}

Jeu::Jeu()
{
    echiquier = Echiquier();
    joueur = Blanc;
}
void Jeu::free_echiquier() { echiquier.free_echiquier(); }

void Jeu::affiche() { echiquier.affiche(); }

string Jeu::affichage_canonique() { return echiquier.canonical_position(); }

void Jeu::deplace(string orig, string dest)
{
    square depart(orig);
    square arrivee(dest);
    if (!echiquier.echiquier[depart.ligne][depart.colonne]) {
        cerr << "Mouvement illegal : //  no piece in origine square  //" << endl;
        return;
    } else if (echiquier.echiquier[depart.ligne][depart.colonne]->get_couleur() !=
               joueur) {
        cerr << "Mouvement illegal : //  wrong player  //" << endl;
        return;
    }

    else if (!echiquier.echiquier[depart.ligne][depart.colonne]->est_mouvement_legal(
                 depart, arrivee, echiquier.echiquier)) {
        cerr << "Mouvement illegal : //  illegal move  //" << endl;
        return;
    }

    else if (echiquier.echiquier[depart.ligne][depart.colonne]->est_mouvement_legal(
                 depart, arrivee, echiquier.echiquier)) {
        if (echiquier.echiquier[depart.ligne][depart.colonne]->next_move_promotion) {
            echiquier.echiquier[depart.ligne][depart.colonne]->promotion(
                depart, echiquier.echiquier);
        }
        echiquier.deplacePiece(depart, arrivee);
        joueur = (joueur == Blanc) ? Noir : Blanc;
    } else {
        cerr << "Mouvement illegal : //  unknown error  //" << endl;
    }
}

bool Jeu::check_echec(Echiquier echiquier)
{
    Echiquier *clone_echuiquier = echiquier.copy_Echiquier();
    couleur_t color = joueur;
    square king_pos = get_king_pos(color, clone_echuiquier);
    for (int i = 0; i < NBCOL; i++) {
        for (int j = 0; j < NBCOL; j++) {

            if (clone_echuiquier->echiquier[i][j] &&
                clone_echuiquier->echiquier[i][j]->get_couleur() != color) {
                square depart(i, j);
                square arrivee(king_pos.ligne, king_pos.colonne);
                if (clone_echuiquier->echiquier[i][j]->est_mouvement_legal(
                        depart, arrivee, clone_echuiquier->echiquier)) {

                    delete clone_echuiquier;
                    king_checked = true;
                    return true;
                }
            }
        }
    }
    delete clone_echuiquier;
    king_checked = false;
    return false;
}
square Jeu::get_king_pos(couleur_t color, Echiquier *echiquier)
{

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (echiquier->echiquier[i][j] && echiquier->echiquier[i][j]->id == 4 &&
                echiquier->echiquier[i][j]->get_couleur() == color) {
                return square(i, j);
            } else if (echiquier->echiquier[i][j] &&
                       echiquier->echiquier[i][j]->id == 12 &&
                       echiquier->echiquier[i][j]->get_couleur() == color) {
                return square(i, j);
            }
        }
    }
    return square(-1, -1);
}
bool Jeu::check_move_echec(Echiquier echiquier, string orig, string dest)
{
    Echiquier *clone_echuiquier = echiquier.copy_Echiquier();
    square depart(orig);
    int i = depart.ligne;
    int j = depart.colonne;
    square arrivee(dest);
    if (!clone_echuiquier->echiquier[depart.ligne][depart.colonne]) {
        delete clone_echuiquier;
        return true;
    }
    if (clone_echuiquier->echiquier[i][j]->est_mouvement_legal(
            depart, arrivee, clone_echuiquier->echiquier)) {
        // si le deplacement est legal, on effectue le deplacement
        clone_echuiquier->deplacePiece(depart, arrivee);
    } else {
        delete clone_echuiquier;
        return true;
    }
    if (check_echec(*clone_echuiquier)) {
        // si le roi est en echec, alors le deplacement est illegal
        delete clone_echuiquier;
        return false;
    }
    // si le roi n'est pas en echec, alors le deplacement est legal
    delete clone_echuiquier;
    return true;
}

bool Jeu::deplace_roque(string move)
{

    if (joueur == Blanc) {

        if (move == "O-O") {
            if (!echiquier.echiquier[0][4] || !echiquier.echiquier[0][7]) {
                cerr << "Mouvement illegal : //  no piece in origine square  //" << endl;
                return false;
            }
            if (echiquier.echiquier[0][4]->piece_deplace ||
                echiquier.echiquier[0][7]->piece_deplace) {
                cerr << "Mouvement illegal : //  roque impossible  //" << endl;
                return false;
            }
            if (echiquier.echiquier[0][7]->est_mouvement_legal(square(0, 7), square(0, 5),
                                                               echiquier.echiquier)) {

                if (echiquier.check_square_threat(square(0, 5), &echiquier, joueur) ||
                    echiquier.check_square_threat(square(0, 6), &echiquier, joueur)) {
                    cerr << "Mouvement illegal : //  roque impossible  //" << endl;
                    return false;
                }
                echiquier.deplacePiece(square(0, 4), square(0, 6));
                echiquier.deplacePiece(square(0, 7), square(0, 5));
                echiquier.echiquier[0][6]->piece_deplace = true;
                echiquier.echiquier[0][5]->piece_deplace = true;
                joueur = Noir;
                return true;
            } else {
                cerr << "Mouvement illegal : //  roque impossible  //" << endl;
                return false;
            }

        } else if (move == "O-O-O") {
            if (!echiquier.echiquier[0][4] || !echiquier.echiquier[0][0]) {
                cerr << "Mouvement illegal : //  no piece in origine square  //" << endl;
                return false;
            }
            if (echiquier.echiquier[0][4]->piece_deplace ||
                echiquier.echiquier[0][0]->piece_deplace) {
                cerr << "Mouvement illegal : //  roque impossible  //" << endl;
                return false;
            }
            if (echiquier.echiquier[0][0]->est_mouvement_legal(square(0, 0), square(0, 3),
                                                               echiquier.echiquier)) {
                if (echiquier.check_square_threat(square(0, 1), &echiquier, joueur) ||
                    echiquier.check_square_threat(square(0, 3), &echiquier, joueur) ||
                    echiquier.check_square_threat(square(0, 2), &echiquier, joueur)) {
                    cerr << "Mouvement illegal : //  roque impossible  //" << endl;
                    return false;
                }
                echiquier.deplacePiece(square(0, 4), square(0, 2));
                echiquier.deplacePiece(square(0, 0), square(0, 3));
                echiquier.echiquier[0][2]->piece_deplace = true;
                echiquier.echiquier[0][3]->piece_deplace = true;
                joueur = Noir;
                return true;
            } else {
                cerr << "Mouvement illegal : //  roque impossible  //" << endl;
                return false;
            }
        }
    }
    if (joueur == Noir) {
        if (move == "O-O") {
            if (!echiquier.echiquier[7][4] || !echiquier.echiquier[7][7]) {
                cerr << "Mouvement illegal : //  no piece in origine square  //" << endl;
                return false;
            }
            if (echiquier.echiquier[7][4]->piece_deplace ||
                echiquier.echiquier[7][7]->piece_deplace) {
                cerr << "Mouvement illegal : //  roque impossible  //" << endl;
                return false;
            }
            if (echiquier.echiquier[7][7]->est_mouvement_legal(square(7, 7), square(7, 5),
                                                               echiquier.echiquier)) {
                if (echiquier.check_square_threat(square(7, 5), &echiquier, joueur) ||
                    echiquier.check_square_threat(square(7, 6), &echiquier, joueur)) {
                    cerr << "Mouvement illegal : //  roque impossible  //" << endl;
                    return false;
                }
                echiquier.deplacePiece(square(7, 4), square(7, 6));
                echiquier.deplacePiece(square(7, 7), square(7, 5));
                echiquier.echiquier[7][6]->piece_deplace = true;
                echiquier.echiquier[7][5]->piece_deplace = true;
                joueur = Blanc;
                return true;
            } else {
                cerr << "Mouvement illegal : //  roque impossible  //" << endl;
                return false;
            }
        } else if (move == "O-O-O") {
            if (!echiquier.echiquier[7][4] || !echiquier.echiquier[7][0]) {
                cerr << "Mouvement illegal : //  no piece in origine square  //" << endl;
                return false;
            }
            if (echiquier.echiquier[7][4]->piece_deplace ||
                echiquier.echiquier[7][0]->piece_deplace) {
                cerr << "Mouvement illegal : //  roque impossible  //" << endl;
                return false;
            }
            if (echiquier.echiquier[7][0]->est_mouvement_legal(square(7, 0), square(7, 3),
                                                               echiquier.echiquier)) {
                if (echiquier.check_square_threat(square(7, 1), &echiquier, joueur) ||
                    echiquier.check_square_threat(square(7, 3), &echiquier, joueur) ||
                    echiquier.check_square_threat(square(7, 2), &echiquier, joueur)) {
                    cerr << "Mouvement illegal : //  roque impossible  //" << endl;
                    return false;
                }
                echiquier.deplacePiece(square(7, 4), square(7, 2));
                echiquier.deplacePiece(square(7, 0), square(7, 3));
                echiquier.echiquier[7][2]->piece_deplace = true;
                echiquier.echiquier[7][3]->piece_deplace = true;
                joueur = Blanc;
                return true;
            } else {
                cerr << "Mouvement illegal : //  roque impossible  //" << endl;
                return false;
            }
        }
    }

    return false;
}

bool Jeu::check_mat(Echiquier echiquier)
{
    Echiquier *clone_echiquier = echiquier.copy_Echiquier();
    couleur_t color = joueur;
    for (int i = 0; i < NBCOL; i++) {
        for (int j = 0; j < NBCOL; j++) {

            if (clone_echiquier->echiquier[i][j] &&
                clone_echiquier->echiquier[i][j]->get_couleur() == color) {
                square depart(i, j);
                for (int k = 0; k < NBCOL; k++) {
                    for (int l = 0; l < NBCOL; l++) {

                        square arrivee(k, l);
                        if ((clone_echiquier->echiquier[k][l] &&
                             clone_echiquier->echiquier[k][l]->get_couleur() != color) ||
                            !clone_echiquier->echiquier[k][l]) {
                            // check si arrivee contient une piece de couleur
                            // differente, ou si elle est vide
                            if (clone_echiquier->echiquier[i][j]->est_mouvement_legal(
                                    depart, arrivee, clone_echiquier->echiquier)) {

                                if (check_move_echec(*clone_echiquier, depart.toString(),
                                                     arrivee.toString())) {
                                    // check si le deplacement met le roi en
                                    // echec si check_move_echec rtourne True,
                                    // alors il existe un deplacement qui ne met
                                    // pas le roi en echec
                                    delete clone_echiquier;
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // si on sort de la boucle, alors il n'existe aucun deplacement possible qui
    // ne met pas le roi en echec donc le roi est en echec et mat
    delete clone_echiquier;
    return true;
}
bool Jeu::check_pat(Echiquier echiquier)
{
    Echiquier *clone_echiquier = echiquier.copy_Echiquier();
    couleur_t color = joueur;
    for (int i = 0; i < NBCOL; i++) {
        for (int j = 0; j < NBCOL; j++) {

            if (clone_echiquier->echiquier[i][j] &&
                clone_echiquier->echiquier[i][j]->get_couleur() == color) {
                square depart(i, j);
                for (int k = 0; k < NBCOL; k++) {
                    for (int l = 0; l < NBCOL; l++) {

                        square arrivee(k, l);
                        if ((clone_echiquier->echiquier[k][l] &&
                             clone_echiquier->echiquier[k][l]->get_couleur() != color) ||
                            !clone_echiquier->echiquier[k][l]) {
                            // check si arrivee contient une piece de couleur
                            // differente, ou si elle est vide
                            if (clone_echiquier->echiquier[i][j]->est_mouvement_legal(
                                    depart, arrivee, clone_echiquier->echiquier) &&
                                check_move_echec(*clone_echiquier, depart.toString(),
                                                 arrivee.toString())) {
                                // check si le deplacement est légal et qui ne
                                // met pas le roi en echec si check_move_echec
                                // rtourne True, alors il existe un deplacement
                                // qui ne met pas le roi en echec

                                delete clone_echiquier;
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    // si on sort de la boucle, alors il n'existe aucun deplacement possible qui
    // ne met pas le roi en echec donc le roi est en pat

    delete clone_echiquier;
    return true;
}