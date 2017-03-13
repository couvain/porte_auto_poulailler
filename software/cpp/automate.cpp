#include "automate.h"
#include <iostream>
#include <string>
#include <iomanip>
#include "colors.h"
#include "log.h"

//*****************************************************************************
//
// Constructeur
//
//-----------------------------------------------------------------------------
CAutomate::CAutomate()
{
	int i,j;

	m_etat_actuel = ETAT_AUTOMATE_ETEINT; // L'automate n'est pas démarré
	m_nb_etats = 0;
	m_nb_trans = 0;
	m_etat_final = -1;

	// Réinitialisation de la matrice [etats/transitions]
	for (i = 0 ; i< NBMAX_ETATS;i++)
	{
		for (j = 0; j < NBMAX_TRANS; j++)
		{
			m_matrice[i][j] = ETAT_INDEFINI;
		}
	}
}

//*****************************************************************************
//
// Destructeur
//
//-----------------------------------------------------------------------------
CAutomate::~CAutomate()
{
}

//*****************************************************************************
//
//
//
//-----------------------------------------------------------------------------
int CAutomate::get_etat_actuel()
{
	return m_etat_actuel;
}

//*****************************************************************************
//
//
//
//-----------------------------------------------------------------------------
int CAutomate::get_nb_transitions_definies_pour_etat_actuel()
{
	int l_nb_transitions = 0;
	int i;

	for (i = 0 ; i < m_nb_trans ; i++)
	{
		if (m_matrice[m_etat_actuel][i] != CAutomate::ETAT_INDEFINI)
		{
			l_nb_transitions++;
		}
	}

	return l_nb_transitions;
}

//*****************************************************************************
//
// Retourne l'indice de la i-ème transition définie, partant de l'état courant
//
//-----------------------------------------------------------------------------
int CAutomate::get_numero_transition_definie(int p_indice)
{
	int i            = 0;
	int l_compteur   = 0;
	int l_nb_trans   = get_nb_transitions_definies_pour_etat_actuel();
	int l_transition = CAutomate::TRANSITION_INDEFINIE;
	int l_etat_suivant;

	if ((p_indice > l_nb_trans) || (p_indice < 1))
	{
		LOG(ERROR) << "CAutomate::get_numero_transition - indice ("<< p_indice << ") hors limites [1;" << l_nb_trans << "]";
		return -1;
	}

	// tant qu'on n'est pas arrivé à la "p_indice"-ième transition définie
	//while ((l_compteur != p_indice) && (i<(NBMAX_TRANS-1)))
	while ((l_compteur != p_indice) && (i<(NBMAX_TRANS)))
	{
		// On teste la i-ème transition
		l_etat_suivant = m_matrice[m_etat_actuel][i];

		if (l_etat_suivant != CAutomate::ETAT_INDEFINI)
		{
			// On a trouvé une transition définie
			l_compteur++;
			l_transition = i;
		}
		i++;
	}

	return l_transition;
}

//*****************************************************************************
//
// Définit quel état est l'état final de l'automate qui arrête le programme
//
//-----------------------------------------------------------------------------
void CAutomate::set_etat_final(int p_numero_etat_final)
{
	if ((p_numero_etat_final < 0) || (p_numero_etat_final >= m_nb_etats))
	{
		LOG(WARNING) << "L'état final de l'automate doit être compris entre 0 (inclus) et " << (m_nb_etats-1) << "(inclus)";
	}
	else
	{
		LOG(INFO) << "[INFO] Définition de l'état final de l'automate : "<< p_numero_etat_final;
		m_etat_final = p_numero_etat_final;
	}
}

//*****************************************************************************
//
// Indique si on est dans l'état final
//
//-----------------------------------------------------------------------------
bool CAutomate::termine()
{
	if (m_etat_actuel == m_etat_final)
	{
		LOG(INFO) << "Arrivée en état final [" << m_etat_final << "] - Arrêt de l'automate.";
		return true;
	}
	else
	{
		return false;
	}
}

//*****************************************************************************
//
// Démarre l'automate
//
//-----------------------------------------------------------------------------
void CAutomate::demarre()
{
	if (m_etat_actuel == ETAT_AUTOMATE_ETEINT)
	{
		LOG(INFO) << "---------------------------------------------------------";
		LOG(INFO) << "Démarrage de l'automate";
		// On se met en état 0 (initial) et on lance la fonction callback associée
		passe_en_etat(0);
		affiche();
	}
	else
	{
		// L'automate est déjà démarré
		LOG(WARNING) << "L'automate est déjà démarré. Impossible de le démarrer une deuxième fois.";
	}
}


//*****************************************************************************
//
//
//
//-----------------------------------------------------------------------------
void CAutomate::passe_en_etat(int p_numero_etat)
{
	LOG(INFO) << "L'automate passe en état " << p_numero_etat << ": " <<  m_nom_etat[p_numero_etat];

	m_etat_actuel = p_numero_etat;
	callback();
}

//*****************************************************************************
//
// Appel de la fonction callback associée à l'état courant m_etat
//
//-----------------------------------------------------------------------------
void CAutomate::callback()
{
	LOG(INFO) << "Etat " << m_etat_actuel << ": appel de la fonction callback associée";
	if (m_callback[m_etat_actuel] != 0)
	{
		m_callback[m_etat_actuel]();
	}
	else
	{
		LOG(WARNING) << "Etat " << m_etat_actuel << ": Pas de fonction callback associée";
	}
}

//*****************************************************************************
//
//
//
//-----------------------------------------------------------------------------
void CAutomate::affiche()
{
	int i,j;

	if (m_etat_actuel == ETAT_AUTOMATE_ETEINT)
	{
		std::cout << "Automate arrêté.\n\n";
	}
	else
	{
		//std::cout << "[INFO] Automate en etat " << m_etat_actuel << ": " << m_nom_etat[m_etat_actuel] << std::endl << std::endl;

		std::cout << "Transition   ";
		for (j =0; j < m_nb_trans; j++)
		{
			std::cout << std::setw(2) << j << " ";
		}
		std::cout << std::endl;

		for (i = 0 ; i< m_nb_etats;i++)
		{
			if (i == m_etat_actuel)
			{
				std::cout << KBOLD << KRED << "Etat [" << std::setw(2) << i << "]:  [";
			}
			else
			{
				std::cout << "Etat  " << std::setw(2) << i << " :  [";
			}
			for (j =0; j < m_nb_trans; j++)
			{
				if (m_matrice[i][j] == ETAT_INDEFINI)
				{
					std::cout << "__ ";
				}
				else
				{
					std::cout << std::setw(2) << m_matrice[i][j] << " ";
				}
			}
			if (i == m_etat_actuel)
			{
				std::cout << "] -> [" << m_nom_etat[i] << "] <-" << RST << RST;
			}
			else
			{
				std::cout << "]    " << m_nom_etat[i];
			}

			if (i == m_etat_final)
			{
				std::cout << " FINAL";
			}

			std::cout << "\n";
		} // for
	}
	std::cout << std::endl;
}

//*****************************************************************************
//
// Fonction de construction de l'automate
//
//-----------------------------------------------------------------------------
void CAutomate::ajoute_etat(std::string p_nom_etat, PtrFnctVoidSansParam p_fonction_callback)
{
	int l_numero_etat_ajoute;

	l_numero_etat_ajoute = m_nb_etats;
	if (l_numero_etat_ajoute < NBMAX_ETATS)
	{
		m_nb_etats++;
		m_nom_etat[l_numero_etat_ajoute] = p_nom_etat;
		m_callback[l_numero_etat_ajoute] = p_fonction_callback;

		LOG(INFO) << "Etat " << l_numero_etat_ajoute << " ajouté (" << p_nom_etat << ")";
	}
	else
	{
		LOG(WARNING) << "Impossible d'ajouter l'état " << l_numero_etat_ajoute << " Dépassement de la limite de NBMAX_ETATS=" << NBMAX_ETATS << " états possibles";
	}
}

//*****************************************************************************
//
// Fonction de construction de l'automate
//
//-----------------------------------------------------------------------------
int CAutomate::ajoute_trans(int p_etat_debut,int p_etat_fin)
{
	int l_numero_trans_ajoutee;

	l_numero_trans_ajoutee = m_nb_trans;
	if (l_numero_trans_ajoutee < NBMAX_TRANS)
	{
		if ((p_etat_debut < NBMAX_ETATS) && 
			(p_etat_fin < NBMAX_ETATS) &&
			(p_etat_debut >= 0) && 
			(p_etat_fin >= 0))
		{
			m_matrice[p_etat_debut][l_numero_trans_ajoutee] = p_etat_fin;
			m_nb_trans++;

			LOG(INFO) << "Transition " << l_numero_trans_ajoutee << " ajoutée ( " << p_etat_debut << " -> " << p_etat_fin << " )";
		}
		else
		{
			LOG(WARNING) << "Impossible d'ajouter la transition " << l_numero_trans_ajoutee << " numéros d'états hors limites";
			return -1;
		}
	}
	else
	{
		LOG(WARNING) << "Impossible d'ajouter la transition " << l_numero_trans_ajoutee << " Dépassement de la limite de NBMAX_TRANS=" << NBMAX_TRANS << " transitions possibles";
		return -1;
	}
	return l_numero_trans_ajoutee;
}

//*****************************************************************************
//
// Fonction de construction de l'automate
//
//-----------------------------------------------------------------------------
void CAutomate::ajoute_trans_existante(int p_etat_debut,int p_etat_fin, int p_numero_trans)
{
	int l_numero_trans_ajoutee;

	l_numero_trans_ajoutee = p_numero_trans;
	if (l_numero_trans_ajoutee < NBMAX_TRANS)
	{
		if ((p_etat_debut < NBMAX_ETATS) && 
			(p_etat_fin < NBMAX_ETATS) &&
			(p_etat_debut >= 0) && 
			(p_etat_fin >= 0))
		{
			m_matrice[p_etat_debut][l_numero_trans_ajoutee] = p_etat_fin;

			LOG(INFO) << "Transition " << l_numero_trans_ajoutee << " ajoutée ( " << p_etat_debut << " -> " << p_etat_fin << " )";
		}
		else
		{
			LOG(WARNING) << "Impossible d'ajouter la transition " << l_numero_trans_ajoutee << " numéros d'états hors limites";
		}
	}
	else
	{
		LOG(WARNING) << "Impossible d'ajouter la transition " << l_numero_trans_ajoutee << " Dépassement de la limite de NBMAX_TRANS=" << NBMAX_TRANS << " transitions possibles";
	}
}

//*****************************************************************************
//
//
//
//-----------------------------------------------------------------------------
void CAutomate::execute_trans(int p_numero_trans)
{
	int p_etat_suivant = ETAT_INDEFINI;

	if (m_etat_actuel == ETAT_AUTOMATE_ETEINT)
	{
		// L'automate est arrêté
		LOG(WARNING) << "Impossible d'exécuter la transition " << p_numero_trans << " : l'automate est arrêté. Veuillez le démarrer d'abord.";
	}
	else
	{
		// L'automate est démarré

		if (p_numero_trans == TRANSITION_INDEFINIE)
		{
			// Aucune transition n'a été détectée donc on ne fait rien
			return;
		}

		if ((p_numero_trans < 0) || (p_numero_trans > m_nb_trans))
		{
			LOG(WARNING) << "Impossible d'exécuter la transition " << p_numero_trans << " : hors limites";
		}
		else
		{
			LOG(INFO) << "---------------------------------------------------------";
			LOG(INFO) << "Execution de la transition " << p_numero_trans;
			std::cout << "Execution de la transition " << p_numero_trans << "\n" << std::endl;
			p_etat_suivant = m_matrice[m_etat_actuel][p_numero_trans];

			if (p_etat_suivant == ETAT_INDEFINI)
			{
				LOG(INFO) << " -> Aucun effet - l'automate reste en état " << m_etat_actuel;
			}
			else
			{
				passe_en_etat(p_etat_suivant);
			}
		}
	}
}
