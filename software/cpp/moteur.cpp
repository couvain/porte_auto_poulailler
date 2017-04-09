#include "moteur.h"
#include "relais.h"
#include "temps.h"
#include "log.h"
#include <string>
#include <iostream>

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
CMoteur::CMoteur(int p_gpio_inv_A, int p_gpio_inv_B, int p_gpio_comm)
{
	m_relais_inverseur_A = new CRelais(p_gpio_inv_A);
	m_relais_inverseur_B = new CRelais(p_gpio_inv_B);
	m_relais_commande = new CRelais(p_gpio_comm);
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
CMoteur::~CMoteur()
{
	delete(m_relais_inverseur_A);
	delete(m_relais_inverseur_B);
	delete(m_relais_commande);
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void CMoteur::initialiser()
{
	m_relais_inverseur_A->initialiser();
	m_relais_inverseur_B->initialiser();
	m_relais_commande->initialiser();
}

//**********************************************************************
//
// Ouvrir la porte
//
//----------------------------------------------------------------------
void CMoteur::cabler_en_marche_avant()
{
	// On arrête le moteur pour faire un câblage inverseur
	m_relais_commande->desactiver();

	CTemps::attendre(500); // Pause d'une demi-seconde

	m_relais_inverseur_A->activer();
	m_relais_inverseur_B->activer();
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void CMoteur::cabler_en_marche_arriere()
{
	// On arrête le moteur pour faire un câblage inverseur
	m_relais_commande->desactiver();

	CTemps::attendre(500); // Pause d'une demi-seconde

	m_relais_inverseur_A->desactiver();
	m_relais_inverseur_B->desactiver();
}
//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void CMoteur::demarrer()
{
	if (m_relais_inverseur_A->get_etat() == CRelais::ETAT_ACTIVE)
	{
		LOG(INFO) << "Démarrage du moteur en marche avant";
	}
	else
	{
		LOG(INFO) << "Démarrage du moteur en marche arrière";
	}

	// On démarre le moteur
	m_relais_commande->activer();
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void CMoteur::stopper()
{
	LOG(INFO) << "Arrêt du moteur";
	// On arrête le moteur
	m_relais_commande->desactiver();

	CTemps::attendre(500); // Pause d'une demi-seconde

	m_relais_inverseur_A->desactiver();
	m_relais_inverseur_B->desactiver();
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void CMoteur::arret_urgence()
{
	LOG(INFO) << "Arrêt d'urgence du moteur";
	// On arrête le moteur
	//m_relais_commande->desactiver();

	m_relais_inverseur_A->arret_urgence();
	m_relais_inverseur_B->arret_urgence();
	m_relais_commande->arret_urgence();

}
