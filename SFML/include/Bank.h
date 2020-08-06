#pragma once

#include <iostream>
#include <cstring>
#include <Property.h>
#include <Player.h>

using namespace std;

class Bank {
private:
	Property** List;
	int size = 0;
public:
	Bank();
	Bank(Property**, int);
	Bank(Bank&);
	void setPropertyList(Property**);
	Property** getPropertyList();
	
	
	void sellPropertyToPlayerByID(Property*, Player&);


	void purchasePropertyFromPlayerByID(Property*, Player);
	void payMortageToPlayerByID(int, Player);
	void declarePlayerBankruptByID(Player** player, int playerCount);

	~Bank();

	// 6th June
	Property** appendPropertyList(Property*);
	Property** removeProperty(Property*);
	Property* getProperty(int);

};