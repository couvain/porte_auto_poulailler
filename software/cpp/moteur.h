#ifndef __MOTEUR_H__
#define __MOTEUR_H__

#include "relais.h"
// Classe créée pour un moteur électrique rotatif simple à courant continu

class CMoteur
{
private:
	// On utilise deux relais pour inverser le sens du courant et donc 
	// inverser le sens de rotation du moteur
	CRelais * m_relais_inverseur_A;
	CRelais * m_relais_inverseur_B;
	
	// on utilise un relais de "commande" permettant de lancer la 
	// rotation du moteur
	CRelais * m_relais_commande;

public:
	CMoteur(int p_gpio_inv_A, int p_gpio_inv_B, int p_gpio_comm);
	~CMoteur();
	void initialiser();
	void cabler_en_marche_avant();
	void cabler_en_marche_arriere();
	void demarrer();
	void stopper();
	
};

#endif
