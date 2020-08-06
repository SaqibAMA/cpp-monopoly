#include <stdlib.h>
#include <time.h>
#include <iostream>


#include "Space.h"
#include "PrivateProperty.h"
#include "CommercialProperty.h"
#include "Player.h"
#include "Card.h"
#include <fstream>

using namespace std;


#pragma once
class Board
{

private:
	int** prevTurns;
	int previousTurn;
	int playerCount;
	int turn;				// keeps the turn of the player
	int dRollCount;			// keeps track of double rolls
	int currRollAmount;		// keep the amount of steps that
							// a user has accumulated

	Player** players;

	Space** cells;
	int jailPosition;

	Card** Chance;				// Added 04th June
	Card** CommunityChest;

public:

	Board();

	int getPlayerCount();
	void setPlayerCount(int);

	void allocatePrevTurns();
	int** getPrevTurns();

	int* rollDice();

	int getDRollCount();
	void setDRollCount(int);

	int getTurn();
	void setTurn(int);

	int getPreviousTurn();
	void setPreviousTurn(int);

	int getCurrRollAmount();
	void setCurrRollAmount(int);

	int getJailPosition();

	Player** getPlayers();
	
	void allocatePlayers();

	Space** getCells();

	Player* getPlayerByID(int id);

	void upgradeProperty(int, int, int);
	void putPlayerOnSpace(int index, int playerID, sf::RenderWindow& window, int dealChoice);


	Card** getChance();
	Card** getCommunityChest();


	void executeCard(Card&, sf::RenderWindow& window);
	void shufflecards();



	// 6th June
	void setPlayerData(int id, char* name, int cash, int size, Property** plot, bool IsinJail, int hasJailCard, bool isBankcorrupt, int PlayerPos, int JailCount);
	char* getPlayerName(int);
	int getPlayerListSize(int);
	Property** getPlayerList(int);
	int getPlayercash(int);
	bool getPlayerisInJail(int);
	int getPlayerhasJailRescueCard(int);
	bool getPlayerisbankrupt(int);
	int getPlayerplayerPosition(int);
	int getPlayerinJailCount(int);




	/// New
	void isAtGo(Player& a);
	void goToJail(Player& a);

	~Board();
};