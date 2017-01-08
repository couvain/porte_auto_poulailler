#include "busi2c.h"
#include "log.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>


//**********************************************************************
//
//
//
//----------------------------------------------------------------------
CBusI2C::CBusI2C()
{
	m_i2c_device_file_handle 	= 0;
	m_i2c_etat_bus				= BUS_I2C_ETAT_INCONNU;
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
CBusI2C::~CBusI2C()
{
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void CBusI2C::ouvre_bus()
{
	//LOG(INFO) << "CBusI2C::ouvre_bus";
	if (is_bus_ouvert())
	{
		LOG(INFO) << "CBusI2C::ouvre_bus - Bus I²C déjà ouvert";
		return;
	}

	// Ouvrir le bus I²C
	std::string l_nom_fichier = G_NOM_DEVICE_I2C;
	//LOG(INFO) << "CBusI2C::ouvre_bus - Tentative d'ouverture du bus I²C";

	m_i2c_device_file_handle = open(l_nom_fichier.c_str(), O_RDWR);

	if (m_i2c_device_file_handle < 0)
	{
		m_i2c_etat_bus = BUS_I2C_ETAT_FERME;

		if (errno == ENOENT)
		{
			// ENOENT -> No such file or directory
			// Pas de "device" I²C sur la machine 
			LOG(ERROR) << "CBusI2C::ouvre_bus - Aucun bus I²C \"" << l_nom_fichier << "\" détecté sur cette machine";
		}
		else
		{
			LOG(ERROR) << "CBusI2C::ouvre_bus - errno = " << errno;
			LOG(ERROR) << "CBusI2C::ouvre_bus - Ouverture du bus I²C impossible";
			// Erreur - Vérification errno
			perror("CBusI2C::ouvre_bus - Ouverture du bus I²C impossible");
		}
	}
	else
	{
		//LOG(INFO) << "CBusI2C::ouvre_bus - Ouverture du bus I²C réussie";
		m_i2c_etat_bus = BUS_I2C_ETAT_OUVERT;
	}
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void CBusI2C::ferme_bus()
{
	//LOG(INFO) << "CBusI2C::ferme_bus";
	if (m_i2c_etat_bus == BUS_I2C_ETAT_OUVERT)
	{
		// Fermeture du bus I²C
		//LOG(INFO) << "CBusI2C::ferme_bus - Fermeture du bus I²C";

		close(m_i2c_device_file_handle);
		m_i2c_etat_bus = BUS_I2C_ETAT_FERME;
	}
	else if (m_i2c_etat_bus == BUS_I2C_ETAT_INCONNU)
	{
		LOG(INFO) << "CBusI2C::ferme_bus - Fermeture du bus I²C (depuis état inconnu)";
		m_i2c_etat_bus = BUS_I2C_ETAT_FERME;
	}
	else if (m_i2c_etat_bus == BUS_I2C_ETAT_FERME)
	{
		LOG(INFO) << "CBusI2C::ferme_bus - Fermeture du bus I²C déjà faite";
	}
}

//**********************************************************************
//
// 
//
//----------------------------------------------------------------------
etatBusI2C_t CBusI2C::get_etat_bus()
{
	return m_i2c_etat_bus;
}

//**********************************************************************
//
// 
//
//----------------------------------------------------------------------
bool CBusI2C::is_bus_ouvert()
{
	return ( m_i2c_etat_bus == BUS_I2C_ETAT_OUVERT );
}

//**********************************************************************
//
//
//
// Testée OK
//----------------------------------------------------------------------
uint8_t CBusI2C::read8(uint8_t p_registre)
{
	uint8_t l_buffer = 0;

	//------------------------------------------------------
	// Sélection du registre à lire
	//------------------------------------------------------
	if ( write(m_i2c_device_file_handle, &p_registre, 1) != 1 )
	{
		LOG(ERROR) << "CBusI2C.read8: Erreur d'écriture dans le registre d'adresse 0x" << std::hex << std::uppercase << (unsigned int) p_registre;
		exit(1);
	}
	//----------------------------------------------------
	// Lecture du registre
	//----------------------------------------------------
	if ( read(m_i2c_device_file_handle, &l_buffer, 1) != 1)
	{
		LOG(ERROR) << "CBusI2C.read8: Erreur de lecture I²C dans le registre d'adresse 0x" << std::hex << std::uppercase << (unsigned int) p_registre;
	}
	//else
	//{
	//	LOG(INFO) << "CBusI2C.read8: Lecture I²C réussie registre(0x" << std::hex << std::uppercase << (unsigned int) p_registre << ") = 0x" << std::hex << std::uppercase << (unsigned int) l_buffer;
	//}

	return l_buffer;
}

//**********************************************************************
//
//
//
// TODO: trouver le bon type de retour sur 2 octets
// TODO: faire le bon calcul de résultat
//----------------------------------------------------------------------
uint16_t CBusI2C::read16(uint8_t p_registre)
{
	uint16_t l_buffer  = 0;
	uint8_t  l_buffer1 = 0;
	uint8_t  l_buffer2 = 0;

	//------------------------------------------------------
	// Sélection du registre à lire
	//------------------------------------------------------
	if ( write(m_i2c_device_file_handle, &p_registre, 1) != 1 )
	{
		LOG(ERROR) << "CBusI2C.read16: Erreur d'écriture dans le registre d'adresse 0x" << std::hex << std::uppercase << (unsigned int) p_registre;
		exit(1);
	}
	//----------------------------------------------------
	// Lecture du registre
	//----------------------------------------------------
	// TODO: tester une double lecture d'octet si ça ne fonctionne toujours pas
	//if ( read(m_i2c_device_file_handle, &l_buffer, 2) != 2)
	if ( read(m_i2c_device_file_handle, &l_buffer1, 1) != 1)
	{
		LOG(ERROR) << "CBusI2C.read16: Erreur de lecture 1 I²C";
	}
	else
	{
		LOG(INFO) << "CBusI2C.read16: Lecture 1: 0x" << (unsigned int) l_buffer1;
		// Lecture 1 réussie
		if ( read(m_i2c_device_file_handle, &l_buffer2, 1) != 1)
		{
			LOG(ERROR) << "CBusI2C.read16: Erreur de lecture 2 I²C";
		}
		else
		{
			LOG(INFO) << "CBusI2C.read16: Lecture 2: 0x" << (unsigned int) l_buffer2;
			// Lecture 2 réussie
			l_buffer = (l_buffer1 << 8) | l_buffer2;
			LOG(INFO) << "CBusI2C.read16: Lecture I²C réussie registre(0x" << std::hex << std::uppercase << (unsigned int) p_registre << ") = 0x" << std::hex << (unsigned int) l_buffer;
		}
	}

	return l_buffer;
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void CBusI2C::write8(uint8_t p_registre, uint8_t p_valeur)
{
	//------------------------------------------------------
	// Sélection du registre à écrire
	//------------------------------------------------------
	if ( write(m_i2c_device_file_handle, &p_registre, 1) != 1 )
	{
		LOG(ERROR) << "CBusI2C.write8: Erreur d'écriture dans le registre d'adresse " << std::hex << std::uppercase << (unsigned int) p_registre;
		exit(1);
	}
	
	//------------------------------------------------------
	// Ecriture du registre
	//------------------------------------------------------
	if ( write(m_i2c_device_file_handle, &p_valeur, 1) != 1 )
	{
		LOG(ERROR) << "CBusI2C.write8: Erreur d'écriture de la valeur 0x" << std::hex << std::uppercase << (unsigned int) p_valeur << " dans le registre d'adresse ox" << std::hex << std::uppercase << (unsigned int) p_registre;
	}
	//else
	//{
	//	LOG(INFO) << "CBusI2C.write8: Ecriture réussie de la valeur 0x" << std::hex << std::uppercase << (unsigned int) p_valeur << " dans le registre d'adresse ox" << std::hex << std::uppercase << (unsigned int) p_registre;
	//}
}

//**********************************************************************
//
// 
//
//----------------------------------------------------------------------
bool CBusI2C::detecte_adresse(uint16_t p_adresse_esclave)
{
	// Détection de l'adresse I²C du capteur sur le bus
	if ( ioctl(m_i2c_device_file_handle, I2C_SLAVE, p_adresse_esclave) < 0 )
	{
		LOG(ERROR) << "Erreur de détection de l'adresse esclave 0x" << std::hex << p_adresse_esclave << " sur le bus I²C";
		return false;
	}
	//else
	//{
	//	LOG(INFO) << "CBusI2C.Adresse esclave 0x" << std::hex << p_adresse_esclave << " présente sur le bus I²C";
	//}
	return true;
}
