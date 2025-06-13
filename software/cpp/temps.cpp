#include "temps.h"
#include "ptrfonction.h"
#include <signal.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <time.h>
#include <sstream>
#include <iomanip>

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void CTemps::attendre(int p_millisecondes)
{
	// NB: usleep fonctionne en microsecondes

	usleep(1000 * p_millisecondes);
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void CTemps::initialise_timeout(int p_secondes, PtrFnctVoidParamInt p_fonction)
{
	signal(SIGALRM, p_fonction);
	alarm(p_secondes); // signal dans p_secondes secondes
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void CTemps::efface_timeout()
{
	alarm(0);
}

//**********************************************************************
//
// TODO: formater l'heure au format 00:00:00
//
//----------------------------------------------------------------------
std::string CTemps::heure_complete()
{
        std::ostringstream oss;
        time_t l_heure;
        tm* l_tm;

        l_heure = time(0);
        l_tm = localtime(&l_heure);

        oss << std::setfill('0') << std::setw(2) << l_tm->tm_hour << ":"
            << std::setfill('0') << std::setw(2) << l_tm->tm_min << ":"
            << std::setfill('0') << std::setw(2) << l_tm->tm_sec;

        return oss.str();
}

//**********************************************************************
//
// heure au format "h heures m minutes"
//
//----------------------------------------------------------------------
std::string CTemps::heure_prononcable()
{
        std::ostringstream oss;
        time_t l_heure;
        tm* l_tm;

        l_heure = time(0);
        l_tm = localtime(&l_heure);

        oss << l_tm->tm_hour << " heures " << l_tm->tm_min << " minutes";

        return oss.str();
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
int CTemps::calcule_secondes(int p_heure, int p_minutes, int p_secondes)
{
	return (p_heure * 3600) + (p_minutes * 60) + p_secondes;
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
int CTemps::get_heure_en_secondes()
{
	int	l_resultat;
	time_t	l_heure;
	tm*	l_tm;

	l_heure = time(0);
	l_tm = localtime(& l_heure);

	//std::cout << "get_heure_en-secondes - " << l_tm->tm_hour << ":" << l_tm->tm_min << ":" << l_tm->tm_sec << std::endl;

	l_resultat = calcule_secondes(l_tm->tm_hour, l_tm->tm_min, l_tm->tm_sec);

	return l_resultat;
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
bool CTemps::c_est_le_matin()
{
	int l_heure_courante = get_heure_en_secondes();

	//std::cout << "heure(s)=" << l_heure_courante << " ---- MIN = " << HEURE_MATIN_MIN << " ---- MAX = " << HEURE_MATIN_MAX << std::endl;

	if ((l_heure_courante >= HEURE_MATIN_MIN) &&
		(l_heure_courante <= HEURE_MATIN_MAX))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
bool CTemps::c_est_le_soir()
{
	int l_heure_courante = get_heure_en_secondes();

	if ((l_heure_courante >= HEURE_SOIR_MIN) &&
		(l_heure_courante <= HEURE_SOIR_MAX))
	{
		return true;
	}
	else
	{
		return false;
	}
}
