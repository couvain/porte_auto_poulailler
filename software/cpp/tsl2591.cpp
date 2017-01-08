#include "tsl2591.h"
#include "busi2c.h"
#include "log.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <unistd.h>


//**********************************************************************
//
// Constructeur
//
//----------------------------------------------------------------------
CTSL2591::CTSL2591()
{
	m_bus 						= 0;
	m_tsl2591_initialized 		= false;
	m_tsl2591_integration_time 	= TSL2591_INTEGRATION_TIME_100MS;
	m_tsl2591_gain 				= TSL2591_GAIN_LOW;
}

//**********************************************************************
//
// Destructeur
//
//----------------------------------------------------------------------
CTSL2591::~CTSL2591()
{
}

//**********************************************************************
//
// Affectation du bus I2C
//
//----------------------------------------------------------------------
void CTSL2591::setBusI2C(CBusI2C * p_bus)
{
	m_bus = p_bus;
}

//**********************************************************************
//
// Vérification du bon fonctionnement du capteur TSL2591
// (procédure begin)
//----------------------------------------------------------------------
bool CTSL2591::tsl2591_initialize()
{
	uint8_t l_registre;
	uint8_t l_buffer;
	bool l_resultat = false;

	m_tsl2591_initialized = false;
	l_registre = TSL2591_COMMAND_BIT | TSL2591_REGISTER_DEVICE_ID;

	l_buffer = m_bus->read8(l_registre);
	if ( 0x50 == ((unsigned int) l_buffer) )
	{
		l_resultat = true;
		m_tsl2591_initialized = true;
		//LOG(INFO) << "tsl2591_initialize: Capteur TSL2591 initialisé OK";
	}
	else
	{
		LOG(ERROR) << "tsl2591_initialize: Erreur d'initialisation du capteur TSL2591";
	}

	return l_resultat;
}

//**********************************************************************
//
// Activation du capteur
//
//----------------------------------------------------------------------
void CTSL2591::enable()
{
	//std::cout << "enable:";
	if (! m_tsl2591_initialized)
	{
		if (! tsl2591_initialize())
		{
			return;
		}

	}
	//m_bus->write8( TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE, TSL2591_ENABLE_POWERON | TSL2591_ENABLE_AEN | TSL2591_ENABLE_AIEN | TSL2591_ENABLE_NPIEN);
	m_bus->write8( TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE, TSL2591_ENABLE_POWERON | TSL2591_ENABLE_AEN );
}

//**********************************************************************
//
// Désactivation du capteur
//
//----------------------------------------------------------------------
void CTSL2591::disable()
{
	//std::cout << "disable:";
	if (! m_tsl2591_initialized)
	{
		if (! tsl2591_initialize())
		{
			return;
		}
	}
	m_bus->write8( TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE, TSL2591_ENABLE_POWEROFF);
}

//**********************************************************************
//
// 
//
//----------------------------------------------------------------------
void CTSL2591::setGain(tsl2591Gain_t p_gain)
{
	//LOG(INFO) << "CTSL2591::setGain";
	if (! m_tsl2591_initialized)
	{
		if (! tsl2591_initialize())
		{
			return;
		}
	}
	enable();
	m_tsl2591_gain = p_gain;
	m_bus->write8( TSL2591_COMMAND_BIT | TSL2591_REGISTER_CONTROL, m_tsl2591_integration_time | m_tsl2591_gain);
	disable();
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void CTSL2591::setIntegrationTime(tsl2591IntegrationTime_t p_integration)
{
	//LOG(INFO) << "CTSL2591::setIntegrationTime";
	if (! m_tsl2591_initialized)
	{
		if (! tsl2591_initialize())
		{
			return;
		}

	}
	enable();
	m_tsl2591_integration_time = p_integration;
	m_bus->write8( TSL2591_COMMAND_BIT | TSL2591_REGISTER_CONTROL, m_tsl2591_integration_time | m_tsl2591_gain);
	disable();
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void CTSL2591::setSensibilite(tsl2591IntegrationTime_t p_integration, tsl2591Gain_t p_gain)
{
	//LOG(INFO) << "CTSL2591::setSensibilite";
	if (! m_tsl2591_initialized)
	{
		if (! tsl2591_initialize())
		{
			return;
		}

	}
	enable();
	m_tsl2591_integration_time = p_integration;
	m_tsl2591_gain = p_gain;
	m_bus->write8( TSL2591_COMMAND_BIT | TSL2591_REGISTER_CONTROL, m_tsl2591_integration_time | m_tsl2591_gain);
	disable();
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
uint32_t CTSL2591::getFullLuminosity2()
{
	LOG(INFO) << "CTSL2591::getFullLuminosity2";
	if (! m_tsl2591_initialized)
	{
		if (! tsl2591_initialize())
		{
			return 0;
		}
	}
	enable();

	for (uint8_t d = 0; d <= m_tsl2591_integration_time; d++)
	{
		usleep(120000);
	}

	uint32_t x;
	x = m_bus->read16( TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN1_LOW);
	x <<= 16;
	x |= m_bus->read16( TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN0_LOW);

	disable();

	return x;
}
//**********************************************************************
//
//
//
//----------------------------------------------------------------------
uint32_t CTSL2591::getFullLuminosity()
{
	//LOG(INFO) << "CTSL2591::getFullLuminosity";
	if (! m_tsl2591_initialized)
	{
		if (! tsl2591_initialize())
		{
			return 0;
		}
	}
	enable();

	for (uint8_t d = 0; d <= m_tsl2591_integration_time; d++)
	{
		usleep(120000);
	}

	uint32_t x1,x2,x3,x4;
	uint32_t x;
	x1 = m_bus->read8( TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN0_LOW);
	x2 = m_bus->read8( TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN0_HIGH);
	x3 = m_bus->read8( TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN1_LOW);
	x4 = m_bus->read8( TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN1_HIGH);

	//LOG(INFO) << "CTSL2591::getFullLuminosity: x1= 0x" << std::hex << (unsigned int) x1;
	//LOG(INFO) << "CTSL2591::getFullLuminosity: x2= 0x" << std::hex << (unsigned int) x2;
	//LOG(INFO) << "CTSL2591::getFullLuminosity: x3= 0x" << std::hex << (unsigned int) x3;
	//LOG(INFO) << "CTSL2591::getFullLuminosity: x4= 0x" << std::hex << (unsigned int) x4;

	
	x = x1 | (x2 << 8) | (x3 << 16) | (x4<<24);


	LOG(INFO) << "CTSL2591::getFullLuminosity: relevé= 0x" << std::hex << (unsigned int) x;

	disable();
	//LOG(INFO) << "Fin CTSL2591::getFullLuminosity";

	return x;
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
uint16_t CTSL2591::getLuminosity(uint8_t p_channel)
{
	uint32_t x = getFullLuminosity();
	if (p_channel == TSL2591_FULLSPECTRUM)
	{
		// Lit une valeur sur 2 octets du canal 0 (visible + infrarouge)
		return (x & 0xFFFF);
	}
	else if (p_channel == TSL2591_INFRARED)
	{
		// Lit une valeur sur 2 octets du canal 1 (infrarouge)
		return (x >> 16);
	}
	else if (p_channel == TSL2591_VISIBLE)
	{
		// Lit tout et calcule juste ce qui est visible
		return (( x & 0xFFFF) - (x >> 16));
	}

	// Canal inconnu
	return 0; 
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
uint32_t CTSL2591::litLux(tsl2591IntegrationTime_t p_integration, tsl2591Gain_t p_gain)
{
	//LOG(INFO) << "CTSL2591::litLux";

	setSensibilite(p_integration, p_gain);

	uint32_t x = getFullLuminosity();
	uint16_t l_canal_spectre_entier;
	uint16_t l_canal_ir;

	// Lit une valeur sur 2 octets du canal 0 (visible + infrarouge)
	l_canal_spectre_entier = (x & 0xFFFF);

	// Lit une valeur sur 2 octets du canal 1 (infrarouge)
	l_canal_ir = (x >> 16);

	return calculateLux(l_canal_spectre_entier, l_canal_ir);
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void CTSL2591::getLuminositesSeparees(uint16_t & p_canal_visible, uint16_t & p_canal_ir)
{
	uint32_t x = getFullLuminosity();
	uint16_t l_canal_spectre_entier;

	// Lit une valeur sur 2 octets du canal 0 (visible + infrarouge)
	l_canal_spectre_entier = (x & 0xFFFF);

	// Lit une valeur sur 2 octets du canal 1 (infrarouge)
	p_canal_ir = (x >> 16);

	// Lit tout et calcule juste ce qui est visible
	p_canal_visible = l_canal_spectre_entier - p_canal_ir;
}


//**********************************************************************
//
//
// ch0 - valeur lue sur le capteur sur le canal 0 (Infrarouge + Visible)
// ch1 - valeur lue sur le capteur sur le canal 1 (Infrarouge seul)
//----------------------------------------------------------------------
float CTSL2591::calculateLux(uint16_t ch0, uint16_t ch1)
{
	float atime;
	float again;
	float cpl;
	float lux1;
	float lux2;
	float lux;
	//uint32_t chan0;
	//uint32_t chan1;

	// Check for overflow conditions first
	if ((ch0 == 0xFFFF) | (ch1 == 0xFFFF))
	{
		LOG(WARNING) << "CTSL2591::calculateLux: OVERFLOW";
		return 0;
	}

	switch(m_tsl2591_integration_time)
	{
		case TSL2591_INTEGRATION_TIME_100MS:
			atime = 100.0F;
			break;
		case TSL2591_INTEGRATION_TIME_200MS:
			atime = 200.0F;
			break;
		case TSL2591_INTEGRATION_TIME_300MS:
			atime = 300.0F;
			break;
		case TSL2591_INTEGRATION_TIME_400MS:
			atime = 400.0F;
			break;
		case TSL2591_INTEGRATION_TIME_500MS:
			atime = 500.0F;
			break;
		case TSL2591_INTEGRATION_TIME_600MS:
			atime = 600.0F;
			break;
		default:
			atime = 100.0F;
			break;
	}

	switch(m_tsl2591_gain)
	{
		case TSL2591_GAIN_LOW:
			again = 1.0F;
			break;
		case TSL2591_GAIN_MED:
			again = 25.0F;
			break;
		case TSL2591_GAIN_HIGH:
			again = 428.0F;
			break;
		case TSL2591_GAIN_MAX:
			again = 9876.0F;
			break;
		default:
			again = 1.0F;
			break;
	}
	// cpl = (ATIME * AGAIN) / DF
	cpl = (atime * again) / TSL2591_LUX_DF;

	lux1 = ( (float)ch0 - (TSL2591_LUX_COEFB * (float)ch1) ) / cpl;
	lux2 = ( (TSL2591_LUX_COEFC * (float)ch0) - (TSL2591_LUX_COEFD * (float)ch1)) / cpl;
	lux = lux1 > lux2 ? lux1 : lux2;

	return lux;
}
