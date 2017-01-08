#ifndef __VOIX_H__
#define __VOIX_H__

#include <string>
#include "agentvocal.h"
#include "relais.h"


class CVoix
{
public:
	static const TAgentVocal AGENT_VOCAL_ESPEAK = 'E';
	static const TAgentVocal AGENT_VOCAL_PICO   = 'P';

private:
	CRelais * m_relais_alim_amplificateur;
	TAgentVocal m_agent_vocal;
	
public:
	CVoix(int p_gpio);
	~CVoix();
	void initialiser();
	void prononcer(std::string p_phrase);
	void set_agent_vocal(TAgentVocal p_agent_vocal);
};



#endif
