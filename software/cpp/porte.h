#ifndef __PORTE_H__
#define __PORTE_H__

#include "moteur.h"

class CPorte
{
private:
	CMoteur * m_moteur;
public:
	CPorte(int p_gpio_inv_A, int p_gpio_inv_B, int p_gpio_comm);
	~CPorte();
	void initialiser();
	void ouvrir();
	void fermer();
	void stopper();
	void arret_urgence();
};

#endif
