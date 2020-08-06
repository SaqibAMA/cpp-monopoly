#pragma once
#include <cstring>
#include "Property.h"

#include <iostream>

using namespace std;

class PrivateProperty :public Property
{
	//DATA MEMBERS
	int houseCount;
	int hotelCount;
	int shopCount;
	bool hasWifi;
	bool hasGas;
	bool hasElectricity;
public:
	//CONSTRUCTORS
	PrivateProperty();
	PrivateProperty(char*, int*, int, char*, char*, int, int, bool, int, int, int, int, int, int, bool, bool, bool);
	PrivateProperty(PrivateProperty&);
	//SETTERS
	void setHouseCount(int);
	void setHotelCount(int);
	void setShopCount(int);
	void setHasWifi(bool);
	void setHasGas(bool);
	void setHasElectricity(bool);
	//GETTERS
	int getHouseCount();
	int getHotelCount();
	int getShopCount();
	bool getHasWifi();
	bool getHasGas();
	bool getHasElectricity();
	//SPECIAL FUNCTIONS
	bool isUpgraded();
	void addHouse();
	void addHotel();
	void addShop();
	void addWifi();
	void addGas();
	void addElectricity();
	int calculateRent();

	int getPurchasePrice();
	int getRentPrice();



	void putsPlayersOnSpace(int& playerCash, int dealChoice);


	//DESTRUCTOR
	~PrivateProperty();
};


