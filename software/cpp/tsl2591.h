#ifndef __TSL2591_H__
#define __TSL2591_H__

#include "uinttypes.h"

#include <string>


const uint16_t G_ADRESSE_CAPTEUR = 0x29;

#define TSL2591_COMMAND_BIT (0xA0)

#define TSL2591_FULLSPECTRUM (0)	// canal 0
#define TSL2591_INFRARED     (1)	// canal 1
#define TSL2591_VISIBLE      (2)	// canal 0 - canal 1

#define TSL2591_LUX_DF    (408.0F)
#define TSL2591_LUX_COEFB (1.64F)	// coefficient ch0
#define TSL2591_LUX_COEFC (0.59F)	// coefficient A ch1
#define TSL2591_LUX_COEFD (0.86F)	// coefficient B ch2


#define TSL2591_ENABLE_POWEROFF (0x00)
#define TSL2591_ENABLE_POWERON  (0x01)
#define TSL2591_ENABLE_AEN      (0x02)
#define TSL2591_ENABLE_AIEN     (0x10)
#define TSL2591_ENABLE_NPIEN    (0x80)

enum
{
  TSL2591_REGISTER_ENABLE             = 0x00,
  TSL2591_REGISTER_CONTROL            = 0x01,
  TSL2591_REGISTER_THRESHOLD_AILTL    = 0x04, // ALS low threshold lower byte
  TSL2591_REGISTER_THRESHOLD_AILTH    = 0x05, // ALS low threshold upper byte
  TSL2591_REGISTER_THRESHOLD_AIHTL    = 0x06, // ALS high threshold lower byte
  TSL2591_REGISTER_THRESHOLD_AIHTH    = 0x07, // ALS high threshold upper byte
  TSL2591_REGISTER_THRESHOLD_NPAILTL  = 0x08, // No Persist ALS low threshold lower byte
  TSL2591_REGISTER_THRESHOLD_NPAILTH  = 0x09, // etc
  TSL2591_REGISTER_THRESHOLD_NPAIHTL  = 0x0A,
  TSL2591_REGISTER_THRESHOLD_NPAIHTH  = 0x0B,
  TSL2591_REGISTER_PERSIST_FILTER     = 0x0C,
  TSL2591_REGISTER_PACKAGE_PID        = 0x11,
  TSL2591_REGISTER_DEVICE_ID          = 0x12,
  TSL2591_REGISTER_DEVICE_STATUS      = 0x13,
  TSL2591_REGISTER_CHAN0_LOW          = 0x14,
  TSL2591_REGISTER_CHAN0_HIGH         = 0x15,
  TSL2591_REGISTER_CHAN1_LOW          = 0x16,
  TSL2591_REGISTER_CHAN1_HIGH         = 0x17
};

typedef enum
{
	TSL2591_INTEGRATION_TIME_100MS = 0x00,
	TSL2591_INTEGRATION_TIME_200MS = 0x01,
	TSL2591_INTEGRATION_TIME_300MS = 0x02,
	TSL2591_INTEGRATION_TIME_400MS = 0x03,
	TSL2591_INTEGRATION_TIME_500MS = 0x04,
	TSL2591_INTEGRATION_TIME_600MS = 0x05
}
tsl2591IntegrationTime_t;

typedef enum
{
	TSL2591_GAIN_LOW  = 0x00, // low gain (1x)
	TSL2591_GAIN_MED  = 0x10, // medium gain (25x)
	TSL2591_GAIN_HIGH = 0x20, // high gain (428x)
	TSL2591_GAIN_MAX  = 0x30  // max gain (9876x)
}
tsl2591Gain_t;

class CBusI2C;

class CTSL2591
{
private:

	CBusI2C * 					m_bus;
	bool						m_tsl2591_initialized;
	tsl2591IntegrationTime_t	m_tsl2591_integration_time;
	tsl2591Gain_t				m_tsl2591_gain;

public:
	CTSL2591();
	~CTSL2591();


	void setBusI2C(CBusI2C * p_bus);

	void setGain(tsl2591Gain_t p_gain);
	void setIntegrationTime(tsl2591IntegrationTime_t p_integration);
	void disable();
	void enable();
	bool tsl2591_initialize();
	bool detecte_adresse_tsl2591();
	float calculateLux(uint16_t ch0, uint16_t ch1);
	uint16_t getLuminosity(uint8_t p_channel);
	uint32_t getFullLuminosity();
	uint32_t getFullLuminosity2();
	void getLuminositesSeparees(uint16_t & p_canal_visible, uint16_t & p_canal_ir);
	uint32_t litLux(tsl2591IntegrationTime_t p_integration, tsl2591Gain_t p_gain);
	void setSensibilite(tsl2591IntegrationTime_t p_integration, tsl2591Gain_t p_gain);



};

#endif 
