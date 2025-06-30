#include "../include/Echiquier.h"
#include <iostream>
#include <string>
using namespace std;

void Echiquier::initialise_echiquier()
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            echiquier[i][j] = nullptr;
        }
    }
}
void Echiquier::pose_piece(piece *p, square pos)
{
    echiquier[pos.ligne][pos.colonne] = p;
}

Echiquier::Echiquier()
{
    initialise_echiquier();

    pieces[Blanc] = {
        new rook(Blanc, "\u2656", 0, square(0, 0)),
        new knight(Blanc, "\u2658", 1, square(0, 1)),
        new bishop(Blanc, "\u2657", 2, square(0, 2)),
        new queen(Blanc, "\u2655", 3, square(0, 3)),
        new king(Blanc, "\u2654", 4, square(0, 4)),
        new bishop(Blanc, "\u2657", 5, square(0, 5)),
        new knight(Blanc, "\u2658", 6, square(0, 6)),
        new rook(Blanc, "\u2656", 7, square(0, 7)),
    };
    pieces[Noir] = {
        new rook(Noir, "\u265C", 8, square(7, 0)),
        new knight(Noir, "\u265E", 9, square(7, 1)),
        new bishop(Noir, "\u265D", 10, square(7, 2)),
        new queen(Noir, "\u265B", 11, square(7, 3)),
        new king(Noir, "\u265A", 12, square(7, 4)),
        new bishop(Noir, "\u265D", 13, square(7, 5)),
        new knight(Noir, "\u265E", 14, square(7, 6)),
        new rook(Noir, "\u265C", 15, square(7, 7)),
    };
    for (size_t i = 0; i < 8; i++) {
        pieces[Blanc].push_back(
            new pawn(Blanc, "\u2659" + string(1, i), 16 + i, square(1, i)));
        pieces[Noir].push_back(
            new pawn(Noir, "\u265F" + string(1, i), 24 + i, square(6, i)));
    }
    for (auto p : pieces[Blanc])
        pose_piece(p, p->get_pos());
    for (auto p : pieces[Noir])
        pose_piece(p, p->get_pos());
}

void Echiquier::free_echiquier()
{
    for (int i = 0; i < NBCOL; i++) {
        for (int j = 0; j < NBCOL; j++) {
            if (echiquier[i][j])
                delete echiquier[i][j];
        }
    }
}

void Echiquier::deplacePiece(square depart, square arrivee)
{
    piece *p = echiquier[depart.ligne][depart.colonne];
    
    if (p->en_passant) {
        // verifie si la piece est un pion et si elle est en passant
        // si c'est le cas,  on supprime la piece 
        if (p->get_couleur() == Blanc) {
            echiquier[arrivee.ligne - 1][arrivee.colonne] = nullptr;
            p->en_passant = false;
        } else {
            echiquier[arrivee.ligne + 1][arrivee.colonne] = nullptr;
            p->en_passant = false;
        }
    }
    // on renitialise le last_move_pawn de toutes les pieces sauf 
    // celle qui est en train de se deplacer
    // on met à jour la position de la piece
    for (auto a : pieces[Blanc]) {
        
        if (a->last_move_pawn && p != a) {
            a->last_move_pawn = false;
        }
    }
    for (auto a : pieces[Noir]) {
        if (a->last_move_pawn && p != a) {
            a->last_move_pawn = false;
        }
    }

    echiquier[arrivee.ligne][arrivee.colonne] = p;
    echiquier[depart.ligne][depart.colonne] = nullptr;
    p->piece_deplace = true;
}
bool Echiquier::check_square_threat(square s, Echiquier *echiquier, couleur_t joueur)
{
    couleur_t color = joueur;
    Echiquier *clone_echuiquier = echiquier->copy_Echiquier();
    for (int i = 0; i < NBCOL; i++) {
        for (int j = 0; j < NBCOL; j++) {

            if (echiquier->echiquier[i][j] &&
                echiquier->echiquier[i][j]->get_couleur() != color) {
                square depart(i, j);
                square arrivee(s.ligne, s.colonne);
                if (echiquier->echiquier[i][j]->est_mouvement_legal(
                        depart, arrivee, echiquier->echiquier)) {
                    delete clone_echuiquier;
                    return true;
                }
            }
        }
    }
    delete clone_echuiquier;
    return false;
}

void Echiquier::affiche()
{

    string space5 = string(5, ' ');
    cout << endl;
    cout << "     a     b     c     d     e     f     g     h    " << endl;
    cout << "  +-----+-----+-----+-----+-----+-----+-----+-----+" << endl;
    for (int i(NBCOL - 1); i >= 0; i--) {
        cout << i + 1 << " "; // numérotation ligne dans affichage
        for (int j(0); j < NBCOL; j++) {
            cout << "|";
            if (echiquier[i][j]) {
                cout << "\u0020\u0020"; // U+0020 est un esapce utf-8 taille police
                echiquier[i][j]->afficher();
                cout << "\u0020"
                     << " ";
            } else
                cout << space5; // 2 ascii spaces
        }
        cout << "|\n  +-----+-----+-----+-----+-----+-----+-----+-----+";
        cout << endl;
    }
}
string Echiquier::pgn_piece_name(string const name, bool view_pawn, bool view_color) const
{

    string psymb;
    if (name == "\u2656")
        psymb = "R"; // Rook W
    else if (name == "\u2658")
        psymb = "N"; // Knight W
    else if (name == "\u2657")
        psymb = "B"; // Bishop W
    else if (name == "\u2655")
        psymb = "Q"; // Queen W
    else if (name == "\u2654")
        psymb = "K"; // King W
    else if (name.rfind("\u2659", 0) == 0 && view_pawn)
        psymb = "P";        // Pawn W
    if (psymb.size() > 0) { // one of the white piece has been found
        if (view_color)
            return "w" + psymb;
        else
            return psymb;
    }
    if (name == "\u265C")
        psymb = "R"; // Rook B
    else if (name == "\u265E")
        psymb = "N"; // Knight B
    else if (name == "\u265D")
        psymb = "B"; // Bishop B
    else if (name == "\u265B")
        psymb = "Q"; // Queen B
    else if (name == "\u265A")
        psymb = "K"; // King B
    else if (name.rfind("\u265F", 0) == 0 && view_pawn)
        psymb = "P";        // Pawn B
    if (psymb.size() > 0) { // one of the black piece has been found
        if (view_color)
            return "b" + psymb;
        else
            return psymb;
    } else
        return "";
}
string Echiquier::canonical_position() const
{
    string output;
    for (size_t row(1); row <= 8; row++) {
        for (char col('a'); col <= 'h'; col++) {
            square square(col + to_string(row));
            if (echiquier[square.ligne][square.colonne])
                // get pieces with theit PGN names,
                // true -> with P for pawns, true -> w/b for colors.

                output += pgn_piece_name(
                    echiquier[square.ligne][square.colonne]->get_nom(), true, true);
            output += ",";
        }
    }
    return output;
}

Echiquier *Echiquier::copy_Echiquier()
{
    Echiquier *copy = new Echiquier();
    for (int i = 0; i < NBCOL; i++) {
        for (int j = 0; j < NBCOL; j++) {
            if (echiquier[i][j]) {
                copy->pose_piece(echiquier[i][j]->clone(), square(i, j));
            } else {
                copy->echiquier[i][j] = nullptr;
            }
        }
    }
    return copy;
}
