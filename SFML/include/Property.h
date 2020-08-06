#pragma once

#include "Space.h"
#include <cstring>

using namespace std;

class Property : public Space
{

	//DATA MEMBERS
	char* propertyName;
	char* propertyGroup;
	int propertyID;
	int ownerID;
	bool mortgaged;

	int purchasePrice;
	int rentPrice;
	int mortgagePrice;

public:
	//CONSTRUCTORS
	Property();
	Property(char* , int*, int,char*, char*, int, int, bool,
		int, int, int);
	Property(Property&);
	//SETTERS
	void setPropertyName(char*);
	void setPropertyGroup(char*);
	void setPropertyID(int);
	void setOwnerID(int);
	void setMortgaged(bool);
    //GETTERS
	char* getPropertyName();
	char* getPropertyGroup();
	int getPropertyID();
	int getOwnerID();
	bool getMortgaged();


	virtual int getPurchasePrice();
	virtual int getRentPrice();
	int getMortgagePrice();

	void setPurchasePrice(int);
	void setRentPrice(int);
	void setMortgagePrice(int);
	virtual int upgradedPrice() { return 0; };

	virtual void addHouse() {};
	virtual	void addHotel() {};
	virtual void addShop() {};
	virtual void addWifi() {};
	virtual void addGas() {};
	virtual void addElectricity() {};

	virtual int getHouseCount() { return 0; }
	virtual int getHotelCount() { return 0; }
	virtual int getShopCount() { return 0; }
	virtual bool getHasWifi() { return 0; }
	virtual bool getHasGas() { return 0; }
	virtual bool getHasElectricity() { return 0; }

	void putsPlayersOnSpace(int playerID);

	//DESTRUCTOR
	~Property();
};
