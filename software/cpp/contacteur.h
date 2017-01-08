#ifndef __CONTACTEUR_H__
#define __CONTACTEUR_H__

#include "GPIO.h"

// Classe définissant un matériel physique comme par exemple un 
// interrupteur ou un capteur de fin de course, un matériel physique 
// pouvant prendre les deux états binaires: ouvert ou fermé

class CContacteur
{
public:

	static const bool ETAT_FERME  = true;
	static const bool ETAT_OUVERT = false;

private:

	bool m_etat;
	CGPIO * m_gpio;

public:

	CContacteur(int p_numero_gpio);	
	~CContacteur();	
	bool get_etat();
};

#endif
