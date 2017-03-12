#ifndef __TEMPS_H__
#define __TEMPS_H__

#include "ptrfonction.h"
#include <string>

#define HEURE_EN_SECONDES(x,y,z) x*3600+y*60+z


class CTemps
{
public:
	// Ouverture des portes entre 6h00 et 10h00 le matin
	// Régler la première valeur pour limiter l'heure de début de chant du COQ
	// Exemple: si vous voulez que le coq ne chante qu'à 8h00, réglez HEURE_MATIN_MIN sur (8,0,0)

	static const int HEURE_MATIN_MIN = HEURE_EN_SECONDES(6,0,0);
	static const int HEURE_MATIN_MAX = HEURE_EN_SECONDES(10,0,0);

	// Fermeture des portes entre 17h00 et 23h59 le soir
	static const int HEURE_SOIR_MIN = HEURE_EN_SECONDES(17,0,0);
	static const int HEURE_SOIR_MAX = HEURE_EN_SECONDES(23,59,0);

	static void attendre(int p_millisecondes);
	static void initialise_timeout(int p_secondes, PtrFnctVoidParamInt p_fonction);
	static void efface_timeout();
	static std::string heure_complete();
	static std::string heure_prononcable();

	static int calcule_secondes(int p_heure, int p_minutes, int p_secondes);
	static int get_heure_en_secondes();
	static bool c_est_le_matin();
	static bool c_est_le_soir();
};


#endif
