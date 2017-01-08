#ifndef __BUSI2C_H__
#define __BUSI2C_H__

#include "uinttypes.h"

#include <string>

const std::string G_NOM_DEVICE_I2C = "/dev/i2c-1";

typedef enum
{
	BUS_I2C_ETAT_INCONNU  = 0x00, 
	BUS_I2C_ETAT_FERME    = 0x01, 
	BUS_I2C_ETAT_OUVERT   = 0x02 
}
etatBusI2C_t;


class CBusI2C
{
private:
	
	int				m_i2c_device_file_handle;
	etatBusI2C_t	m_i2c_etat_bus;

public:
	CBusI2C();
	~CBusI2C();
	void 			ouvre_bus();
	void 			ferme_bus();
	etatBusI2C_t 	get_etat_bus();
	bool 			is_bus_ouvert();
	void 			write8(uint8_t p_registre, uint8_t p_valeur);
	uint16_t 		read16(uint8_t p_registre);
	uint8_t 		read8(uint8_t p_registre);
	bool 			detecte_adresse(uint16_t p_adresse_esclave);

	
};

#endif
