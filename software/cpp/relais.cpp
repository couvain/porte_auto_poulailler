#include "relais.h"
#include <string>

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
CRelais::CRelais(int p_num_gpio)
{
	m_gpio = new CGPIO(p_num_gpio);
	if (m_gpio != 0)
	{
		initialiser();
		m_etat = CRelais::ETAT_DESACTIVE;
	}
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
CRelais::~CRelais()
{
	m_gpio->set_value(false);
	delete(m_gpio);
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void CRelais::initialiser()
{
	m_gpio->export_gpio();
	// Un relais est un actionneur donc GPIO en mode OUT
	m_gpio->set_direction_output();
	// Désactiver le GPIO, si possible
	m_gpio->set_value(true);
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void CRelais::activer()
{
	m_gpio->set_value(false);
	m_etat = CRelais::ETAT_ACTIVE;
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void CRelais::desactiver()	
{
	m_gpio->set_value(true);
	m_etat = CRelais::ETAT_DESACTIVE;

}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
bool CRelais::get_etat()
{
	return m_etat;
}
