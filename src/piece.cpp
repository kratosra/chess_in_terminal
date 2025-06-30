#include "../include/piece.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

piece::piece(couleur_t couleur, string nom, int id, square position)
    : couleur(couleur), nom(nom), id(id), position(position){};
piece::piece(const piece &piece)
    : couleur(piece.couleur), nom(piece.nom), id(piece.id), position(piece.position){};

piece::~piece(){};

void piece::afficher() { cout << nom; }

square piece::get_pos() { return position; }

couleur_t piece::get_couleur() { return couleur; }

string piece::get_nom() { return nom; }

pawn ::pawn(couleur_t couleur, string nom, int id, square position)
    : piece(couleur, nom, id, position){};

pawn::pawn(const pawn &pawn) : piece(pawn){};
pawn *pawn::clone() { return new pawn(*this); }

bool pawn::est_mouvement_legal(square case_origine, square case_destination,
                               piece *echiquier[8][8])
{
    if ((case_destination.ligne < 0 || case_destination.ligne >= NBCOL) ||
        (case_destination.colonne < 0 || case_destination.colonne >= NBCOL)) {
        return false;
    }
    if (echiquier[case_destination.ligne][case_destination.colonne] != nullptr) {
        if (echiquier[case_destination.ligne][case_destination.colonne]->get_couleur() ==
            echiquier[case_origine.ligne][case_origine.colonne]->get_couleur()) {
            return false;
        }
    }
    if (echiquier[case_origine.ligne][case_origine.colonne]->get_couleur() == Blanc) {
        if (echiquier[case_origine.ligne][case_destination.colonne] &&
            echiquier[case_origine.ligne][case_destination.colonne]->last_move_pawn &&
            echiquier[case_origine.ligne][case_destination.colonne]->get_couleur() ==
                Noir) {
            this->en_passant = true;
            this->last_move_pawn = false;
            if (case_destination.ligne == 7) {
                this->next_move_promotion = true;
            }
            return true;
        }
        if (case_origine.ligne == 1) {
            if (case_origine.colonne == case_destination.colonne &&
                (case_destination.ligne == case_origine.ligne + 1 ||
                 case_destination.ligne == case_origine.ligne + 2)) {
                if (case_destination.ligne == case_origine.ligne + 2) {
                    if (echiquier[case_origine.ligne + 1][case_origine.colonne] !=
                            nullptr ||
                        echiquier[case_origine.ligne + 2][case_origine.colonne] !=
                            nullptr) {
                        return false;
                    }
                } else if (case_destination.ligne == case_origine.ligne + 1) {
                    if (echiquier[case_origine.ligne + 1][case_origine.colonne] !=
                        nullptr) {
                        return false;
                    }
                }
                if (case_destination.ligne == case_origine.ligne + 2) {
                    this->last_move_pawn = true;
                }
                if (case_destination.ligne == 7) {
                    this->next_move_promotion = true;
                }
                return true;
            } else if (abs(case_origine.colonne - case_destination.colonne) == 1 &&
                       case_destination.ligne == case_origine.ligne + 1) {
                if (echiquier[case_destination.ligne][case_destination.colonne] ==
                    nullptr) {
                    return false;
                }
                if (case_destination.ligne == 7) {
                    this->next_move_promotion = true;
                }
                return true;
            } else {
                return false;
            }
        } else {
            if (case_origine.colonne == case_destination.colonne &&
                case_destination.ligne == case_origine.ligne + 1) {
                if (echiquier[case_destination.ligne][case_destination.colonne] !=
                    nullptr) {
                    return false;
                }
                if (case_destination.ligne == 7) {
                    this->next_move_promotion = true;
                }
                return true;
            } else if (abs(case_origine.colonne - case_destination.colonne) == 1 &&
                       case_destination.ligne == case_origine.ligne + 1) {
                if (echiquier[case_destination.ligne][case_destination.colonne] ==
                    nullptr) {
                    return false;
                }
                if (case_destination.ligne == 7) {
                    this->next_move_promotion = true;
                }
                return true;
            } else {
                return false;
            }
        }
    } else {
        if (echiquier[case_origine.ligne][case_destination.colonne] &&
            echiquier[case_origine.ligne][case_destination.colonne]->last_move_pawn &&
            echiquier[case_origine.ligne][case_destination.colonne]->get_couleur() ==
                Blanc) {
            this->en_passant = true;
            this->last_move_pawn = false;
            if (case_destination.ligne == 0) {
                this->next_move_promotion = true;
            }
            return true;
        }
        if (case_origine.ligne == 6) {
            if (case_origine.colonne == case_destination.colonne &&
                (case_destination.ligne == case_origine.ligne - 1 ||
                 case_destination.ligne == case_origine.ligne - 2)) {
                if (case_destination.ligne == case_origine.ligne - 2) {
                    if (echiquier[case_origine.ligne - 1][case_origine.colonne] !=
                            nullptr ||
                        echiquier[case_origine.ligne - 2][case_origine.colonne] !=
                            nullptr) {
                        return false;
                    }
                } else if (case_destination.ligne == case_origine.ligne - 1) {
                    if (echiquier[case_origine.ligne - 1][case_origine.colonne] !=
                        nullptr) {
                        return false;
                    }
                }
                if (case_destination.ligne == case_origine.ligne - 2) {
                    this->last_move_pawn = true;
                }
                if (case_destination.ligne == 0) {
                    this->next_move_promotion = true;
                }

                return true;
            } else if (abs(case_origine.colonne - case_destination.colonne) == 1 &&
                       case_destination.ligne == case_origine.ligne - 1) {
                if (echiquier[case_destination.ligne][case_destination.colonne] ==
                    nullptr) {
                    return false;
                }
                this->last_move_pawn = false;
                if (case_destination.ligne == 0) {
                    this->next_move_promotion = true;
                }
                return true;
            } else {
                return false;
            }
        } else {
            if (case_origine.colonne == case_destination.colonne &&
                case_destination.ligne == case_origine.ligne - 1) {
                if (echiquier[case_destination.ligne][case_destination.colonne] !=
                    nullptr) {
                    return false;
                }
                this->last_move_pawn = false;
                if (case_destination.ligne == 0) {
                    this->next_move_promotion = true;
                }
                return true;
            } else if (abs(case_origine.colonne - case_destination.colonne) == 1 &&
                       case_destination.ligne == case_origine.ligne - 1) {
                if (echiquier[case_destination.ligne][case_destination.colonne] ==
                    nullptr) {
                    return false;
                }
                if (case_destination.ligne == 0) {
                    this->next_move_promotion = true;
                }
                return true;
            } else {
                return false;
            }
        }
    }
    // en passant

    return false;
}

rook ::rook(couleur_t couleur, string nom, int id, square position)
    : piece(couleur, nom, id, position){};
rook::rook(const rook &rook) : piece(rook){};
rook *rook::clone() { return new rook(*this); }

bool rook::est_mouvement_legal(square case_origine, square case_destination,
                               piece *echiquier[8][8])
{
    if ((case_destination.ligne < 0 || case_destination.ligne >= NBCOL) ||
        (case_destination.colonne < 0 || case_destination.colonne >= NBCOL)) {
        return false;
    }
    if (echiquier[case_destination.ligne][case_destination.colonne] != nullptr) {
        if (echiquier[case_destination.ligne][case_destination.colonne]->get_couleur() ==
            echiquier[case_origine.ligne][case_origine.colonne]->get_couleur()) {
            return false;
        }
    }

    if (case_origine.ligne == case_destination.ligne &&
        case_origine.colonne != case_destination.colonne) {
        if (case_origine.colonne < case_destination.colonne) {
            for (int i = case_origine.colonne + 1; i < case_destination.colonne; i++) {
                if (echiquier[case_origine.ligne][i] != nullptr) {
                    return false;
                }
            }
        } else {
            for (int i = case_origine.colonne - 1; i > case_destination.colonne; i--) {
                if (echiquier[case_origine.ligne][i] != nullptr) {
                    return false;
                }
            }
        }
        return true;
    } else if (case_origine.colonne == case_destination.colonne &&
               case_origine.ligne != case_destination.ligne) {
        if (case_origine.ligne < case_destination.ligne) {
            for (int i = case_origine.ligne + 1; i < case_destination.ligne; i++) {
                if (echiquier[i][case_origine.colonne] != nullptr) {
                    return false;
                }
            }
        } else {
            for (int i = case_origine.ligne - 1; i > case_destination.ligne; i--) {
                if (echiquier[i][case_origine.colonne] != nullptr) {
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

knight ::knight(couleur_t couleur, string nom, int id, square position)
    : piece(couleur, nom, id, position){};
knight::knight(const knight &knight) : piece(knight){};
knight *knight::clone() { return new knight(*this); }
bool knight::est_mouvement_legal(square case_origine, square case_destination,
                                 piece *echiquier[8][8])
{
    if ((case_destination.ligne < 0 || case_destination.ligne >= NBCOL) ||
        (case_destination.colonne < 0 || case_destination.colonne >= NBCOL)) {
        return false;
    }
    if (echiquier[case_destination.ligne][case_destination.colonne] != nullptr) {
        if (echiquier[case_destination.ligne][case_destination.colonne]->get_couleur() ==
            echiquier[case_origine.ligne][case_origine.colonne]->get_couleur()) {
            return false;
        }
    }
    if ((case_destination.ligne == case_origine.ligne + 2 &&
         case_destination.colonne == case_origine.colonne + 1) ||
        (case_destination.ligne == case_origine.ligne + 2 &&
         case_destination.colonne == case_origine.colonne - 1) ||
        (case_destination.ligne == case_origine.ligne - 2 &&
         case_destination.colonne == case_origine.colonne + 1) ||
        (case_destination.ligne == case_origine.ligne - 2 &&
         case_destination.colonne == case_origine.colonne - 1) ||
        (case_destination.ligne == case_origine.ligne + 1 &&
         case_destination.colonne == case_origine.colonne + 2) ||
        (case_destination.ligne == case_origine.ligne + 1 &&
         case_destination.colonne == case_origine.colonne - 2) ||
        (case_destination.ligne == case_origine.ligne - 1 &&
         case_destination.colonne == case_origine.colonne + 2) ||
        (case_destination.ligne == case_origine.ligne - 1 &&
         case_destination.colonne == case_origine.colonne - 2)) {
        return true;
    } else {
        return false;
    }
}

bishop ::bishop(couleur_t couleur, string nom, int id, square position)
    : piece(couleur, nom, id, position){};
bishop::bishop(const bishop &bishop) : piece(bishop){};
bishop *bishop::clone() { return new bishop(*this); }
bool bishop::est_mouvement_legal(square case_origine, square case_destination,
                                 piece *echiquier[8][8])
{
    if ((case_destination.ligne < 0 || case_destination.ligne >= NBCOL) ||
        (case_destination.colonne < 0 || case_destination.colonne >= NBCOL)) {
        return false;
    }
    if (echiquier[case_destination.ligne][case_destination.colonne] != nullptr) {
        if (echiquier[case_destination.ligne][case_destination.colonne]->get_couleur() ==
            echiquier[case_origine.ligne][case_origine.colonne]->get_couleur()) {
            return false;
        }
    }
    if (abs(case_origine.ligne - case_destination.ligne) ==
        abs(case_origine.colonne - case_destination.colonne)) {
        if (case_origine.ligne < case_destination.ligne &&
            case_origine.colonne < case_destination.colonne) {
            for (int i = 1; i < abs(case_origine.ligne - case_destination.ligne); i++) {
                if (echiquier[case_origine.ligne + i][case_origine.colonne + i] !=
                    nullptr) {
                    return false;
                }
            }
        } else if (case_origine.ligne < case_destination.ligne &&
                   case_origine.colonne > case_destination.colonne) {
            for (int i = 1; i < abs(case_origine.ligne - case_destination.ligne); i++) {
                if (echiquier[case_origine.ligne + i][case_origine.colonne - i] !=
                    nullptr) {
                    return false;
                }
            }
        } else if (case_origine.ligne > case_destination.ligne &&
                   case_origine.colonne < case_destination.colonne) {
            for (int i = 1; i < abs(case_origine.ligne - case_destination.ligne); i++) {
                if (echiquier[case_origine.ligne - i][case_origine.colonne + i] !=
                    nullptr) {
                    return false;
                }
            }
        } else {
            for (int i = 1; i < abs(case_origine.ligne - case_destination.ligne); i++) {
                if (echiquier[case_origine.ligne - i][case_origine.colonne - i] !=
                    nullptr) {
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

queen ::queen(couleur_t couleur, string nom, int id, square position)
    : piece(couleur, nom, id, position){};
queen::queen(const queen &queen) : piece(queen){};
queen *queen::clone() { return new queen(*this); }
bool queen::est_mouvement_legal(square case_origine, square case_destination,
                                piece *echiquier[8][8])
{
    if ((case_destination.ligne < 0 || case_destination.ligne >= NBCOL) ||
        (case_destination.colonne < 0 || case_destination.colonne >= NBCOL)) {
        return false;
    }
    if (echiquier[case_destination.ligne][case_destination.colonne] != nullptr) {
        if (echiquier[case_destination.ligne][case_destination.colonne]->get_couleur() ==
            echiquier[case_origine.ligne][case_origine.colonne]->get_couleur()) {
            return false;
        }
    }
    if (case_origine.ligne == case_destination.ligne &&
        case_origine.colonne != case_destination.colonne) {
        if (case_origine.colonne < case_destination.colonne) {
            for (int i = case_origine.colonne + 1; i < case_destination.colonne; i++) {
                if (echiquier[case_origine.ligne][i] != nullptr) {
                    return false;
                }
            }
        } else {
            for (int i = case_origine.colonne - 1; i > case_destination.colonne; i--) {
                if (echiquier[case_origine.ligne][i] != nullptr) {
                    return false;
                }
            }
        }
        return true;
    } else if (case_origine.colonne == case_destination.colonne &&
               case_origine.ligne != case_destination.ligne) {
        if (case_origine.ligne < case_destination.ligne) {
            for (int i = case_origine.ligne + 1; i < case_destination.ligne; i++) {
                if (echiquier[i][case_origine.colonne] != nullptr) {
                    return false;
                }
            }
        } else {
            for (int i = case_origine.ligne - 1; i > case_destination.ligne; i--) {
                if (echiquier[i][case_origine.colonne] != nullptr) {
                    return false;
                }
            }
        }
        return true;
    } else if (abs(case_origine.ligne - case_destination.ligne) ==
               abs(case_origine.colonne - case_destination.colonne)) {
        if (case_origine.ligne < case_destination.ligne &&
            case_origine.colonne < case_destination.colonne) {
            for (int i = 1; i < abs(case_origine.ligne - case_destination.ligne); i++) {
                if (echiquier[case_origine.ligne + i][case_origine.colonne + i] !=
                    nullptr) {
                    return false;
                }
            }
        } else if (case_origine.ligne < case_destination.ligne &&
                   case_origine.colonne > case_destination.colonne) {
            for (int i = 1; i < abs(case_origine.ligne - case_destination.ligne); i++) {
                if (echiquier[case_origine.ligne + i][case_origine.colonne - i] !=
                    nullptr) {
                    return false;
                }
            }
        } else if (case_origine.ligne > case_destination.ligne &&
                   case_origine.colonne < case_destination.colonne) {
            for (int i = 1; i < abs(case_origine.ligne - case_destination.ligne); i++) {
                if (echiquier[case_origine.ligne - i][case_origine.colonne + i] !=
                    nullptr) {
                    return false;
                }
            }
        } else {
            for (int i = 1; i < abs(case_origine.ligne - case_destination.ligne); i++) {
                if (echiquier[case_origine.ligne - i][case_origine.colonne - i] !=
                    nullptr) {
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }
}
king ::king(couleur_t couleur, string nom, int id, square position)
    : piece(couleur, nom, id, position){};
king::king(const king &king) : piece(king){};
king *king::clone() { return new king(*this); }
bool king::est_mouvement_legal(square case_origine, square case_destination,
                               piece *echiquier[8][8])
{
    if ((case_destination.ligne < 0 || case_destination.ligne >= NBCOL) ||
        (case_destination.colonne < 0 || case_destination.colonne >= NBCOL)) {
        return false;
    }
    if (echiquier[case_destination.ligne][case_destination.colonne] != nullptr) {
        if (echiquier[case_destination.ligne][case_destination.colonne]->get_couleur() ==
            echiquier[case_origine.ligne][case_origine.colonne]->get_couleur()) {
            return false;
        }
    }
    if (abs(case_origine.ligne - case_destination.ligne) <= 1 &&
        abs(case_origine.colonne - case_destination.colonne) <= 1) {
        return true;
    } else {
        return false;
    }
}
void piece::promotion(square case_origin, piece *echiquier[8][8])
{
boucle:;
    char new_piece;
    int id;
    cout << "Promotion de pion" << endl;
    switch (echiquier[case_origin.ligne][case_origin.colonne]->get_couleur()) {
    case Blanc:
        if (case_origin.ligne == 6) {
            cout << "Choisissez la piece de promotion (Q, R, B, N)" << endl;
            std::cin >> new_piece;
            switch (new_piece) {
            case 'Q':
                id = echiquier[case_origin.ligne][case_origin.colonne]->id;
                delete echiquier[case_origin.ligne][case_origin.colonne];
                echiquier[case_origin.ligne][case_origin.colonne] =
                    new queen(Blanc, "\u2655", id, case_origin);
                break;
            case 'R':
                id = echiquier[case_origin.ligne][case_origin.colonne]->id;
                delete echiquier[case_origin.ligne][case_origin.colonne];
                echiquier[case_origin.ligne][case_origin.colonne] =
                    new rook(Blanc, "\u2656", id, case_origin);
                break;
            case 'B':
                id = echiquier[case_origin.ligne][case_origin.colonne]->id;
                delete echiquier[case_origin.ligne][case_origin.colonne];
                echiquier[case_origin.ligne][case_origin.colonne] =
                    new bishop(Blanc, "\u2657", id, case_origin);
                break;
            case 'N':
                id = echiquier[case_origin.ligne][case_origin.colonne]->id;
                delete echiquier[case_origin.ligne][case_origin.colonne];
                echiquier[case_origin.ligne][case_origin.colonne] =
                    new knight(Blanc, "\u2658", id, case_origin);
                break;
            default:
                cout << "Choix invalide" << endl;
                goto boucle;
                break;
            }
        } else {
            return;
        }

        break;

    case Noir:
        if (case_origin.ligne == 1) {
            cout << "Choisissez la piece de promotion (Q, R, B, N)" << endl;
            std::cin >> new_piece;
            switch (new_piece) {
            case 'Q':
                id = echiquier[case_origin.ligne][case_origin.colonne]->id;
                delete echiquier[case_origin.ligne][case_origin.colonne];
                echiquier[case_origin.ligne][case_origin.colonne] =
                    new queen(Noir, "\u265B", id, case_origin);
                break;
            case 'R':
                id = echiquier[case_origin.ligne][case_origin.colonne]->id;
                delete echiquier[case_origin.ligne][case_origin.colonne];
                echiquier[case_origin.ligne][case_origin.colonne] =
                    new rook(Noir, "\u265C", id, case_origin);
                break;
            case 'B':
                id = echiquier[case_origin.ligne][case_origin.colonne]->id;
                delete echiquier[case_origin.ligne][case_origin.colonne];
                echiquier[case_origin.ligne][case_origin.colonne] =
                    new bishop(Noir, "\u265D", id, case_origin);
                break;
            case 'N':
                id = echiquier[case_origin.ligne][case_origin.colonne]->id;
                delete echiquier[case_origin.ligne][case_origin.colonne];
                echiquier[case_origin.ligne][case_origin.colonne] =
                    new knight(Noir, "\u265E", id, case_origin);
                break;
            default:
                cout << "Choix invalide" << endl;
                goto boucle;
                break;
            }

        } else {
            return;
        }
        break;
    default:
        cout << "Choix invalide" << endl;
        goto boucle;
        break;
    }
}