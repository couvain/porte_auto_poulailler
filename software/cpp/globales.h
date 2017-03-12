#ifndef __GLOBALES_H__
#define __GLOBALES_H__

#include <string>

// Seuils de luminosités pour le matin et pour le soir

const uint32_t G_SEUIL_LUMINOSITE_MATIN = 18; // TODO: mettre une valeur mesurée cohérente
const uint32_t G_SEUIL_LUMINOSITE_SOIR  = 20; // TODO: mettre une valeur mesurée cohérente

// Période de scrutation des transitions

const int G_PERIODE_ECONOMIE_ENERGIE = 10000; 	// 10 secondes en mode économie d'énergie
const int G_PERIODE_NORMALE          = 1000; 	// 1 seconde en mode normal
const int G_PERIODE_TRAVAIL_INTENSE  = 50;		// 50 ms en mode travail intense

// Durées des timeouts en secondes

const int G_DELAI_REPRISE   = 3;	// 3 secondes
const int G_DELAI_OUVERTURE = 5;	// 5 secondes TODO à mesurer
const int G_DELAI_FERMETURE = 5;	// 5 secondes TODO à mesurer

// Ports GPIO définis

const int G_GPIO_BOUTON_OUVERTURE_PORTE   = 9;
const int G_GPIO_BOUTON_FERMETURE_PORTE   = 25 ;
const int G_GPIO_BOUTON_RAZ	          = 11;

const int G_GPIO_CONTACTEUR_PORTE_OUVERTE = 8;
const int G_GPIO_CONTACTEUR_PORTE_FERMEE  = 7;

const int G_GPIO_RELAIS_INVERSEUR_A_PORTE = 4;
const int G_GPIO_RELAIS_INVERSEUR_B_PORTE = 17;
const int G_GPIO_RELAIS_COMMANDE_PORTE    = 18;

const int G_GPIO_RELAIS_ALIM_AMPLI        = 10; // Pour la voix

const int G_GPIO_RELAIS_ALERTE            = 27;

const int G_GPIO_RELAIS_INUTILISE_1       = 22;
const int G_GPIO_RELAIS_INUTILISE_2       = 23;
const int G_GPIO_RELAIS_INUTILISE_3       = 24;

const int G_GPIO_BOUTON_ARRET_URGENCE     = 5;

const int G_TIMEOUT_NON_DEFINI = 0;
const int G_TIMEOUT_NON_ECOULE = 1;
const int G_TIMEOUT_ECOULE     = 2;

int g_etat_du_timeout = G_TIMEOUT_NON_DEFINI;

int g_compteur_ouvertures   = 0;
int g_compteur_fermetures   = 0;
std::string g_message_vocal = "";

class CAutomate;
class CContacteur;
class CTSL2591;
class CBusI2C;
class CPorte;
class CRelais;
class CVoix;

CAutomate*		g_automate;
CContacteur*		g_bouton_ouverture;
CContacteur*		g_bouton_fermeture;
CContacteur*		g_bouton_raz;
CContacteur*		g_bouton_arret_urgence;
CContacteur*		g_contacteur_porte_ouverte;
CContacteur*		g_contacteur_porte_fermee;
CTSL2591*		g_capteur_luminosite;
CBusI2C *		g_bus_i2c;
CPorte*			g_porte;
CRelais*		g_relais_alerte;
CVoix*			g_voix;

CRelais*		g_relais_inutilise_1;
CRelais*		g_relais_inutilise_2;
CRelais*		g_relais_inutilise_3;

int 			g_periode_verification_transitions;


#endif
