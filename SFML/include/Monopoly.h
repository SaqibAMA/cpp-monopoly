
#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <Bank.h>
#include <Board.h>
#include <SFML/Graphics.hpp>
#include <fstream>

struct Point {

	/*
	
	Point class to handle
	all board coordinates.

	*/

	int x;
	int y;

	Point() {
	
		x = 0;
		y = 0;

	}

	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}

	void set(int x, int y) {
	
		this->x = x;
		this->y = y;

	}

};

class Monopoly
{

public:

	// Constructors
	Monopoly();

	// Getters and Setters
	void setTotalPlayers(int totalPlayers);
	int getTotalPlayers();

	Board getBoard();
	Bank getBank();



	// Member functions

	void printPlayerOnCell(sf::RenderWindow&,
		sf::RectangleShape,
		int, int);
	void initializePositions();
	int getPlayerPosition(int);

	void movePlayer(int, int, sf::RenderWindow&, int, bool);

	void playDice(sf::RenderWindow&, sf::RectangleShape*,
		sf::Texture*, int);

	void updateDiceTextures(sf::Texture*);

	void setDealChoice(int);


	void checkBankruptcy();

	bool getGameWon();
	void mortgageProperty(int);



	// REVIEW


	int convertFromCellToPropertyID(int);
	bool canBuildHouse(int, int);
	bool sameupgradedlevel(int, int);



	// 6th June
	void loadGame(char*);
	void saveGame();


	~Monopoly();

private:
	Bank bank;
	Board board;
	Point* bCoord;
	int* playerPosition;
	int dealChoice;
	bool* bankruptPlayers;
	bool gameWon;


};

