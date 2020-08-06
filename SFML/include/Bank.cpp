#include "Bank.h"

/*

Revisions before addition:

Get all the properties from a file
and encode them into the List array.

OR

Use the monopoly class to pass in the
array through parameterized constructor.


*/


//DEFUALT CONSTRUCTOR
Bank::Bank() {
	List = nullptr;
	size = 0;
}

//PARAMATRIZED CONSTRUCTOR
Bank::Bank(Property** List, int size) {
	this->size = size;
	this->List = new Property * [size];
	for (int i = 0; i < size; i++) {
		List[i] = new Property();
	}
	for (int i = 0; i < size; i++) {
		this->List[i] = List[i];
	}
}

//COPY CONSTRUCTOR
Bank::Bank(Bank& obj) {
	this->size = obj.size;
	this->List = new Property * [obj.size];
	for (int i = 0; i < obj.size; i++) {
		List[i] = new Property();
	}
	for (int i = 0; i < size; i++) {
		this->List[i] = List[i];
	}
}


//PROPERTY LIST SETTER
void Bank::setPropertyList(Property** List) {
	if (this->List != nullptr) {
		delete[] this->List;
		this->List = nullptr;
	}

	
	this->size = size;
	
	
	this->List = new Property * [size];
	for (int i = 0; i < size; i++) {
		List[i] = new Property();
	}
	for (int i = 0; i < size; i++) {
		this->List[i] = List[i];
	}
}

//PROPERTY LIST GETTER
Property** Bank::getPropertyList() {
	return this->List;
}



Property** Bank::appendPropertyList(Property* id) {
	Property** temp = new Property * [size + 1];
	for (int i = 0; i < size + 1; i++) {
		temp[i] = new Property;
	}
	for (int i = 0; i < size; i++) {
		temp[i] = List[i];
	}
	temp[size] = id;
	size++;
	return temp;
}


// 6the June


Property** Bank::removeProperty(Property* id) {
	Property** temp = new Property * [size - 1];
	for (int i = 0; i < size - 1; i++) {
		temp[i] = new Property;
	}
	int j = 0;
	for (int i = 0; i < size; i++) {
		if (List[i]->getPropertyID() != id->getPropertyID()) {
			temp[j] = List[i];
			j++;
		}
	}
	size--;
	return temp;
}
//SELL PROPERTY FUNCTION
void Bank::sellPropertyToPlayerByID(Property* Propertyid, Player& player) {
	// Faulty code
	// It sells on basis of index, not on ID
	player.addProperty(Propertyid);
	int price = Propertyid->getPurchasePrice();
	player.deductCash(price);
	List = removeProperty(Propertyid);

}



//BUY PROPERTY FUNCTION
void Bank::purchasePropertyFromPlayerByID(Property* Propertyid, Player player) {
	int id = 0;
	id = Propertyid->getPropertyID();
	if (player.searchProperty(id) == true) {
		player.addCash(player.SoldPropertyPrice(id));
		player.removeProperty(Propertyid);
		List = appendPropertyList(Propertyid);
	}
	else {
		cout << "PLAYER DOEST OWN THE GIVEN PROPERTY : " << endl;
	}
}

//MORTAGE FUNCTION
void Bank::payMortageToPlayerByID(int Propertyid, Player player) {
	int mortage = 0;
	mortage = int(List[Propertyid][0].getPurchasePrice() / 2.0);
	player.addCash(mortage);
	List[Propertyid][0].setOwnerID(-1);
}


//CHECK BANKRUPT FUNCTION
void Bank::declarePlayerBankruptByID(Player** player, int playerCount) {
	for (int i = 0; i < playerCount; i++) {
		if (player[i]->getCash() <= 0 && player[i]->getPropertyListSize() == 0) {
			player[i]->setIsBankrupt(true);
		}
	}
}

Property* Bank::getProperty(int id) {
	for (int i = 0; i < size; i++) {
		if (List[i]->getPropertyID() == id) {
			return List[i];
		}
	}

	return nullptr;
}


Bank::~Bank() {

	if (size) {
		free(List);
	}

}