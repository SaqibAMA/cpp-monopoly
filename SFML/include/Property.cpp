#include <cstring>
#include "Property.h"

using namespace std;


//CONSTRUCTORS
Property::Property() : Space() {

	propertyName = nullptr;
	propertyGroup = nullptr;
	propertyID = 0;
	ownerID = -1;
	mortgaged = false;

	purchasePrice = 0;
	rentPrice = 0;
	mortgagePrice = 0;

}
Property::Property(char* spaceType,
	int* playersOnSpace,
	int playersOnSpaceCount,
	char* propertyName,
	char* propertyGroup,
	int propertyID,
	int ownerID,
	bool mortgaged,
	int purchasePrice,
	int rentPrice,
	int mortgagePrice) : Space(spaceType, playersOnSpace, playersOnSpaceCount) {

	int size = strlen(propertyName);
	this->propertyName = new char[size + 1];
	for (int i = 0; i < size; i++) {
		this->propertyName[i] = propertyName[i];
	}
	this->propertyName[size] = '\0';

	size = strlen(propertyGroup);
	this->propertyGroup = new char[size + 1];
	for (int i = 0; i < size; i++) {
		this->propertyGroup[i] = propertyGroup[i];
	}
	this->propertyGroup[size] = '\0';

	this->propertyID = propertyID;
	this->ownerID = ownerID;
	this->mortgaged = mortgaged;

	this->purchasePrice = purchasePrice;
	this->rentPrice = rentPrice;
	this->mortgagePrice = mortgagePrice;

}
Property::Property(Property& pro) {
	char* temp = pro.getPropertyName();
	char* temp1 = pro.getPropertyGroup();

	int size = strlen(temp);
	this->propertyName = new char[size + 1];
	for (int i = 0; i < size; i++) {
		this->propertyName[i] = temp[i];
	}
	this->propertyName[size] = '\0';

	size = strlen(temp1);
	this->propertyGroup = new char[size + 1];
	for (int i = 0; i < size; i++) {
		this->propertyGroup[i] = temp1[i];
	}
	this->propertyGroup[size] = '\0';

	this->propertyID = pro.getPropertyID();
	this->ownerID = pro.getOwnerID();
	this->mortgaged = pro.getMortgaged();

	this->setSpaceType(pro.getSpaceType());
	this->setPlayersOnSpace(pro.getPlayersOnSpace(), pro.getPlayersOnSpaceCount());
	this->setPlayersOnSpaceCount(pro.getPlayersOnSpaceCount());
}
//SETTERS
void Property::setPropertyName(char* propertyName) {
	int size = strlen(propertyName);
	this->propertyName = new char[size + 1];
	for (int i = 0; i < size; i++) {
		this->propertyName[i] = propertyName[i];
	}
	this->propertyName[size] = '\0';
}
void Property::setPropertyGroup(char* propertyGroup) {
	int size = strlen(propertyGroup);
	this->propertyGroup = new char[size + 1];
	for (int i = 0; i < size; i++) {
		this->propertyGroup[i] = propertyGroup[i];
	}
	this->propertyGroup[size] = '\0';
}
void Property::setPropertyID(int propertyID) {
	this->propertyID = propertyID;
}
void Property::setOwnerID(int ownerID) {
	this->ownerID = ownerID;
}
void Property::setMortgaged(bool mortgaged) {
	this->mortgaged = mortgaged;
}
//GETTERS
char* Property::getPropertyName() {
	return propertyName;
}
char* Property::getPropertyGroup() {
	return propertyGroup;
}
int Property::getPropertyID() {
	return propertyID;
}
int Property::getOwnerID() {
	return ownerID;
}
bool Property::getMortgaged() {
	
	return mortgaged;

}


// revision: 16/05/2020
int Property::getPurchasePrice() {
	return purchasePrice;
}

int Property::getRentPrice() {

	return rentPrice;

}

int Property::getMortgagePrice() {
	return mortgagePrice;
}


//DESTRUCTOR
Property::~Property() {
	if (propertyName != nullptr) {
		delete propertyName;
		propertyName = nullptr;
	}
	if (propertyGroup != nullptr) {
		delete propertyGroup;
		propertyGroup = nullptr;
	}
	propertyID = 0;
	ownerID = 0;
}


void Property::putsPlayersOnSpace(int playerID) {



}


void Property::setPurchasePrice(int P) {
	purchasePrice = P;
}
void Property::setRentPrice(int R) {
	rentPrice = R;
}
void Property::setMortgagePrice(int M) {
	mortgagePrice = M;
}