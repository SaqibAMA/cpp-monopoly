#pragma once

#include <cstring>

class Card {

public:
	char* cardAction;
	int cardId;

public:

	Card() {
	
		cardAction = new char('\0');
		cardId = 0;

	}

	Card(char* cardAction, int cardId) {
	
		this->cardAction = new char[strlen(cardAction) + 1];
		strcpy_s(this->cardAction, strlen(cardAction) + 1, cardAction);

		this->cardId = cardId;

	}
	

	char* getCardAction() {
	
		return cardAction;

	}

	void setCardAction(char* cardAction) {
	
		delete[] this->cardAction;
		this->cardAction = new char[strlen(cardAction) + 1];
		strcpy_s(this->cardAction, strlen(cardAction) + 1, cardAction);

	}

	int getCardID() {
		return cardId;
	}
	
	void setCardID(int cardId) {
	
		this->cardId = cardId;

	}

	virtual int getCardType() = 0;

	~Card() {
		delete[] cardAction;
	}

};

class ChanceCard : public Card {

private:
	int cardType;

public:
	ChanceCard() : Card() {
	
		cardType = 0;

	}

	int getCardType() { return cardType; }
	void setCardType(int cardType) { this->cardType = cardType; }


};

class CommunityCard : public Card {

private:
	int cardType;

public:
	CommunityCard() : Card() {
	
		cardType = 1;

	}

	int getCardType() { return cardType; }
	void setCardType(int cardType) { this->cardType = cardType; }

};