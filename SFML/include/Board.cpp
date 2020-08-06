#define _CRT_SECURE_NO_WARNINGS

#include "Board.h"

int privatePropertySpaces[20] = {

		1, 3, 6, 8,
		9, 11, 13, 14,
		16, 18, 19, 21,
		23, 24, 26, 27,
		29, 31, 32, 34,

};

int commercialPropertySpaces[8] = {

		5, 12, 15, 25,
		28, 35, 37, 39

};

int specialSpaces[12] = {
		0, 2, 4, 7,
		10, 17, 20, 22,
		30, 33, 36, 38
};


struct Map {

public:
	char* key;
	int value;
	int* list;

	Map() {
		key = nullptr;
		value = 0;
		list = nullptr;
	}

	Map(const char* key, int value, int a, int b, int c = 0, int d = 0) {
	
		this->value = value;

		this->key = new char[strlen(key) + 1];
		strcpy(this->key, key);

		list = new int[value];


		if (c == 0) {
			list[0] = a;
			list[1] = b;
		}
		else if (d == 0){

			list[0] = a;
			list[1] = b;
			list[2] = c;

		}
		else {

			list[0] = a;
			list[1] = b;
			list[2] = c;
			list[3] = d;

		}

	}

};


Board::Board() {

	players = nullptr;

	prevTurns = nullptr;
	playerCount = 0;

	turn = 0;
	dRollCount = 0;

	srand(time(0));

	previousTurn = 0;

	currRollAmount = 0;

	jailPosition = 10;



	// Initializing Board Cells

	cells = new Space * [40];

	ifstream fin("files/cellDetails.txt");

	// Special Spaces

	for (int i = 0; i < 12; i++) {
	
		cells[specialSpaces[i]] = new Space;

		char temp[100];

		fin.getline(temp, 100);


		cells[specialSpaces[i]]->setSpaceType(temp);

	
	}
	

	for (int i = 0; i < 20; i++) {
	

		cells[privatePropertySpaces[i]] = new PrivateProperty;

		PrivateProperty* p = (PrivateProperty *) cells[privatePropertySpaces[i]];

		char temp[100];

		fin.getline(temp, 100);

		p->setPropertyName(temp);

		fin >> temp;

		p->setPropertyGroup(temp);

		p->setSpaceType("PRIVATE");

		int tempNum;

		fin >> tempNum;

		p->setPropertyID(tempNum);

		fin >> tempNum;

		p->setOwnerID(tempNum);

		fin >> tempNum;

		p->setMortgaged((bool) tempNum);

		fin >> tempNum;

		p->setPurchasePrice(tempNum);

		fin >> tempNum;

		p->setRentPrice(tempNum);

		fin.ignore();
	
	
	}


	for (int i = 0; i < 8; i++) {

		cells[commercialPropertySpaces[i]] = new CommercialProperty;

		CommercialProperty* c = (CommercialProperty* ) cells[commercialPropertySpaces[i]];

		char temp[100];

		fin.getline(temp, 100);

		c->setPropertyName(temp);

		fin >> temp;

		c->setPropertyGroup(temp);

		c->setSpaceType("COMMERCIAL");

		c->setPropertyID(i + 20);

		int tempNum;

		fin >> tempNum;

		c->setPurchasePrice(tempNum);

		fin >> tempNum;

		c->setRentPrice(tempNum);

		fin.ignore();


	}


	fin.close();



	// 6th June

	// Review

	ifstream chanceCardsFile;
	ifstream communityChestFile;
	chanceCardsFile.open("files/Chance.txt");
	communityChestFile.open("files/CommunityChest.txt");

	Chance = new Card * [15];
	CommunityChest = new Card * [15];
	for (int i = 0; i < 15; i++) {
		Chance[i] = new ChanceCard;
		CommunityChest[i] = new CommunityCard;
	}
	for (int i = 0; i < 15; i++) {
		char CCtemp[200];
		char CCCtemp[200];
		chanceCardsFile.getline(CCtemp, 200);
		communityChestFile.getline(CCCtemp, 200);
		Chance[i]->setCardAction(CCtemp);
		CommunityChest[i]->setCardAction(CCCtemp);
		Chance[i]->setCardID(i);
		CommunityChest[i]->setCardID(i);
	}

	chanceCardsFile.close();
	communityChestFile.close();



}

int Board::getPlayerCount() {

	return playerCount;

}

void Board::setPlayerCount(int playerCount) {

	this->playerCount = playerCount;

}

void Board::allocatePrevTurns() {

	if (prevTurns) free(prevTurns);

	prevTurns = new int*[playerCount];

	for (int i = 0; i < playerCount; i++) {
		prevTurns[i] = new int[2];

		for (int j = 0; j < 2; j++)
			prevTurns[i][j] = 0;

	}

}

int** Board::getPrevTurns() {
	return prevTurns;
}

int* Board::rollDice() {

	int* diceNum = new int[2];

	diceNum[0] = rand() % 6 + 1;
	diceNum[1] = rand() % 6 + 1;

	//diceNum[0] = 1;
	//diceNum[1] = 1;

	//cin >> diceNum[0] >> diceNum[1];



	while (diceNum[0] == prevTurns[playerCount - 3][0] && diceNum[1] == prevTurns[playerCount - 3][1]) {

		diceNum[0] = rand() % 6 + 1;
		diceNum[1] = rand() % 6 + 1;

	}



	for (int i = 0; i < playerCount - 1; i++) {
	
		prevTurns[i][0] = prevTurns[i + 1][0];
		prevTurns[i][1] = prevTurns[i + 1][1];

	}

	prevTurns[playerCount - 1][0] = diceNum[0];
	prevTurns[playerCount - 1][1] = diceNum[1];


	if (diceNum[0] != diceNum[1]) {

		previousTurn = turn;

		if (dRollCount) {
			dRollCount = 0;
		}

		turn = (turn + 1) % playerCount;

	}
	else {

		dRollCount++;

		if (dRollCount == 3) {

			diceNum[0] = -1;
			diceNum[1] = -1;

			//cout << "dCount hit 3!" << endl;

			/*turn++;
			turn = turn % playerCount;*/

			currRollAmount = 0;

		}

	}


	return diceNum;


}


int Board::getDRollCount() { return dRollCount; }

void Board::setDRollCount(int dRollCount) {
	this->dRollCount = dRollCount;
}


int Board::getTurn() {

	return turn;

}

void Board::setTurn(int turn) {

	this->turn = turn;

}

int Board::getPreviousTurn() {
	return previousTurn; 
}

void Board::setPreviousTurn(int previousTurn) {
	this->previousTurn = previousTurn; 
}

int Board::getCurrRollAmount() { return currRollAmount; }

void Board::setCurrRollAmount(int currRollAmount) {
	this->currRollAmount = currRollAmount;
}

int Board::getJailPosition() {
	return jailPosition;
}

Player** Board::getPlayers() {

	return players;

}

void Board::allocatePlayers() {

	if (players) delete[] players;

	this->players = new Player * [playerCount];

	const char* names[5] = {

		"Saqib",
		"Nabeel",
		"Salman",
		"Abdur Rehman",
		"Chooran"

	};

	for (int i = 0; i < playerCount; i++)
		this->players[i] = new Player(names[i], i);

}

Space** Board::getCells() {
	return cells;
}


Player* Board::getPlayerByID(int id) {

	for (int i = 0; i < playerCount; i++) {


		if (players[i]->getPlayerID() == id) {
		
			return players[i];
		
		}
	
	
	}

	return nullptr;

}



void Board::upgradeProperty(int index, int i, int j) {

	// Property Map
	Map** propertyMap = new Map * [7];

	propertyMap[0] = new Map("IQBAL", 3, 1, 3, 6);
	propertyMap[1] = new Map("JOHAR", 2, 8, 9);
	propertyMap[2] = new Map("FAISAL", 3, 11, 13, 14);
	propertyMap[3] = new Map("MODEL", 3, 16, 18, 19);
	propertyMap[4] = new Map("GULBERG", 3, 21, 23, 24);
	propertyMap[5] = new Map("DHA", 3, 26, 27, 29);
	propertyMap[6] = new Map("BAHRIA", 3, 31, 32, 34);



	//Player* owner = getPlayerByID(3);
	PrivateProperty* p = (PrivateProperty*)getCells()[index];
	
	if (p->getOwnerID() != -1 && !p->getMortgaged()) {

		Player* owner = getPlayerByID(p->getOwnerID());



		bool canUpgrade = true;
		int groupInd = 0;

		char* pGroup = p->getPropertyGroup();

		for (int i = 0; i < 7; i++) {

			if (strcmp(pGroup, propertyMap[i]->key) == 0) groupInd = i;

		}

		
		int totalHouses = 0;
		int minHouses = INT_MAX;

		for (int i = 0; i < propertyMap[groupInd]->value && canUpgrade; i++) {

			PrivateProperty* p = (PrivateProperty*)cells[propertyMap[groupInd]->list[i]];
			if (p->getOwnerID() != owner->getPlayerID()) {

				canUpgrade = false;
				totalHouses += p->getHouseCount();

			}

		}



		for (int i = 0; i < propertyMap[groupInd]->value; i++) {

			PrivateProperty* p = (PrivateProperty*)cells[propertyMap[groupInd]->list[i]];

			if (p->getHouseCount() < minHouses) {
				minHouses = p->getHouseCount();
			}

		}




		if (canUpgrade) {

			if (i == 0 && j == 0) {

				if (owner->getCash() >= 100) {

					//if (minHouses == INT_MAX) {
					//	
					//	p->addHouse();
					//	owner->deductCash(100);
					//
					//}
					//else {

						if (p->getHouseCount() == minHouses) {

							p->addHouse();
							owner->deductCash(100);

						}
						else {

							// Error message

							sf::RenderWindow prompt(sf::VideoMode(280, 90), "NOT ENOUGH CASH!", sf::Style::Titlebar);

							sf::Font stdFont;
							stdFont.loadFromFile("fonts/Montserrat-Black.ttf");

							sf::Text nec;
							nec.setString("CANNOT BUILD\nHouses must be\nequally spread out.");
							nec.setPosition(5.0f, 5.0f);
							nec.setFont(stdFont);
							nec.setCharacterSize(15);
							nec.setFillColor(sf::Color::Black);

							sf::Texture closeBtnTexture;
							closeBtnTexture.loadFromFile("assets/upgrade_prompt_close.png");
							sf::RectangleShape closeBtn(sf::Vector2f(15.0f, 15.0f));
							closeBtn.setTexture(&closeBtnTexture);
							closeBtn.setPosition(250.0f, 5.0f);

							while (prompt.isOpen()) {

								sf::Event evt;
								while (prompt.pollEvent(evt)) {

									if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {


										sf::Vector2f mousePos = prompt.mapPixelToCoords(sf::Mouse::getPosition(prompt));
										sf::FloatRect closeBtnBounds = closeBtn.getGlobalBounds();

										if (closeBtnBounds.contains(mousePos)) {

											prompt.close();

										}


									}

								}

								prompt.clear(sf::Color::White);
								prompt.draw(nec);
								prompt.draw(closeBtn);
								prompt.display();

							}

						}

					//}

				}


			}
			else if (i == 0 && j == 1) {

				if (owner->getCash() >= 30 && !p->getHasWifi()) {
					p->addWifi();
					owner->deductCash(30);
				}

			}
			else if (i == 1 && j == 0) {

				if (owner->getCash() >= 300 || p->getHouseCount() >= 3) {

					p->addShop();

					(owner->getCash() >= 300) ?
						owner->deductCash(300) :
						p->setHouseCount(p->getHouseCount() - 3);

				}

			}
			else if (i == 1 && j == 1) {

				if (owner->getCash() >= 50 && !p->getHasElectricity()) {
				
					p->addElectricity();
					owner->deductCash(50);

				}

			}
			else if (i == 2 && j == 0) {

				if (p->getHouseCount() >= 4 && p->getShopCount() >= 2) {
				
					p->setHouseCount(p->getHouseCount() - 4);
					p->setShopCount(p->getShopCount() - 2);
					p->addHotel();

				}

			}
			else if (i == 2 && j == 1) {

				if (owner->getCash() >= 50 && !p->getHasGas()) {

					p->addGas();
					owner->deductCash(50);

				}

			}

		}


	}


	for (int i = 0; i < 7; i++)
		delete[] propertyMap[i];

	delete[] propertyMap;


}


void Board::putPlayerOnSpace(int index, int playerID, sf::RenderWindow& window, int dealChoice) {


	Player* p = getPlayerByID(playerID);

	cout << "dealChoice from Board --> " << dealChoice << endl;

	int propertiesOwnedByPlayer = 0;

	for (int i = 0; i < 40; i++) {
	
		if (strcmp(cells[i]->getSpaceType(), "PRIVATE") == 0 || strcmp(cells[i]->getSpaceType(), "COMMERCIAL") == 0) {
		

			Property* property = (Property *) cells[i];

			if (property->getOwnerID() == p->getPlayerID()) {
			
				propertiesOwnedByPlayer++;

			}

		

		}

	}


	if (strcmp(cells[index]->getSpaceType(), "PRIVATE") == 0 && dealChoice != -1) {
	

		cout << p->getCash() << endl;

		PrivateProperty* k = (PrivateProperty *) cells[index];
		
		
		if ( (p->getCash() >= k->getPurchasePrice() && dealChoice == 0) || (p->getCash() >= k->getRentPrice() && dealChoice == 1)) {
		

			int deltaPrice = p->getCash();

			k->putsPlayersOnSpace(p->getCashRef(), dealChoice);

			deltaPrice = deltaPrice - p->getCash();
			
			if (k->getOwnerID() != -1) {

				getPlayerByID(k->getOwnerID())->addCash(deltaPrice);

			}

			if (dealChoice == 0) {
				
				k->setOwnerID(p->getPlayerID());

			}

		
		
		}
		else if (propertiesOwnedByPlayer) {

			int netWorth = 0;

			for (int i = 0; i < 40; i++) {
			

				if (strcmp(cells[i]->getSpaceType(), "PRIVATE") == 0 || strcmp(cells[i]->getSpaceType(), "COMMERCIAL") == 0) {


					Property* property = (Property*)cells[i];

					if (property->getOwnerID() == p->getPlayerID()) {

						netWorth += property->getPurchasePrice();

					}



				}
				

			}

			cout << "Net Worth Is -> " << netWorth << endl;


			int minimumWorthPropertyIndex = -1;

			for (int i = 0; i < p->getPropertyListSize(); i++) {
			
				if (p->getPropertyList()[i]->getPurchasePrice() >= k->getPurchasePrice()) {
				
					minimumWorthPropertyIndex = i;

				}

			}

			if (minimumWorthPropertyIndex == -1) {

				p->setIsBankrupt(true); 
			
			}
			else {
				
				int deltaPrice = p->getCash();

				if (netWorth >= k->getPurchasePrice() && dealChoice == 0) {

					k->putsPlayersOnSpace(p->getCashRef(), dealChoice);


				}
				else if (netWorth >= k->getRentPrice() && dealChoice == 1) {


					k->putsPlayersOnSpace(p->getCashRef(), dealChoice);

				}

				deltaPrice = deltaPrice - p->getCash();

				if (dealChoice == 0) {

					k->setOwnerID(p->getPlayerID());

				}

				if (k->getOwnerID() != -1)
					getPlayerByID(k->getOwnerID())->addCash(deltaPrice);

			}

		}
		else {
		
			p->setIsBankrupt(true);

		}




	}


}


Card** Board::getChance() {
	return Chance;
}

Card** Board::getCommunityChest() {
	return CommunityChest;
}



// 6th June
// Review

void Board::executeCard(Card& C, sf::RenderWindow& window) {
	int type = 0;
	type = C.getCardType();
	if (type == 0) {
		int id = C.getCardID();
		if (id == 0) {
			players[turn]->setPlayerPosition(0);
			players[turn]->setCash(players[turn][0].getCash() + 300);
			cells[0]->putsPlayersOnSpace(turn);
		}
		if (id == 1) {
			if (players[turn]->getPlayerPosition() > 26) {
				players[turn]->setCash(500);
			}
			players[turn]->setPlayerPosition(26);
			cells[26]->putsPlayersOnSpace(turn);
		}
		if (id == 2) {
			int pos = 0;
			bool check = false;
			int index = 0;
			pos = players[turn]->getPlayerPosition();
			if (pos >= 0 && pos < 12) {
				players[turn]->setPlayerPosition(12);
				cells[12]->putsPlayersOnSpace(turn);
				for (int i = 0; i < playerCount && check == false; i++) {
					PrivateProperty** temp = (PrivateProperty**)players[i]->getPropertyList();
					for (int i = 0; i < players[i]->getPropertyListSize() && check == false; i++) {
						if (temp[i]->getPropertyID() == 12) {
							check = true;
							index = i;
						}
					}
				}
				if (check == true) {
					players[index]->setCash(players[index]->getCash() + (5 * currRollAmount));
					players[turn]->setCash(players[turn]->getCash() - (5 * currRollAmount));
				}
				else {
					//BUY PROPERTY FUNTION HAS TO BE CALLED HERE
				}
			}
			if (pos > 12 && pos < 28) {
				players[turn]->setPlayerPosition(28);
				cells[28]->putsPlayersOnSpace(turn);
				for (int i = 0; i < playerCount && check == false; i++) {
					PrivateProperty** temp = (PrivateProperty**)players[i]->getPropertyList();
					for (int i = 0; i < players[i]->getPropertyListSize() && check == false; i++) {
						if (temp[i]->getPropertyID() == 28) {
							check = true;
							index = i;
						}
					}
				}
				if (check == true) {
					players[index]->setCash(players[index]->getCash() + (5 * currRollAmount));
					players[turn]->setCash(players[turn]->getCash() - (5 * currRollAmount));
				}
				else {
					//BUY PROPERTY FUNTION HAS TO BE CALLED HERE
				}
			}
			if (pos > 28 && pos < 37) {
				players[turn]->setPlayerPosition(37);
				cells[37]->putsPlayersOnSpace(turn);
				for (int i = 0; i < playerCount && check == false; i++) {
					PrivateProperty** temp = (PrivateProperty**)players[i]->getPropertyList();
					for (int i = 0; i < players[i]->getPropertyListSize() && check == false; i++) {
						if (temp[i]->getPropertyID() == 37) {
							check = true;
							index = i;
						}
					}
				}
				if (check == true) {
					players[index]->setCash(players[index]->getCash() + (5 * currRollAmount));
					players[turn]->setCash(players[turn]->getCash() - (5 * currRollAmount));
				}
				else {
					//BUY PROPERTY FUNTION HAS TO BE CALLED HERE
				}
			}
		}
		if (id == 3) {
			int pos = 0;
			int index = 0;
			bool check = false;
			int rent = 0;
			pos = players[turn]->getPlayerPosition();
			if (pos >= 0 && pos < 5) {
				players[turn]->setPlayerPosition(5);
				cells[5]->putsPlayersOnSpace(turn);
				for (int i = 0; i < playerCount && check == false; i++) {
					PrivateProperty** temp = (PrivateProperty**)players[i]->getPropertyList();
					for (int i = 0; i < players[i]->getPropertyListSize() && check == false; i++) {
						if (temp[i]->getPropertyID() == 5) {
							check = true;
							index = i;
							rent = temp[i]->getRentPrice();
						}
					}
				}
				if (check == true) {
					players[index]->setCash(players[index]->getCash() + (2 * rent));
					players[turn]->setCash(players[turn]->getCash() - (2 * rent));
				}
				else {
					//BUY PROPERTY FUNTION HAS TO BE CALLED HERE
				}
			}
			if (pos > 5 && pos < 15) {
				players[turn]->setPlayerPosition(15);
				cells[15]->putsPlayersOnSpace(turn);
				for (int i = 0; i < playerCount && check == false; i++) {
					PrivateProperty** temp = (PrivateProperty**)players[i]->getPropertyList();
					for (int i = 0; i < players[i]->getPropertyListSize() && check == false; i++) {
						if (temp[i]->getPropertyID() == 15) {
							check = true;
							index = i;
							rent = temp[i]->getRentPrice();
						}
					}
				}
				if (check == true) {
					players[index]->setCash(players[index]->getCash() + (2 * rent));
					players[turn]->setCash(players[turn]->getCash() - (2 * rent));
				}
				else {
					//BUY PROPERTY FUNTION HAS TO BE CALLED HERE
				}
			}
			if (pos > 15 && pos < 25) {
				players[turn]->setPlayerPosition(25);
				cells[25]->putsPlayersOnSpace(turn);
				for (int i = 0; i < playerCount && check == false; i++) {
					PrivateProperty** temp = (PrivateProperty**)players[i]->getPropertyList();
					for (int i = 0; i < players[i]->getPropertyListSize() && check == false; i++) {
						if (temp[i]->getPropertyID() == 25) {
							check = true;
							index = i;
							rent = temp[i]->getRentPrice();
						}
					}
				}
				if (check == true) {
					players[index]->setCash(players[index]->getCash() + (2 * rent));
					players[turn]->setCash(players[turn]->getCash() - (2 * rent));
				}
				else {
					//BUY PROPERTY FUNTION HAS TO BE CALLED HERE
				}
			}
			if (pos > 25 && pos < 35) {
				players[turn]->setPlayerPosition(35);
				cells[35]->putsPlayersOnSpace(turn);
				for (int i = 0; i < playerCount && check == false; i++) {
					PrivateProperty** temp = (PrivateProperty**)players[i]->getPropertyList();
					for (int i = 0; i < players[i]->getPropertyListSize() && check == false; i++) {
						if (temp[i]->getPropertyID() == 35) {
							check = true;
							index = i;
							rent = temp[i]->getRentPrice();
						}
					}
				}
				if (check == true) {
					players[index]->setCash(players[index]->getCash() + (2 * rent));
					players[turn]->setCash(players[turn]->getCash() - (2 * rent));
				}
				else {
					//BUY PROPERTY FUNTION HAS TO BE CALLED HERE
				}
			}
		}
		if (id == 4) {
			int pos = 0;
			int index = 0;
			bool check = false;
			int rent = 0;
			pos = players[turn]->getPlayerPosition();
			if (pos >= 0 && pos < 5) {
				players[turn]->setPlayerPosition(5);
				cells[5]->putsPlayersOnSpace(turn);
				for (int i = 0; i < playerCount && check == false; i++) {
					PrivateProperty** temp = (PrivateProperty**)players[i]->getPropertyList();
					for (int i = 0; i < players[i]->getPropertyListSize() && check == false; i++) {
						if (temp[i]->getPropertyID() == 5) {
							check = true;
							index = i;
							rent = temp[i]->getRentPrice();
						}
					}
				}
				if (check == true) {
					players[index]->setCash(players[index]->getCash() + (2 * rent));
					players[turn]->setCash(players[turn]->getCash() - (2 * rent));
				}
				else {
					//BUY PROPERTY FUNTION HAS TO BE CALLED HERE
				}
			}
			if (pos > 5 && pos < 15) {
				players[turn]->setPlayerPosition(15);
				cells[15]->putsPlayersOnSpace(turn);
				for (int i = 0; i < playerCount && check == false; i++) {
					PrivateProperty** temp = (PrivateProperty**)players[i]->getPropertyList();
					for (int i = 0; i < players[i]->getPropertyListSize() && check == false; i++) {
						if (temp[i]->getPropertyID() == 15) {
							check = true;
							index = i;
							rent = temp[i]->getRentPrice();
						}
					}
				}
				if (check == true) {
					players[index]->setCash(players[index]->getCash() + (2 * rent));
					players[turn]->setCash(players[turn]->getCash() - (2 * rent));
				}
				else {
					//BUY PROPERTY FUNTION HAS TO BE CALLED HERE
				}
			}
			if (pos > 15 && pos < 25) {
				players[turn]->setPlayerPosition(25);
				cells[25]->putsPlayersOnSpace(turn);
				for (int i = 0; i < playerCount && check == false; i++) {
					PrivateProperty** temp = (PrivateProperty**)players[i]->getPropertyList();
					for (int i = 0; i < players[i]->getPropertyListSize() && check == false; i++) {
						if (temp[i]->getPropertyID() == 25) {
							check = true;
							index = i;
							rent = temp[i]->getRentPrice();
						}
					}
				}
				if (check == true) {
					players[index]->setCash(players[index]->getCash() + (2 * rent));
					players[turn]->setCash(players[turn]->getCash() - (2 * rent));
				}
				else {
					//BUY PROPERTY FUNTION HAS TO BE CALLED HERE
				}
			}
			if (pos > 25 && pos < 35) {
				players[turn]->setPlayerPosition(35);
				cells[35]->putsPlayersOnSpace(turn);
				for (int i = 0; i < playerCount && check == false; i++) {
					PrivateProperty** temp = (PrivateProperty**)players[i]->getPropertyList();
					for (int i = 0; i < players[i]->getPropertyListSize() && check == false; i++) {
						if (temp[i]->getPropertyID() == 35) {
							check = true;
							index = i;
							rent = temp[i]->getRentPrice();
						}
					}
				}
				if (check == true) {
					players[index]->setCash(players[index]->getCash() + (2 * rent));
					players[turn]->setCash(players[turn]->getCash() - (2 * rent));
				}
				else {
					//BUY PROPERTY FUNTION HAS TO BE CALLED HERE
				}
			}
		}
		if (id == 5) {
			int pos = players[turn]->getPlayerPosition();
			if (pos >= 0 && pos < 15) {
				players[turn]->setPlayerPosition(16);
				cells[16]->putsPlayersOnSpace(turn);
			}
			else {
				players[turn]->setPlayerPosition(16);
				cells[16]->putsPlayersOnSpace(turn);
				players[turn]->setCash(players[turn][0].getCash() + 300);
			}

		}
		if (id == 6) {
			players[turn]->setCash(players[turn][0].getCash() + 100);
		}
		if (id == 7) {
			players[turn]->setHasJailRescueCard(players[turn]->getHasJailRescueCard() + 1);
		}
		if (id == 8) {
			if (players[turn]->getPlayerPosition() >= 4) {
				players[turn]->setPlayerPosition(players[turn]->getPlayerPosition() - 4);
				cells[players[turn]->getPlayerPosition() - 4]->putsPlayersOnSpace(turn);
			}
			else {
				players[turn]->setPlayerPosition(38);
				cells[38]->putsPlayersOnSpace(turn);
			}
		}
		if (id == 9) {
			int price = 0;
			PrivateProperty** temp = (PrivateProperty**)players[turn]->getPropertyList();
			for (int i = 0; i < players[turn]->getPropertyListSize(); i++) {
				price = price + temp[i]->getHouseCount() * 50;
				price = price + temp[i]->getHotelCount() * 100;
			}
			players[turn]->setCash(players[turn][0].getCash() - price);
		}
		if (id == 10) {
			players[turn]->setCash(players[turn][0].getCash() - 25);
		}
		if (id == 11) {
			players[turn]->setCash(players[turn][0].getCash() - ((playerCount - 1) * 25));
			for (int i = 0; i < playerCount; i++) {
				if (i != turn) {
					players[i]->setCash(players[i]->getCash() + 25);
				}
			}
		}
		if (id == 12) {
			players[turn]->setCash(players[turn]->getCash() + 150);
		}
		if (id == 13) {
			if (players[turn]->getPlayerPosition() > 35) {
				players[turn]->setCash(players[turn]->getCash() + 500);
			}
			players[turn]->setPlayerPosition(35);
			cells[35]->putsPlayersOnSpace(turn);
		}
		if (id == 14) {
			if (players[turn]->getPlayerPosition() > 4) {
				players[turn]->setCash(players[turn]->getCash() + 500);
			}
			players[turn]->setPlayerPosition(4);
			cells[4]->putsPlayersOnSpace(turn);
		}
	}
	else {
		int id = C.getCardID();
		if (id == 0) {
			players[turn]->setPlayerPosition(0);
			cells[0]->putsPlayersOnSpace(turn);
			players[turn]->setCash(players[turn][0].getCash() + 400);
		}
		if (id == 1) {
			players[turn]->setCash(players[turn][0].getCash() + 200);
		}
		if (id == 2) {
			players[turn]->setCash(players[turn][0].getCash() - 200);
		}
		if (id == 3) {
			players[turn]->setCash(players[turn][0].getCash() + 50);
		}
		if (id == 4) {
			players[turn]->setHasJailRescueCard(players[turn]->getHasJailRescueCard() + 1);
		}
		if (id == 5) {
			players[turn]->setCash(players[turn][0].getCash() + 150);
		}
		if (id == 6) {
			players[turn]->setCash(players[turn][0].getCash() + 200);
		}
		if (id == 7) {
			players[turn]->setCash(players[turn][0].getCash() - 100);
		}
		if (id == 8) {
			players[turn]->setCash(players[turn][0].getCash() - 200);
		}
		if (id == 9) {
			players[turn]->setCash(players[turn][0].getCash() + 50);
		}
		if (id == 10) {
			int price = 0;
			PrivateProperty** temp = (PrivateProperty**)players[turn]->getPropertyList();
			for (int i = 0; i < players[turn]->getPropertyListSize(); i++) {
				price = price + temp[i]->getHouseCount() * 50;
				price = price + temp[i]->getHotelCount() * 125;
			}
			players[turn]->setCash(players[turn][0].getCash() - price);
		}
		if (id == 11) {
			players[turn]->setCash(players[turn][0].getCash() + 300);
		}
		if (id == 12) {
			players[turn]->setCash(players[turn][0].getCash() - 50);
		}
		if (id == 13) {
			players[turn]->setCash(players[turn][0].getCash() - 80);
		}
		if (id == 14) {
			players[turn]->setCash(players[turn][0].getCash() - 50);
		}
	}
}

void Board::shufflecards() {
	int num1 = 0;
	srand((unsigned)time(0));
	num1 = (rand() % 6) + 1;
	for (int i = 0; i < num1; i++) {
		for (int i = 0; i < 14; i++) {
			swap(Chance[i], Chance[i + 1]);
			swap(CommunityChest[i], CommunityChest[i + 1]);
		}
	}
	/*for (int i = 0; i < 15; i++) {
		cout << CommunityChest[i]->getCardAction() << " " << CommunityChest[i]->getCardID() << endl;
	}
	cout << endl;
	for (int i = 0; i < 15; i++) {
		cout << Chance[i]->getCardAction() << " " << Chance[i]->getCardID() << endl;
	}*/
}


// REVIEW

// NEW FUNNCTIONS
void Board::isAtGo(Player& a) {
	if (a.getPlayerPosition() == 20) {
		a.addCash(500);
	}
}
void Board::goToJail(Player& a) {
	if (a.getPlayerPosition() == 30) {
		// GO TO JAIL per ha
		a.setPlayerPosition(10);
	}

}



Board::~Board() {





}



// 6th June

void Board::setPlayerData(int id, char* name, int cash, int size, Property** plot, bool IsinJail, int hasJailCard, bool isBankcorrupt, int PlayerPos, int JailCount) {
	players[id]->setPlayerID(id);
	players[id]->setName(name);
	players[id]->setCash(cash);
	players[id]->setPropertyListSize(size);
	players[id]->setPropertyList(plot, size);
	players[id]->setIsInJail(IsinJail);
	players[id]->setHasJailRescueCard(hasJailCard);
	players[id]->setIsBankrupt(isBankcorrupt);
	players[id]->setPlayerPosition(PlayerPos);
	players[id]->setIsInJailCount(JailCount);
}
char* Board::getPlayerName(int i) {
	return players[i]->getName();
}

int Board::getPlayerListSize(int i) {
	return players[i]->getPropertyListSize();
}
Property** Board::getPlayerList(int i) {
	return players[i]->getPropertyList();
}
int Board::getPlayercash(int i) {
	return players[i]->getCash();
}
bool Board::getPlayerisInJail(int i) {
	return players[i]->getIsInJail();
}
int Board::getPlayerhasJailRescueCard(int i) {
	return players[i]->getHasJailRescueCard();
}
bool Board::getPlayerisbankrupt(int i) {
	return players[i]->getIsBankrupt();
}
int Board::getPlayerplayerPosition(int i) {
	return players[i]->getPlayerPosition();
}
int Board::getPlayerinJailCount(int i) {
	return players[i]->getIsInJailCount();
}