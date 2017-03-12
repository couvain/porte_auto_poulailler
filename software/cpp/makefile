CFLAGS=-c -std=c++11 --all-warnings --extra-warnings

poulailler: automate.o busi2c.o contacteur.o GPIO.o moteur.o porte.o poulailler.o relais.o temps.o tsl2591.o voix.o
	g++ -o poulailler automate.o busi2c.o contacteur.o GPIO.o moteur.o porte.o poulailler.o relais.o temps.o tsl2591.o voix.o

automate.o: automate.cpp
	g++ $(CFLAGS) automate.cpp
	
busi2c.o: busi2c.cpp
	g++ $(CFLAGS) busi2c.cpp

contacteur.o: contacteur.cpp
	g++ $(CFLAGS) contacteur.cpp

GPIO.o: GPIO.cpp
	g++ $(CFLAGS) GPIO.cpp

moteur.o: moteur.cpp
	g++ $(CFLAGS) moteur.cpp

porte.o: porte.cpp
	g++ $(CFLAGS) porte.cpp

poulailler.o: poulailler.cpp globales.h
	g++ $(CFLAGS) poulailler.cpp

relais.o: relais.cpp
	g++ $(CFLAGS) relais.cpp

temps.o: temps.cpp
	g++ $(CFLAGS) temps.cpp
	
tsl2591.o: tsl2591.cpp
	g++ $(CFLAGS) tsl2591.cpp

voix.o: voix.cpp
	g++ $(CFLAGS) voix.cpp
