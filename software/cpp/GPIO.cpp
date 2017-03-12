#include "GPIO.h"
#include "log.h"
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

//******************************************************************************
//
//
//
//------------------------------------------------------------------------------
CGPIO::CGPIO(int p_numero_gpio)
{
	LOG(INFO) << "Création GPIO numéro " << p_numero_gpio;
	m_numero_gpio = p_numero_gpio;

	// Création des chemins d'accès aux GPIO
	m_chemin_export = "/sys/class/gpio/export";
	m_chemin_unexport = "/sys/class/gpio/unexport";

	m_chemin_value = "/sys/class/gpio/gpio";
	m_chemin_value += std::to_string(this->m_numero_gpio);
	m_chemin_value += "/value";

	m_chemin_direction ="/sys/class/gpio/gpio";
	m_chemin_direction += std::to_string(this->m_numero_gpio);
	m_chemin_direction += "/direction";

	this->export_gpio();
}

//******************************************************************************
//
//
//
//------------------------------------------------------------------------------
CGPIO::~CGPIO()
{
	this->unexport_gpio();
}

//******************************************************************************
//
//
//
//------------------------------------------------------------------------------
int CGPIO::export_gpio()
{
	int l_statut;
	std::string l_num_gpio;

	// Ouverture du device "export"
	l_statut = open(m_chemin_export.c_str(),  O_WRONLY|O_SYNC);
	if (l_statut < 0)
	{
		if (errno == ENOENT)
		{
			// ENOENT -> No such file or directory
			// Pas de "device" GPIO sur la machine
			LOG(ERROR) << "[CGPIO::export_gpio] 00 - Aucun device export GPIO \"" << m_chemin_export << "\" détecté sur cette machine";
		}
		else
		{
			LOG(ERROR) << "errno=" << errno << std::endl;
			LOG(ERROR) << "[CGPIO::export_gpio] 01 - Ouverture du device \"export\" de GPIO impossible.";

			perror("[CGPIO::export_gpio] 01 - Ouverture du device \"export\" de GPIO impossible.\n Cause");
		}
		exit(1);
	}
	m_exportfd = l_statut;

	// Ecriture du numéro de GPIO dans le device "export"
	l_num_gpio = std::to_string(m_numero_gpio);

	l_statut = write(m_exportfd, l_num_gpio.c_str(), l_num_gpio.length());
	if (l_statut < 0)
	{
		// On tente un unexport avant de réécrire le numéro de GPIO dans le device "export"
		// (évite un blocage)
		LOG(WARNING) << "[CGPIO::export_gpio] 02 - GPIO déjà exporté. Unexport préliminaire pour GPIO " << this->m_numero_gpio;
		this->unexport_gpio();
		l_statut = write(this->m_exportfd, l_num_gpio.c_str(), l_num_gpio.length());

		if (l_statut < 0)
		{
			// Cas possible: Le GPIO est probablement déjà exporté (Device or resource busy)
			LOG(ERROR) << "write() status = " << l_statut;
			LOG(ERROR) << "[CGPIO::export_gpio] 03 - Ecriture dans le device \"export\" de GPIO impossible.";
			perror("[CGPIO::export_gpio] 03 - Ecriture dans le device \"export\" de GPIO impossible.\n Cause");

			exit(1);
		}
	}

	// Fermeture du device "export"
	l_statut = close(this->m_exportfd);
	if (l_statut < 0)
	{
		LOG(ERROR) << "[CGPIO::export_gpio] 04 - Fermeture du device \"export\" de GPIO impossible. Statut = " << l_statut;
		perror("[CGPIO::export_gpio] 04 - Fermeture du device \"export\" de GPIO impossible.\n Cause");
		exit(1);
	}

    return l_statut;
}

//******************************************************************************
//
//
//
//------------------------------------------------------------------------------
int CGPIO::unexport_gpio()
{
	int l_statut;
	std::string l_num_gpio;

	l_statut = open(m_chemin_unexport.c_str(),  O_WRONLY|O_SYNC);
	if (l_statut < 0)
	{
		LOG(ERROR) << "[CGPIO::unexport_gpio] 13 - Ouverture du device \"unexport\" de GPIO impossible. Statut = " << l_statut;
		perror("[CGPIO::unexport_gpio] 13 - Ouverture du device \"unexport\" de GPIO impossible.\n Cause");
		exit(1);
	}
	m_unexportfd = l_statut;

	l_num_gpio = std::to_string(m_numero_gpio);
	l_statut = write(this->m_unexportfd, l_num_gpio.c_str(), l_num_gpio.length());
	if (l_statut < 0)
	{
		LOG(ERROR) << "[CGPIO::unexport_gpio] 14 - Ecriture dans le device \"unexport\" de GPIO impossible. Statut =" << l_statut;
		perror("[CGPIO::unexport_gpio] 14 - Ecriture dans le device \"unexport\" de GPIO impossible.\n Cause");
		exit(1);
	}

	l_statut = close(this->m_unexportfd);
	if (l_statut < 0)
	{
		LOG(ERROR) << "[CGPIO::unexport_gpio] 15 - Fermeture du device \"unexport\" de GPIO impossible. Statut =" << l_statut;
		perror("[CGPIO::unexport_gpio] 15 - Fermeture du device \"unexport\" de GPIO impossible.\n Cause");
		exit(1);
	}

	return l_statut;
}

//******************************************************************************
//
//
//
//------------------------------------------------------------------------------
int CGPIO::set_direction_output()
{
	return set_direction("out");
}

//******************************************************************************
//
//
//
//------------------------------------------------------------------------------
int CGPIO::set_direction_input()
{
	return set_direction("in");
}

//******************************************************************************
//
//
//
//------------------------------------------------------------------------------
int CGPIO::set_direction(std::string p_direction)
{
	int l_statut;

	l_statut = open(m_chemin_direction.c_str(), O_WRONLY|O_SYNC); // open direction file for gpio
	if (l_statut < 0)
	{
		LOG(ERROR) << "[CGPIO::set_direction] 06 - Ouverture du device \"direction\" de GPIO impossible. Statut= " << l_statut;
		perror("[CGPIO::set_direction] 06 - Ouverture du device \"direction\" de GPIO impossible.\n Cause");
		exit(1);
	}
	m_directionfd = l_statut;

	if ((p_direction.compare("in")  != 0) &&
		(p_direction.compare("out") != 0) )
	{
		LOG(ERROR) << "[CGPIO::set_direction] 07 - Valeur de direction invalide. Devrait être \"in\" ou \"out\"";
		fprintf(stderr, "[CGPIO::set_direction] 07 - Valeur de direction invalide. Devrait être \"in\" ou \"out\". \n");
		exit(1);
	}

	l_statut = write(m_directionfd, p_direction.c_str(), p_direction.length());
	if (l_statut < 0)
	{
		LOG(ERROR) << "[CGPIO::set_direction] 08 - Ecriture dans le device \"direction\" de GPIO impossible. Statut=" << l_statut;
		perror("[CGPIO::set_direction] 08 - Ecriture dans le device \"direction\" de GPIO impossible.\n Cause");
		exit(1);
	}

	l_statut = close(m_directionfd);
	if (l_statut < 0)
	{
		LOG(ERROR) << "[CGPIO::set_direction] 09 - Fermeture du device \"direction\" de GPIO impossible. Statut" << l_statut;
		perror("[CGPIO::set_direction] 09 - Fermeture du device \"direction\" de GPIO impossible.\n Cause");
		exit(1);
	}

	return l_statut;
}

//******************************************************************************
//
// Mettre le GPIO à l'état passé en paramètre
//
//------------------------------------------------------------------------------
int CGPIO::set_value(bool p_valeur)
{
	int l_statut;
	std::string l_val;

	// Ouverture du device "value"
	l_statut = open(m_chemin_value.c_str(), O_WRONLY|O_SYNC);
	if (l_statut < 0)
	{
		//std::cout << "setValStr = " << setValStr << std::endl;
		LOG(ERROR) << "[CGPIO::set_value] 10 - Ouverture du device \"value\" de GPIO impossible. Statut =" << l_statut;
		perror("[CGPIO::set_value] 10 - Ouverture du device \"value\" de GPIO impossible\n Cause");
		exit(1);
	}
	m_valuefd = l_statut;

	if (p_valeur)
	{
		l_val ="1";
	}
	else
	{
		l_val="0";
	}

	// Ecriture de la valeur dans le device "value"
	l_statut = write(m_valuefd, l_val.c_str(), l_val.length());
	if (l_statut < 0)
	{
		LOG(ERROR) << "[CGPIO::set_value] m_valuefd =" << m_valuefd;
		LOG(ERROR) << "[CGPIO::set_value] 11 - Ecriture dans le device \"value\" de GPIO impossible. Statut=" << l_statut;
		perror("[CGPIO::set_value] 11 - Ecriture dans le device \"value\" de GPIO impossible.\n Cause");
		exit(1);
	}

	// Fermeture du device "value"
	l_statut = close(m_valuefd);
	if (l_statut < 0)
	{
		LOG(ERROR) << "[CGPIO::set_value] 12 - Fermeture du device \"value\" de GPIO impossible. Statut =" << l_statut;
		perror("[CGPIO::set_value] 12 - Fermeture du device \"value\" de GPIO impossible.\n Cause");
        exit(1);
	}

	return l_statut;
}

//******************************************************************************
//
//
//
//------------------------------------------------------------------------------
bool CGPIO::get_value()
{
	std::string l_val;
	bool l_resultat = false;

	get_value(l_val);

	if (l_val.compare("0") == 0)
	{
		l_resultat = false;
	}
	else if (l_val.compare("1") == 0)
	{
		l_resultat = true;
	}
	else
	{
		LOG(ERROR) << "[CGPIO::get_value] 30 - Erreur de lecture dans le GPIO " << m_numero_gpio;
	}
	return l_resultat;
}

//******************************************************************************
//
//
// p_val est un paramètre en sortie
//------------------------------------------------------------------------------
int CGPIO::get_value(std::string& p_val)
{
	char l_buffer[10];
	int  l_statut;

	// Ouverture du device "value"
	l_statut = open(m_chemin_value.c_str(), O_RDONLY|O_SYNC);
	if (l_statut < 0)
	{
		LOG(ERROR) << "[CGPIO::get_value] 23 - Ouverture du device \"value\" de GPIO impossible. Statut = " << l_statut;
		perror("[CGPIO::get_value] 23 - Ouverture du device \"value\" de GPIO impossible.\n Cause");
		exit(1);
	}
	m_valuefd = l_statut;

	// Lecture du device "value"
	l_statut = read(m_valuefd, &l_buffer, 1);
	if (l_statut < 0)
	{
		LOG(ERROR) << "[CGPIO::get_value] 24 - Lecture dans le device \"value\" de GPIO impossible. Statut=" << l_statut;
		perror("[CGPIO::get_value] 24 - Lecture dans le device \"value\" de GPIO impossible.\n Cause");
		exit(1);
	}

	l_buffer[1]='\0';
	p_val = std::string(l_buffer);

	if ((p_val.compare("1") != 0) && 
		(p_val.compare("0") != 0) ) 
	{
		LOG(ERROR) << "[CGPIO::get_value] 25 - Valeur lue invalide. Devrait être \"1\" ou \"0\".";
		fprintf(stderr, "[CGPIO::get_value] 25 - Valeur lue invalide. Devrait être \"1\" ou \"0\".\n");
		exit(1);
	}

	// Fermeture du device "value"
	l_statut = close(m_valuefd);
	if (l_statut < 0)
	{
		LOG(ERROR) << "[CGPIO::get_value] 26 - Fermeture du device \"value\" de GPIO impossible. Statut=" << l_statut;
		perror("[CGPIO::get_value] 26 - Fermeture du device \"value\" de GPIO impossible.\n Cause");
		exit(1);
	}

	return l_statut;
}

//******************************************************************************
//
//
//
//------------------------------------------------------------------------------
int CGPIO::get_numero_gpio()
{
	return m_numero_gpio;
}
