#ifndef __RELAIS_H__
#define __RELAIS_H__

#include "GPIO.h"

class CRelais
{
private:
	CGPIO * m_gpio;
	bool	m_etat; // true = activé / false = désactivé

public:
	static const bool ETAT_ACTIVE    = true;
	static const bool ETAT_DESACTIVE = false;
	CRelais(int p_num_gpio);
	~CRelais();

	void initialiser();
	void activer();
	void desactiver();	
	bool get_etat();
};

#endif
