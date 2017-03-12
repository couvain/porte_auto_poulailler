#ifndef __AUTOMATE_H__
#define __AUTOMATE_H__

#include <string>
#include "ptrfonction.h"


class CAutomate
{
public:
	static const int NBMAX_ETATS = 19;
	static const int NBMAX_TRANS = 29;
private:
	static const int ETAT_AUTOMATE_ETEINT = -1;
	static const int ETAT_INDEFINI        = -2;

	int m_etat_actuel;	// Etat actuel de l'automate
	int m_etat_final;	// Etat final de l'automate

	int m_nb_etats; 	// Nombre d'états de l'automate
	int m_nb_trans;		// Nombre de transitions
	int m_matrice[NBMAX_ETATS][NBMAX_TRANS];		// Tableau des états/transitions
	std::string m_nom_etat[NBMAX_ETATS]; 			// Nom de chacun des états
	
	PtrFnctVoidSansParam m_callback[NBMAX_ETATS];	// Tableau de pointeurs des fonctions appelées par chaque état

	// Le callback est uniquement appelé par l'automate
	void callback();
	void passe_en_etat(int p_numero_etat);
	
public:
	static const int TRANSITION_INDEFINIE = -1;
	CAutomate();
	~CAutomate();
	void demarre();
	void affiche();
	void ajoute_etat(std::string p_nom_etat, PtrFnctVoidSansParam p_fonction_callback);
	int ajoute_trans(int p_etat_debut,int p_etat_fin);
	void ajoute_trans_existante(int p_etat_debut,int p_etat_fin, int p_numero_trans);
	void execute_trans(int p_numero_trans);
	void set_etat_final(int p_numero_etat_final);
	bool termine();
	
	int get_etat_actuel();
	int get_nb_transitions_definies_pour_etat_actuel();
	int get_numero_transition_definie(int p_indice);
};

#endif
