#include "contacteur.h"
#include "GPIO.h"
#include <string>


//**********************************************************************
//
//
//
//----------------------------------------------------------------------
CContacteur::CContacteur(int p_numero_gpio)
{
	m_gpio = new CGPIO(p_numero_gpio);
	if (m_gpio != 0)
	{
		// Mettre le GPIO en lecture (IN)
		m_gpio->set_direction_input();
		m_etat = m_gpio->get_value();
	}
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
CContacteur::~CContacteur()
{
	delete(m_gpio);
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
bool CContacteur::get_etat()
{
	// Récupérer l'état dans le GPIO
	m_etat = m_gpio->get_value();

	return m_etat;
}
