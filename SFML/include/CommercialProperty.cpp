#include "CommercialProperty.h"
#include <cstring>
using namespace std;
//CONSTRUCTORS
CommercialProperty::CommercialProperty() :Property()
{
}
CommercialProperty::CommercialProperty(char* spaceType, int* playersOnSpace, int playersOnSpaceCount, char* propertyName, char* propertyGroup, int propertyID, int ownnerID, bool mortgaged, int purchasePrice, int rentPrice, int mortgagedPrice) : Property(spaceType, playersOnSpace, playersOnSpaceCount, propertyName, propertyGroup, propertyID, ownnerID, mortgaged,purchasePrice,rentPrice,mortgagedPrice)
{
}

CommercialProperty::CommercialProperty(CommercialProperty& CommPro) {
	this->setSpaceType(CommPro.getSpaceType());
	this->setPlayersOnSpace(CommPro.getPlayersOnSpace(), CommPro.getPlayersOnSpaceCount());
	this->setPlayersOnSpaceCount(CommPro.getPlayersOnSpaceCount());
	this->setPropertyName(CommPro.getPropertyName());
	this->setPropertyGroup(CommPro.getPropertyGroup());
	this->setPropertyID(CommPro.getPropertyID());
	this->setOwnerID(CommPro.getOwnerID());
	this->setMortgaged(CommPro.getMortgaged());
	this->setPurchasePrice(CommPro.getPurchasePrice());
	this->setRentPrice(CommPro.getRentPrice());
	this->setMortgagePrice(CommPro.getMortgagePrice());
}

int CommercialProperty::getPurchasePrice() {

	return Property::getPurchasePrice();

}

int CommercialProperty::getRentPrice() {

	return Property::getRentPrice();

}

CommercialProperty::~CommercialProperty() {



}