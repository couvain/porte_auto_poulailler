#include <iostream>
#include <iomanip>
#include <string>

#include "automate.h"
#include "contacteur.h"
#include "porte.h"
#include "busi2c.h"
#include "tsl2591.h"
#include "temps.h"
#include "log.h"
#include "voix.h"
#include "globales.h"

// TODO: Ajouter le timeout aux transitions
// TODO: revoir précisément chaque callback
// TODO: Supprimer les logs au fur et à mesure
// TODO: faire la classe CCapteurNumerique avec I2C
// TODO: tester systématiquement les allocations de mémoire
// TODO: faire le script de sauvegarde
// TODO: utiliser une gestion d'exceptions
// TODO: faire un schéma électronique de l'automate
// TODO: Reconcevoir le mouvement de la porte dans l'automate
// TODO: mettre des messages d'erreur plus clairs / visibles pour l'utilisateur

void initialiser_hardware();
void liberer_hardware();

void boucle_principale();
int lit_numero_transition();
void construction_automate();
void construction_objets();
void destruction_objets();

void callback_etat_0();
void callback_etat_1();
void callback_etat_2();
void callback_etat_3();
void callback_etat_4();
void callback_etat_5();
void callback_etat_6();
void callback_etat_7();
void callback_etat_8();
void callback_etat_9();
void callback_etat_10();
void callback_etat_11();
void callback_etat_12();
void callback_etat_13();
void callback_etat_14();
void callback_etat_15();
void callback_etat_16();
void callback_etat_17();

bool teste_transition(int p_num_transition);

bool teste_transition_0();
bool teste_transition_1();
bool teste_transition_2();
bool teste_transition_3();
bool teste_transition_4();
bool teste_transition_5();
bool teste_transition_6();
bool teste_transition_7();
bool teste_transition_8();
bool teste_transition_9();
bool teste_transition_10();
bool teste_transition_11();
bool teste_transition_12();
bool teste_transition_13();
bool teste_transition_14();
bool teste_transition_15();
bool teste_transition_16();
bool teste_transition_17();
bool teste_transition_18();
bool teste_transition_19();
bool teste_transition_20();
bool teste_transition_21();
bool teste_transition_22();
bool teste_transition_23();
bool teste_transition_24();
bool teste_transition_25();
bool teste_transition_26();
bool teste_transition_27();

void affiche_transitions();

void demarrer_timeout(int p_duree_secondes);
bool is_timeout_ecoule();
void callback_timeout(int p_param);
void stop_timeout();

//**********************************************************************
//
// Programme principal
//
//----------------------------------------------------------------------
int main ()
{
	std::cout << std::endl;

	FILE* pFile = fopen("poulailler.log", "a");
	COutputToFile::Stream() = pFile;
	CFileLog::ReportingLevel() = CFileLog::FromString("[DEBUG]");

	LOG(INFO) << "--------------------------------------";
	LOG(INFO) << "Porte automatique de poulailler v1.0";
	LOG(INFO) << "--------------------------------------";
	std::cout << "--------------------------------------\n";
	std::cout << "Porte automatique de poulailler v1.0\n";
	std::cout << "--------------------------------------" << std::endl;
	
	construction_objets();
	g_voix->set_agent_vocal(CVoix::AGENT_VOCAL_PICO);
	g_voix->joue_wav("chicken.wav");
	g_voix->prononcer("Démarrage du système");
	
	g_periode_verification_transitions = G_PERIODE_NORMALE;
	
	initialiser_hardware();
	construction_automate();
	g_automate->affiche();
	
	// Tout se passe là
	boucle_principale();
	
	liberer_hardware();
	
	destruction_objets();
	
	return 0;
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void initialiser_hardware()
{
	// Relier le capteur I2C au bus I2C
	g_capteur_luminosite->setBusI2C(g_bus_i2c);
	
	// Ouvrir le bus I2C
	g_bus_i2c->ouvre_bus();
	
	if (!g_bus_i2c->is_bus_ouvert())
	{
			// Arrêt prématuré du programme
			exit(1);
	}
	else
	{
		if (g_bus_i2c->detecte_adresse(G_ADRESSE_CAPTEUR))
		{
			// Test de fonctionnement du capteur TSL2591
			g_capteur_luminosite->tsl2591_initialize();
		}
	}
	
	// GPIO 4  - relais inverseur A du moteur de la porte
	// GPIO 17 - relais inverseur B du moteur de la porte
	// GPIO 18 - relais de commande du moteur de la porte
	g_porte->initialiser();

	// GPIO 27 - relais de commande du flash d'alerte
	g_relais_alerte->initialiser();

	// GPIO 10 - relais de l'alimentation électrique de l'amplificateur audio
	g_voix->initialiser();

	// GPIO 22 - relais inutilisé
	g_relais_inutilise_1->initialiser();
	
	// GPIO 23 - relais inutilisé
	g_relais_inutilise_2->initialiser();
	
	// GPIO 24 - relais inutilisé
	g_relais_inutilise_3->initialiser();

}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void liberer_hardware()
{
	g_bus_i2c->ferme_bus();
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void construction_automate()
{
	// Construction de l'automate

	// Etape 1 - les états

	g_automate->ajoute_etat("Réinitialisation"			, callback_etat_0);
	g_automate->ajoute_etat("Porte fermante"			, callback_etat_1);
	g_automate->ajoute_etat("Porte fermée à temps"		, callback_etat_2);
	g_automate->ajoute_etat("Porte fermée"				, callback_etat_3);
	g_automate->ajoute_etat("Porte ouvrante"			, callback_etat_4);	
	g_automate->ajoute_etat("Porte ouverte à temps"		, callback_etat_5);
	g_automate->ajoute_etat("Porte ouverte"				, callback_etat_6);
	
	g_automate->ajoute_etat("Porte pas fermée à temps"	, callback_etat_7);
	g_automate->ajoute_etat("Porte fermante bloquée"	, callback_etat_8);
	g_automate->ajoute_etat("Nouvelle fermeture"		, callback_etat_9);
	g_automate->ajoute_etat("Porte totalement bloquée"	, callback_etat_10);
	g_automate->ajoute_etat("Blocage en fermeture"		, callback_etat_11);
	
	g_automate->ajoute_etat("Porte pas ouverte à temps"	, callback_etat_12);	
	g_automate->ajoute_etat("Porte ouvrante bloquée"	, callback_etat_13);
	g_automate->ajoute_etat("Nouvelle ouverture"		, callback_etat_14);
	g_automate->ajoute_etat("Blocage en fermeture"		, callback_etat_15);

	g_automate->ajoute_etat("Alerte"					, callback_etat_16);
	
	// Pour le cas où la porte est descendue par gravité, par erreur
	g_automate->ajoute_etat("Porte descendue par erreur", callback_etat_17);
	
	// Etat pour réglage de l'heure (et de la date?)



	// Etape 2 - les transitions

	g_automate->ajoute_trans(0,1);	// transition 0

	g_automate->ajoute_trans(1,2);	// transition 1
	g_automate->ajoute_trans(2,3);	// transition 2
	g_automate->ajoute_trans(3,4);	// transition 3
	g_automate->ajoute_trans(4,5);	// transition 4
	g_automate->ajoute_trans(5,6);	// transition 5
	g_automate->ajoute_trans(6,1);	// transition 6

	g_automate->ajoute_trans(1,7);	// Transition 7
	g_automate->ajoute_trans(9,7);	// Transition 8
	g_automate->ajoute_trans(8,9);	// Transition 9
	g_automate->ajoute_trans(9,2);	// Transition 10
	g_automate->ajoute_trans(9,11);	// Transition 11
	g_automate->ajoute_trans(8,10);	// Transition 12
	g_automate->ajoute_trans(10,16);	// Transition 13
	g_automate->ajoute_trans(11,16);	// Transition 14
	g_automate->ajoute_trans(4,12);		// Transition 15
	g_automate->ajoute_trans(14,12);	// Transition 16
	g_automate->ajoute_trans(7,8);		// Transition 17
	g_automate->ajoute_trans(12,13);	// Transition 18
	g_automate->ajoute_trans(14,15);	// Transition 19
	g_automate->ajoute_trans(14,5);		// Transition 20
	g_automate->ajoute_trans(13,14);	// Transition 21
	g_automate->ajoute_trans(13,10);	// Transition 22
	g_automate->ajoute_trans(15,15);	// Transition 23
	g_automate->ajoute_trans(16,0);		// Transition 24
	g_automate->ajoute_trans(16,16);	// Transition 25

	g_automate->ajoute_trans(6,17);		// Transition 26
	g_automate->ajoute_trans(17,4);		// Transition 27
	
	// Pas d'état final (à revoir?)
	//g_automate->set_etat_final(3);
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void construction_objets()
{
	g_automate = new CAutomate();

	g_bouton_ouverture 	= new CContacteur(G_GPIO_BOUTON_OUVERTURE_PORTE);
	g_bouton_fermeture 	= new CContacteur(G_GPIO_BOUTON_FERMETURE_PORTE);
	g_bouton_raz 		= new CContacteur(G_GPIO_BOUTON_RAZ);

	g_contacteur_porte_ouverte 	= new CContacteur(G_GPIO_CONTACTEUR_PORTE_OUVERTE);
	g_contacteur_porte_fermee 	= new CContacteur(G_GPIO_CONTACTEUR_PORTE_FERMEE);

	g_porte = new CPorte(G_GPIO_RELAIS_INVERSEUR_A_PORTE,
						 G_GPIO_RELAIS_INVERSEUR_B_PORTE,
						 G_GPIO_RELAIS_COMMANDE_PORTE);

	g_capteur_luminosite = new CTSL2591();
	g_bus_i2c = new CBusI2C();
	
	g_voix = new CVoix(G_GPIO_RELAIS_ALIM_AMPLI);

	g_relais_alerte      = new CRelais(G_GPIO_RELAIS_ALERTE);

	// On déclare les relais inutilisés pour éteindre leur LED et économiser l'énergie
	g_relais_inutilise_1 = new CRelais(G_GPIO_RELAIS_INUTILISE_1);
	g_relais_inutilise_2 = new CRelais(G_GPIO_RELAIS_INUTILISE_2);
	g_relais_inutilise_3 = new CRelais(G_GPIO_RELAIS_INUTILISE_3);

}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void destruction_objets()
{
	delete(g_automate);

	delete(g_bouton_ouverture);
	delete(g_bouton_fermeture);
	delete(g_bouton_raz);

	delete(g_contacteur_porte_ouverte);
	delete(g_contacteur_porte_fermee);

	delete(g_porte);

	delete(g_capteur_luminosite);
	delete(g_bus_i2c);
	delete(g_relais_alerte);

	delete(g_voix);

	delete(g_relais_inutilise_1);
	delete(g_relais_inutilise_2);
	delete(g_relais_inutilise_3);
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void boucle_principale()
{
	bool l_fin = false;
	int l_transition = CAutomate::TRANSITION_INDEFINIE;

	g_automate->demarre();

	while(! l_fin)
	{
		l_transition = lit_numero_transition();

		g_automate->execute_trans(l_transition);
		g_automate->affiche();
		affiche_transitions();

		CTemps::attendre(g_periode_verification_transitions); // pause de durée fluctuante

		// A-t-on_terminé l'automate ?
		l_fin = g_automate->termine();
	}
}

//**********************************************************************
//
// Fonction qui prend les entrées et calcule les transitions correspondantes
// parmi les transitions de l'état courant passe en paramètre
//
//----------------------------------------------------------------------
int lit_numero_transition()
{
	int i;
	int l_transition = CAutomate::TRANSITION_INDEFINIE;
	int l_nombre_transitions_definies;
	
	l_nombre_transitions_definies = g_automate->get_nb_transitions_definies_pour_etat_actuel();
	
	// On ne teste que les transitions partant de l'état actuel
	for (i = 1 ; i <= l_nombre_transitions_definies ; i++)
	{
		l_transition = g_automate->get_numero_transition_definie(i);
		if (teste_transition(l_transition) == true)
		{
			// On a trouvé une transition activée
			// Transition trouvée
			LOG(INFO) << "Transition trouvée: " << l_transition;
			return l_transition;
		}
	}
	return CAutomate::TRANSITION_INDEFINIE;
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void affiche_transitions()
{
	int i;

	LOG(INFO) << "Valeurs des Transitions" << std::endl;
	LOG(INFO) << "Transition";
	for ( i = 0;i < CAutomate::NBMAX_TRANS; i++ )
	{
		LOG(INFO) << std::setw(3) << i;
	}

	LOG(INFO) << "Valeur    ";
	for ( i = 0; i < CAutomate::NBMAX_TRANS; i++ )
	{
		LOG(INFO) << std::setw(3) << teste_transition(i);
	}
}

//**********************************************************************
// const int G_TIMEOUT_NON_DEFINI = 0;
// const int G_TIMEOUT_NON_ECOULE = 1;
// const int G_TIMEOUT_ECOULE     = 2;
// int g_etat_du_timeout = G_TIMEOUT_NON_DEFINI;
//----------------------------------------------------------------------
void demarrer_timeout(int p_duree_secondes)
{
	LOG(INFO) << "Timeout défini sur " << p_duree_secondes << " secondes";
	CTemps::initialise_timeout(p_duree_secondes, callback_timeout);
	g_etat_du_timeout = G_TIMEOUT_NON_ECOULE;
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void callback_timeout(int p_param)
{
	LOG(INFO) << "Timeout arrivé à échéance";
	g_etat_du_timeout = G_TIMEOUT_ECOULE;
	CTemps::efface_timeout();
}

//**********************************************************************
// const int G_TIMEOUT_NON_DEFINI = 0;
// const int G_TIMEOUT_NON_ECOULE = 1;
// const int G_TIMEOUT_ECOULE     = 2;
// int g_etat_du_timeout = G_TIMEOUT_NON_DEFINI;
//----------------------------------------------------------------------
bool is_timeout_ecoule()
{
	if (g_etat_du_timeout == G_TIMEOUT_ECOULE)
	{
		return true;
	}
	return false;
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void stop_timeout()
{
	if (g_etat_du_timeout == G_TIMEOUT_NON_ECOULE)
	{
		LOG(INFO) << "Timeout stoppé";
		CTemps::efface_timeout();
		g_etat_du_timeout = G_TIMEOUT_NON_DEFINI;
	}
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
bool teste_transition(int p_num_transition)
{
	switch (p_num_transition)
	{
		case  0:	return teste_transition_0();
		case  1:	return teste_transition_1();
		case  2:	return teste_transition_2();
		case  3:	return teste_transition_3();
		case  4:	return teste_transition_4();
		case  5:	return teste_transition_5();
		case  6:	return teste_transition_6();
		case  7:	return teste_transition_7();
		case  8:	return teste_transition_8();
		case  9:	return teste_transition_9();
		case 10:	return teste_transition_10();
		case 11:	return teste_transition_11();
		case 12:	return teste_transition_12();
		case 13:	return teste_transition_13();
		case 14:	return teste_transition_14();
		case 15:	return teste_transition_15();
		case 16:	return teste_transition_16();
		case 17:	return teste_transition_17();
		case 18:	return teste_transition_18();
		case 19:	return teste_transition_19();
		case 20:	return teste_transition_20();
		case 21:	return teste_transition_21();
		case 22:	return teste_transition_22();
		case 23:	return teste_transition_23();
		case 24:	return teste_transition_24();
		case 25:	return teste_transition_25();
		case 26:	return teste_transition_26();
		case 27:	return teste_transition_27();
		default:	return false;
	}
	return false;
}

//**********************************************************************
//
// Transition entre 0 et 1
// "1"
//
//----------------------------------------------------------------------
bool teste_transition_0() // OK
{
	return true;
}

//**********************************************************************
//
// Transition entre 1 et 2
// "Contact fermé OK ET pas timeout"
//
//----------------------------------------------------------------------
bool teste_transition_1() 
{
	return (g_contacteur_porte_fermee->get_etat()) &&
			!(is_timeout_ecoule());
}

//**********************************************************************
//
// Transition entre 2 et 3
// "1"
//
//----------------------------------------------------------------------
bool teste_transition_2() // OK
{
	return true;
}

//**********************************************************************
//
// Transition entre 3 et 4
// Bouton ouverture OU (Heure = MATIN et Luminosité>SEUIL_LUMINOSITE_MATIN )
//
//----------------------------------------------------------------------
bool teste_transition_3() // OK
{
	return g_bouton_ouverture->get_etat() ||
		( CTemps::c_est_le_matin() &&
		g_capteur_luminosite->litLux(TSL2591_INTEGRATION_TIME_500MS,TSL2591_GAIN_LOW) > G_SEUIL_LUMINOSITE_MATIN );
}

//**********************************************************************
//
// Transition entre 4 et 5
// Contact ouvert OK et  pas timeout
//
//----------------------------------------------------------------------
bool teste_transition_4()
{
	return g_contacteur_porte_ouverte->get_etat() && 
	!(is_timeout_ecoule());
}

//**********************************************************************
//
// Transition entre  5 et 6
// "1"
//
//----------------------------------------------------------------------
bool teste_transition_5() // OK
{
	return true;
}

//**********************************************************************
//
// Transition entre 6 et 1
// Bouton fermeture ou (heure = soir et luminosite < seuil_luminosite_soir)
//
//----------------------------------------------------------------------
bool teste_transition_6() // OK
{
	return g_bouton_fermeture->get_etat() ||
			(CTemps::c_est_le_soir() &&
			g_capteur_luminosite->litLux(TSL2591_INTEGRATION_TIME_500MS,TSL2591_GAIN_LOW) < G_SEUIL_LUMINOSITE_SOIR);
}

//**********************************************************************
//
// Transition entre 1 et 7
// timeout et contact fermé ko
//
//----------------------------------------------------------------------
bool teste_transition_7() 
{
	bool l_resultat;
	
	l_resultat = is_timeout_ecoule() && 
				!(g_contacteur_porte_fermee->get_etat());
	
	return l_resultat;
}

//**********************************************************************
//
// Transition entre 9 et 7
// timeout ET contact ferme ko ET compteur_fermeture < 3
//
//----------------------------------------------------------------------
bool teste_transition_8() 
{
	return is_timeout_ecoule() && 
			!(g_contacteur_porte_fermee->get_etat()) && 
			(g_compteur_fermetures < 3);
}

//**********************************************************************
//
// Transition entre 8 et 9
// contact ouvert ET pas timeout
//
//----------------------------------------------------------------------
bool teste_transition_9() 
{
	return g_contacteur_porte_ouverte->get_etat() && 
			!(is_timeout_ecoule());
}

//**********************************************************************
//
// Transition entre 9 et 2
// contact fermé ok ET pas timeout
//
//----------------------------------------------------------------------
bool teste_transition_10() 
{
	return g_contacteur_porte_fermee->get_etat() && 
			!(is_timeout_ecoule());
}

//**********************************************************************
//
// Transition entre 9 et 11
// timeout ET contact ferme ko ET compteur fermetures = 3
//
//----------------------------------------------------------------------
bool teste_transition_11() 
{
	bool l_resultat;
	
	l_resultat = is_timeout_ecoule() && 
				!(g_contacteur_porte_fermee->get_etat()) &&
				 (g_compteur_fermetures == 3);
				
	return l_resultat;
}

//**********************************************************************
//
// Transition entre 8 et 10
// timeout ET contact ouvert KO
//
//----------------------------------------------------------------------
bool teste_transition_12() 
{
	return is_timeout_ecoule() && 
			!(g_contacteur_porte_ouverte->get_etat());
}

//**********************************************************************
//
// Transition entre 10 et 16
// "1"
//
//----------------------------------------------------------------------
bool teste_transition_13() 
{
	return true;
}

//**********************************************************************
//
// Transition entre 11 et 16
// "1"
//
//----------------------------------------------------------------------
bool teste_transition_14() 
{
	return true;
}

//**********************************************************************
//
// Transition entre 4 et 12
// timeout et contact ouvert KO
//
//----------------------------------------------------------------------
bool teste_transition_15() 
{
	return is_timeout_ecoule() &&
			!(g_contacteur_porte_ouverte->get_etat());
}

//**********************************************************************
//
// Transition entre 14 et 12
// timeout ET contact ouvert ko ET compteur ouverture < 3
//
//----------------------------------------------------------------------
bool teste_transition_16() 
{
	return is_timeout_ecoule() && 
			!(g_contacteur_porte_ouverte->get_etat()) &&
			(g_compteur_ouvertures < 3);
}

//**********************************************************************
//
// Transition entre 7 et 8
// "1"
//
//----------------------------------------------------------------------
bool teste_transition_17() 
{
	return true;
}

//**********************************************************************
//
// Transition entre 12 et 13
// "1"
//
//----------------------------------------------------------------------
bool teste_transition_18() 
{
	return true;
}

//**********************************************************************
//
// Transition entre 14 et 15
// timeout ET contact ouvert ko ET compteur ouverture =3
//
//----------------------------------------------------------------------
bool teste_transition_19() 
{
	bool l_resultat;
	
	l_resultat = is_timeout_ecoule() && 
				!(g_contacteur_porte_ouverte->get_etat()) &&
				 (g_compteur_ouvertures == 3);
			
	return l_resultat;
}

//**********************************************************************
//
// Transition entre 14 et 5	
// contact ouvert ok ET pas timeout
//
//----------------------------------------------------------------------
bool teste_transition_20() 
{
	return g_contacteur_porte_ouverte->get_etat() && 
			!(is_timeout_ecoule());
}

//**********************************************************************
//
// Transition entre 13 et 14	
// contact fermé ok ET pas timeout
//
//----------------------------------------------------------------------
bool teste_transition_21() 
{
	return g_contacteur_porte_fermee->get_etat() && 
			!(is_timeout_ecoule());
}

//**********************************************************************
//
// Transition entre 13 et 10	
// timeout ET contact fermé KO 
//
//----------------------------------------------------------------------
bool teste_transition_22() 
{
	return is_timeout_ecoule() && 
			!(g_contacteur_porte_fermee->get_etat());
}

//**********************************************************************
//
// Transition entre 15 et 16	
// "1" 
//
//----------------------------------------------------------------------
bool teste_transition_23() 
{
	return true;
}

//**********************************************************************
//
// Transition entre 16 et 0	
// bouton raz
//
//----------------------------------------------------------------------
bool teste_transition_24() 
{
	return g_bouton_raz->get_etat();
}

//**********************************************************************
//
// Transition entre 16 et 16
// bouton ouverture (permet d'entendre de nouveau le message vocal)
//
//----------------------------------------------------------------------
bool teste_transition_25() 
{
	return g_bouton_ouverture->get_etat();
}

//**********************************************************************
//
// Transition entre 6 et 17
// contact ouvert KO
//
//----------------------------------------------------------------------
bool teste_transition_26() 
{
	return !(g_contacteur_porte_ouverte->get_etat()); 
}

//**********************************************************************
//
// Transition entre 17 (porte descendue) et 4 (porte ouvrante)
// =1
//
//----------------------------------------------------------------------
bool teste_transition_27() 
{
	return true;
}

//**********************************************************************
//**********************************************************************
//********** Les callbacks appelés par les états de l'automate *********
//**********************************************************************
//**********************************************************************

//**********************************************************************
//
// Etat 0 -> Réinitialisation
//
//----------------------------------------------------------------------
void callback_etat_0()
{
	LOG(INFO) << "Appel du callback_etat_0 - Réinitialisation";
	g_periode_verification_transitions = G_PERIODE_NORMALE;
	g_voix->prononcer("Démarrage de l'automate");
	

	// Eteindre le flash d'alerte
	g_relais_alerte->desactiver();

	// Compteurs à zéro
	g_compteur_ouvertures   = 0;
	g_compteur_fermetures   = 0;
	g_message_vocal = "";

	// Couper le moteur de la porte
	g_porte->stopper();
	
	// On stoppe un éventuel timeout en cours
	stop_timeout();
}

//**********************************************************************
//
// Etat 1 -> Porte fermante
//
//----------------------------------------------------------------------
void callback_etat_1() // OK
{
	LOG(INFO) << "Appel du callback_etat_1 - Porte fermante";
	g_periode_verification_transitions = G_PERIODE_TRAVAIL_INTENSE;

	demarrer_timeout(G_DELAI_FERMETURE);

	// Fermer la porte
	g_porte->fermer();

	g_compteur_fermetures =1;
}

//**********************************************************************
//
// Etat 2 -> Porte fermée à temps
//
//----------------------------------------------------------------------
void callback_etat_2() // OK
{
	LOG(INFO) << "Appel du callback_etat_2 - Porte fermée à temps";

	stop_timeout();
	g_porte->stopper();	
}

//**********************************************************************
//
// Etat 3 -> Porte fermée
//
//----------------------------------------------------------------------
void callback_etat_3() // OK
{
	LOG(INFO) << "Appel du callback_etat_3 - Porte fermée";

	// Economiser l'énergie
	g_periode_verification_transitions = G_PERIODE_ECONOMIE_ENERGIE;
	g_voix->prononcer("Porte fermée");
}

//**********************************************************************
//
// Etat 4 -> Porte ouvrante
//
//----------------------------------------------------------------------
void callback_etat_4() // OK
{
	LOG(INFO) << "Appel du callback_etat_4 - Porte ouvrante";
	g_periode_verification_transitions = G_PERIODE_TRAVAIL_INTENSE;
	
	demarrer_timeout(G_DELAI_OUVERTURE);
	
	// Ouvrir la porte
	g_porte->ouvrir();

	g_compteur_ouvertures = 1;
}

//**********************************************************************
//
// Etat 5 -> Porte ouverte à temps
//
//----------------------------------------------------------------------
void callback_etat_5() // OK
{
	LOG(INFO) << "Appel du callback_etat_5 - Porte ouverte à temps";

	stop_timeout();
	g_porte->stopper();
}

//**********************************************************************
//
// Etat 6 -> Porte ouverte
//
//----------------------------------------------------------------------
void callback_etat_6() // OK
{
	LOG(INFO) << "Appel du callback_etat_6 - Porte ouverte";

	// Economiser l'énergie
	g_periode_verification_transitions = G_PERIODE_ECONOMIE_ENERGIE;
	g_voix->prononcer("Porte ouverte");
}

//**********************************************************************
//
// Etat 7 -> Porte pas fermée à temps
//
//----------------------------------------------------------------------
void callback_etat_7()
{
	LOG(INFO) << "Appel du callback_etat_7 - Porte pas fermée à temps";
	g_periode_verification_transitions = G_PERIODE_NORMALE;

	stop_timeout();
	g_porte->stopper();
}

//**********************************************************************
//
// Etat 8 -> Porte fermante bloquée
//
//----------------------------------------------------------------------
void callback_etat_8()
{
	LOG(INFO) << "Appel du callback_etat_8 - Porte fermante bloquée";
	g_periode_verification_transitions = G_PERIODE_TRAVAIL_INTENSE;

	demarrer_timeout(G_DELAI_OUVERTURE);
	
	// Ouvrir la porte
	g_porte->ouvrir();
}

//**********************************************************************
//
// Etat 9 -> Nouvelle fermeture
//
//----------------------------------------------------------------------
void callback_etat_9() // OK
{
	LOG(INFO) << "Appel du callback_etat_9 - Nouvelle fermeture";

	// Attendre DELAI REPRISE
	CTemps::attendre(G_DELAI_REPRISE);

	demarrer_timeout(G_DELAI_FERMETURE);

	// Fermer la porte
	g_porte->fermer();
	
	g_compteur_fermetures++;
}

//**********************************************************************
//
// Etat 10-> Porte totalement bloquée
//
//----------------------------------------------------------------------
void callback_etat_10()
{
	LOG(INFO) << "Appel du callback_etat_10 - Porte totalement bloquée";

	g_message_vocal="Porte complètement bloquée";
	stop_timeout();
	g_porte->stopper();
}

//**********************************************************************
//
// Etat 11 -> Blocage en fermeture
//
//----------------------------------------------------------------------
void callback_etat_11()
{
	LOG(INFO) << "Appel du callback_etat_11 - Blocage en fermeture";

	g_message_vocal="Blocage en fermeture";
	
	stop_timeout();
	g_porte->stopper();
}

//**********************************************************************
//
// Etat 12 -> Porte pas ouverte à temps
//
//----------------------------------------------------------------------
void callback_etat_12()
{
	LOG(INFO) << "Appel du callback_etat_12 - Porte pas ouverte à temps";
	g_periode_verification_transitions = G_PERIODE_NORMALE;

	stop_timeout();
	g_porte->stopper();
}

//**********************************************************************
//
// Etat 13 -> Porte ouvrante bloquée
//
//----------------------------------------------------------------------
void callback_etat_13()
{
	LOG(INFO) << "Appel du callback_etat_13 - Porte ouvrante bloquée";
	g_periode_verification_transitions = G_PERIODE_TRAVAIL_INTENSE;

	demarrer_timeout(G_DELAI_FERMETURE);
	
	// Fermer la porte
	g_porte->fermer();
}

//**********************************************************************
//
// Etat 14 -> Nouvelle ouverture
//
//----------------------------------------------------------------------
void callback_etat_14() // OK
{
	LOG(INFO) << "Appel du callback_etat_14 - Nouvelle ouverture";

	// Attendre DELAI REPRISE
	CTemps::attendre(G_DELAI_REPRISE);

	demarrer_timeout(G_DELAI_OUVERTURE);

	// Ouvrir la porte
	g_porte->ouvrir();
	
	g_compteur_ouvertures++;
}

//**********************************************************************
//
// Etat 15 -> Blocage en ouverture
//
//----------------------------------------------------------------------
void callback_etat_15()
{
	LOG(INFO) << "Appel du callback_etat_15 - Blocage en ouverture";

	g_message_vocal="Blocage en ouverture";
	stop_timeout();
	g_porte->stopper();
}

//**********************************************************************
//
// Etat 16 -> ALERTE
//
//----------------------------------------------------------------------
void callback_etat_16()
{
	LOG(INFO) << "Appel du callback_etat_16 - ALERTE";
	g_periode_verification_transitions = G_PERIODE_NORMALE;

	// Allumer le flash d'alerte
	g_relais_alerte->activer();

	// Envoyer le message d'erreur par le haut parleur
	g_voix->prononcer("Alerte");
	g_voix->prononcer(g_message_vocal);
}

//**********************************************************************
//
// Etat 17 -> Porte descendue par erreur
//
//----------------------------------------------------------------------
void callback_etat_17()
{
	LOG(INFO) << "Appel du callback_etat_16 - Porte descendue par erreur";
}
