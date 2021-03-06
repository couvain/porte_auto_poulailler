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
	}
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
CRelais::~CRelais()
{
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

	// Désactiver le relais
	desactiver();
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void CRelais::activer()
{
	// Logique inversée à cause de la carte relais utilisée
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

//**********************************************************************
//
// Arrêt d'urgence, on met le GPIO en mode INPUT
//
//----------------------------------------------------------------------
void CRelais::arret_urgence()
{
	desactiver();
	m_gpio->set_direction_input();
}
