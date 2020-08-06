#pragma once


#include <SFML/Graphics.hpp>
#include <cstring>



using namespace std;

class Space
{
	//DATA MEMBERS
	char* spaceType;
	int* playersOnSpace;
	int playersOnSpaceCount;
public:
	//CONSTRUCTORS
	Space();
	Space(char*, int*, int);
	Space(Space&);
	//SETTERS
	void setSpaceType(const char*);
	void setPlayersOnSpace(int*, int);
	void setPlayersOnSpaceCount(int);
	//GETTERS
	char* getSpaceType();
	int* getPlayersOnSpace();
	int getPlayersOnSpaceCount();
	//SEPCIAL FUNCTIONS


	void putsPlayersOnSpace(int playerID);

	

	void removePlayerFromSpace(int);

	virtual ~Space();



};

