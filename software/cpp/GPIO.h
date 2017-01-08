#ifndef __GPIO_H__
#define __GPIO_H__

#include <string>



/* Classe CGPIO
 * A propos: Chaque objet instancié de cette classe contrôlera une broche GPIO
 * Le numéro de broche GPIO doit être passé au contructeur de la classe 
 */
class CGPIO
{
private:
	int m_numero_gpio;

	// Les chemins des devices d'accès au GPIO

	std::string m_chemin_value;
	std::string m_chemin_export;
	std::string m_chemin_unexport;
	std::string m_chemin_direction;

	// Les file descriptor

	int m_valuefd;
	int m_directionfd;
	int m_exportfd;
	int m_unexportfd;

	// Les fonctions privées

	int set_direction(std::string p_direction); // "in" ou "out"
	int get_value(std::string& p_val);

public:

	CGPIO(int p_numero_gpio);
	~CGPIO();
	int export_gpio();
	int unexport_gpio();
	int set_value(bool p_valeur);
	int set_direction_output();
	int set_direction_input();
	bool get_value();
	int get_numero_gpio();

};

#endif
