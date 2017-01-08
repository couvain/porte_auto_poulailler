#include "voix.h"
#include "log.h"

#include <iostream>
#include <string>
#include <stdlib.h>

//*****************************************************************************
//
// 
//
//-----------------------------------------------------------------------------
CVoix::CVoix(int p_gpio)
{
	m_relais_alim_amplificateur = new CRelais(p_gpio);
}

//*****************************************************************************
//
// 
//
//-----------------------------------------------------------------------------
CVoix::~CVoix()
{
	delete(m_relais_alim_amplificateur);
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
void CVoix::initialiser()
{
	m_relais_alim_amplificateur->initialiser();
}

//*****************************************************************************
//
// 
//
//-----------------------------------------------------------------------------
void CVoix::set_agent_vocal(TAgentVocal p_agent_vocal)
{
	m_agent_vocal = p_agent_vocal;
}

//*****************************************************************************
//
// 
//
//-----------------------------------------------------------------------------
void CVoix::prononcer(std::string p_phrase)
{
	std::string l_commande;
	
	m_relais_alim_amplificateur->activer();
	if (m_agent_vocal == CVoix::AGENT_VOCAL_ESPEAK)
	{ 
		l_commande = "espeak -v mb/mb-fr4 \"";
		l_commande += p_phrase;
		l_commande+="\" 2>/dev/null";
	
		LOG(INFO) << "Prononcé (espeak): \"" << p_phrase << "\"";
		system(l_commande.c_str());
	}
	else if (m_agent_vocal == CVoix::AGENT_VOCAL_PICO)
	{
		// 1° - On crée un fichier WAV dans le dossier /tmp
		l_commande = "pico2wave -l fr-FR -w /tmp/phrase.wav \"";
		l_commande += p_phrase;
		l_commande+="\" 2>/dev/null";
		//LOG(INFO) << l_commande;
		system(l_commande.c_str());
	
		LOG(INFO) << "Prononcé (pico): \"" << p_phrase << "\"";

		// 2° - On lit le fichier WAV créé et on le supprime		
		
		l_commande = "play -q \"/tmp/phrase.wav\"; rm -f /tmp/phrase.wav";
		//LOG(INFO) << l_commande;
		system(l_commande.c_str());
	}
	m_relais_alim_amplificateur->desactiver();
}

//*****************************************************************************
//
// Joue un fichier WAV avec la commande play
//
//-----------------------------------------------------------------------------
void CVoix::joue_wav(std::string p_nom_fichier)
{
	std::string l_commande;
	m_relais_alim_amplificateur->activer();
	
	// On lit le fichier WAV 
	
	l_commande = "play -q \"";
	l_commande += p_nom_fichier;
	l_commande += "\"";
	//LOG(INFO) << l_commande;
	system(l_commande.c_str());
	
	m_relais_alim_amplificateur->desactiver();
}

