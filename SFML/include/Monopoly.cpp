#include "Monopoly.h"

const char* diceTextureImg[6] =
{
	"assets/dice_1.png",
	"assets/dice_2.png",
	"assets/dice_3.png",
	"assets/dice_4.png",
	"assets/dice_5.png",
	"assets/dice_6.png"

};


Monopoly::Monopoly() {

	board.setPlayerCount(0);

	bCoord = new Point[40];

	ifstream fin;
	fin.open("files/bCoord.txt");

	for (int i = 0; !fin.eof(); i++) {

		int x, y;

		fin >> x;
		fin >> y;

		bCoord[i].set(x, y);

		fin.ignore();

	}

	playerPosition = nullptr;

	dealChoice = -1;

	bankruptPlayers = nullptr;

	gameWon = false;


	fin.close();


}

// Getters and Setters

void Monopoly::setTotalPlayers(int totalPlayers) {


	board.setPlayerCount(totalPlayers);
	board.allocatePrevTurns();
	board.allocatePlayers();

	if (bankruptPlayers) delete[] bankruptPlayers;

	bankruptPlayers = new bool[board.getPlayerCount()];

	for (int i = 0; i < totalPlayers; i++) {

		bankruptPlayers[i] = false;

	}

}

int Monopoly::getTotalPlayers() {

	return board.getPlayerCount();

}

Board Monopoly::getBoard() {
	return board;
}


// Member functions

void Monopoly::printPlayerOnCell(
	sf::RenderWindow& window, sf::RectangleShape shape, int cell,
	int deviation) {

	if ((cell >= 0 && cell <= 10) ||
		(cell >= 21 && cell <= 29)) {


		sf::Vector2f playerPos = window.mapPixelToCoords(
			sf::Vector2i(bCoord[cell].x + (deviation * 7), bCoord[cell].y)
		);

		shape.setPosition(playerPos);
	}
	else {

		sf::Vector2f playerPos = window.mapPixelToCoords(
			sf::Vector2i(bCoord[cell].x, bCoord[cell].y + (deviation * 7))
		);

		shape.setPosition(playerPos);

	}

	window.draw(shape);


}


void Monopoly::initializePositions() {

	playerPosition = new int[board.getPlayerCount()];

	for (int i = 0; i < board.getPlayerCount(); i++) {
		playerPosition[i] = board.getPlayerByID(i)->getPlayerPosition();
	}


}

int Monopoly::getPlayerPosition(int playerID) {

	return playerPosition[playerID];

}

void Monopoly::movePlayer(int playerID, int currRollCount, sf::RenderWindow& window, int dealChoice = -1, bool toJail = false) {

	if (!board.getPlayerByID(playerID)->getIsBankrupt()){
		sf::Font cardFont;
		cardFont.loadFromFile("fonts/Nexa-Light.otf");

		sf::Font stdFont;
		stdFont.loadFromFile("fonts/Montserrat-Black.ttf");


		Player* player = board.getPlayerByID(playerID);

		int propertiesOwned = 0;

		for (int i = 0; i < 40; i++) {

			if (strcmp(board.getCells()[i]->getSpaceType(), "PRIVATE") == 0 || strcmp(board.getCells()[i]->getSpaceType(), "COMMERCIAL") == 0) {


				Property* property = (Property*)board.getCells()[i];

				if (property->getOwnerID() == player->getPlayerID()) {

					propertiesOwned++;

				}



			}

		}

		if (!player->getIsInJail()) {

			playerPosition[playerID] += currRollCount;
			playerPosition[playerID] %= 40;

			if (playerPosition[playerID] <= player->getPlayerPosition()) {

				player->addCash(500);

			}

			board.getPlayerByID(playerID)->setPlayerPosition(playerPosition[playerID]);
		
			if (strcmp(board.getCells()[playerPosition[playerID]]->getSpaceType(), "PRIVATE") == 0) {
		

				PrivateProperty* property = (PrivateProperty*)board.getCells()[playerPosition[playerID]];

				if (player->getPlayerID() != property->getOwnerID()) {

					sf::RenderWindow buyOrRentPrompt(sf::VideoMode(265, 166), "Buy or Rent", sf::Style::Titlebar);



					// Assets for the prompt

					//Buttons for Prompt

					sf::Texture buyButtonTexture;
					buyButtonTexture.loadFromFile("assets/property_prompt_rent.png");

					sf::RectangleShape buyButton(sf::Vector2f(128.0f, 74.0f));
					buyButton.setTexture(&buyButtonTexture);
					buyButton.setPosition(15.0f, 100.0f);

					sf::Texture rentButtonTexture;
					rentButtonTexture.loadFromFile("assets/property_prompt_rent.png");

					sf::RectangleShape rentButton(sf::Vector2f(128.0f, 74.0f));
					rentButton.setTexture(&rentButtonTexture);
					rentButton.setPosition(140.0f, 100.0f);

					// 0 index -> propertyName
					// 1 index -> propertyPrice
					// 2 index -> propertyRent
					// 3 index -> buy
					// 4 index -> sell

					int dealChoice = -1; // 0 for purchase, 1 for rent
					bool showPurchasePrompt = false;
					int onRent = -1;

					sf::Text* promptText = new sf::Text[5];


					for (int i = 0; i < 5; i++) {

						promptText[i].setFont(cardFont);
						promptText[i].setCharacterSize(14);
						promptText[i].setFillColor(sf::Color::Black);

					}

					promptText[0].setString(property->getPropertyName());
					promptText[0].setPosition(25.0f, 5.0f);

					promptText[1].setString(to_string(property->getPurchasePrice()));
					promptText[1].setPosition(25.0f, 45.0f);
					promptText[1].setFillColor(sf::Color(53, 73, 94));


					promptText[2].setString(to_string(property->getRentPrice()));
					promptText[2].setPosition(25.0f, 70.0f);
					promptText[2].setFillColor(sf::Color(53, 73, 94));


					promptText[3].setString("BUY");
					promptText[3].setFont(stdFont);
					promptText[3].setPosition(55.0f, 120.0f);
					promptText[3].setFillColor(sf::Color(236, 240, 241));

					promptText[4].setString("RENT");
					promptText[4].setFont(stdFont);
					promptText[4].setPosition(175.0f, 120.0f);
					promptText[4].setFillColor(sf::Color(236, 240, 241));

				
					sf::Texture closeBtnTexture;
					closeBtnTexture.loadFromFile("assets/upgrade_prompt_close.png");

					sf::RectangleShape closeBtn(sf::Vector2f(20.0f, 20.f));
					closeBtn.setTexture(&closeBtnTexture);
					closeBtn.setPosition(220.0f, 15.0f);

					while (buyOrRentPrompt.isOpen()) {

						sf::Event evt;

						while (buyOrRentPrompt.pollEvent(evt)) {


							if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {




								sf::Vector2f mousePos = buyOrRentPrompt.mapPixelToCoords(sf::Mouse::getPosition(buyOrRentPrompt));
								sf::FloatRect buyButtonBounds = buyButton.getGlobalBounds();
								sf::FloatRect rentButtonBounds = rentButton.getGlobalBounds();

								sf::FloatRect closeBtnBounds = closeBtn.getGlobalBounds();



								if (buyButtonBounds.contains(mousePos)) {


									dealChoice = 0;

									buyOrRentPrompt.close();


								}

								if (rentButtonBounds.contains(mousePos)) {


									dealChoice = 1;

									buyOrRentPrompt.close();


								}

								if (closeBtnBounds.contains(mousePos)) {


									int currTime = -1;


									int* bidAmount = new int[board.getPlayerCount()];
									bool* bidMade = new bool[board.getPlayerCount()];

									for (int i = 0; i < board.getPlayerCount(); i++) {

										bidAmount[i] = 0;
										bidMade[i] = true;

										if (board.getPlayerByID(i)->getIsBankrupt()) bidMade[i] = false;

									}

									int bidTurn = 0;
									int currBidAmount = 0;
									int bidsMade = 0;

									// buyOrRentPrompt.close();

									sf::RenderWindow bid(sf::VideoMode(265, 166), "BIDDING", sf::Style::Titlebar);

									sf::Clock clk;

									sf::Text currBidTitle;
									currBidTitle.setFont(stdFont);
									currBidTitle.setCharacterSize(12);
									currBidTitle.setPosition(25.0f, 15.0f);
									currBidTitle.setFillColor(sf::Color(52, 73, 94));
									currBidTitle.setString("CURRENT\nBID:");

									sf::Text currBidAmountDisplay;
									currBidAmountDisplay.setFont(stdFont);
									currBidAmountDisplay.setCharacterSize(25);
									currBidAmountDisplay.setPosition(25.0f, 45.0f);
									currBidAmountDisplay.setFillColor(sf::Color(46, 204, 113));

									sf::Text btnText[2];
								
									for (int i = 0; i < 2; i++) {
										btnText[i].setFont(stdFont);
										btnText[i].setCharacterSize(15);
										btnText[i].setPosition(32.0f + (i * 145.0f), 120.0f);
										btnText[i].setFillColor(sf::Color::White);
									}

									btnText[0].setString("BID (+10)");
									btnText[1].setString("QUIT");


									sf::Texture aucBtnTexture;
									aucBtnTexture.loadFromFile("assets/property_prompt_rent.png");

									sf::RectangleShape aucBtn[2];
									for (int i = 0; i < 2; i++) {

										aucBtn[i].setTexture(&aucBtnTexture);
										aucBtn[i].setSize(sf::Vector2f(128.0f, 74.0f));
										aucBtn[i].setPosition(15.0f + (i * 125), 100.0f);

									}


									sf::Text currTurnDisplay;
									currTurnDisplay.setFont(stdFont);
									currTurnDisplay.setString("TURN: ");
									currTurnDisplay.setCharacterSize(15);
									currTurnDisplay.setPosition(160.0f, 25.0f);
									currTurnDisplay.setFillColor(sf::Color(52, 73, 94));


									int bidWon = 0;

									while (bid.isOpen()) {
								

										// Implement the bidding system
										// Clock has been setup.
										// Check turn management.

										sf::Event bEvt;
										while (bid.pollEvent(bEvt)) {


											if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

												mousePos = buyOrRentPrompt.mapPixelToCoords(sf::Mouse::getPosition(bid));
												sf::FloatRect bidButtonBounds = aucBtn[0].getGlobalBounds();
												sf::FloatRect quitButtonBounds = aucBtn[1].getGlobalBounds();

												if (bidButtonBounds.contains(mousePos) && bidMade[bidTurn]) {

													bidMade[bidTurn] = true;
													bidAmount[bidTurn] = currBidAmount + 10;
													currBidAmount += 10;


													bidTurn = (bidTurn + 1) % board.getPlayerCount();
													bidsMade++;


												}
												else if (bidButtonBounds.contains(mousePos)){


													while (!bidMade[bidTurn])
														bidTurn = (bidTurn + 1) % board.getPlayerCount();

													bidMade[bidTurn] = true;
													bidAmount[bidTurn] = currBidAmount + 10;
													currBidAmount += 10;


													bidTurn = (bidTurn + 1) % board.getPlayerCount();
													bidsMade++;												

												}


												if (quitButtonBounds.contains(mousePos)) {
											

													if (bidTurn == 0) {
														bidMade[board.getPlayerCount() - 1] = false;
													}
													else {
														bidMade[bidTurn - 1] = false;
													}

												}

												bidWon = 0;
												int bidWonInd = -1;

												for (int i = 0; i < board.getPlayerCount(); i++) {

													bidWon += bidMade[i];
													bidWonInd = i;

												}

												if (bidWon == 1) {

													board.getPlayerByID(bidWonInd)->deductCash(currBidAmount);

													property->setOwnerID(bidWonInd);


													if (bidWonInd != player->getPlayerID()) {

														player->deductCash(property->getRentPrice());
														board.getPlayerByID(bidWonInd)->addCash(property->getRentPrice());

													}

													bid.close();

												}


											}


										}

										bid.clear(sf::Color::White);

										currBidAmountDisplay.setString(to_string(currBidAmount));
									
										bid.draw(currBidTitle);
										bid.draw(currBidAmountDisplay);
									

										currTurnDisplay.setString("TURN: " + to_string(bidTurn + 1));

										bid.draw(currTurnDisplay);

										for (int i = 0; i < 2; i++) {

											bid.draw(aucBtn[i]);
											bid.draw(btnText[i]);

										}

										bid.display();


									}




								}




							}



						}

						buyOrRentPrompt.clear(sf::Color::White);
						buyOrRentPrompt.draw(buyButton);
						buyOrRentPrompt.draw(rentButton);
						for (int i = 0; i < 5; i++)
							buyOrRentPrompt.draw(promptText[i]);

						buyOrRentPrompt.draw(closeBtn);

						buyOrRentPrompt.display();




					}

					if (player->getCash() >= property->getPurchasePrice() && dealChoice == 0) {


						if (property->getOwnerID() != -1)
							board.getPlayerByID(property->getOwnerID())->addCash(property->getPurchasePrice());

						property->setOwnerID(player->getPlayerID());
						player->deductCash(property->getPurchasePrice());


					}
					else if (player->getCash() >= property->getRentPrice() && dealChoice == 1) {

						if (property->getOwnerID() != -1 && !property->getMortgaged())
							board.getPlayerByID(property->getOwnerID())->addCash(property->getRentPrice());

						player->setIsRenting(property->getPropertyID());
						player->deductCash(property->getRentPrice());

					}
					else if (propertiesOwned && (dealChoice == 1 || dealChoice == 0)) {

						Property* barter = nullptr;

						for (int i = 0; i < 40 && !barter; i++) {


							if (strcmp(board.getCells()[i]->getSpaceType(), "PRIVATE") == 0
								|| strcmp(board.getCells()[i]->getSpaceType(), "COMMERCIAL") == 0) {

								Property* temp = (Property*)board.getCells()[i];

								if (temp->getPurchasePrice() >= property->getPurchasePrice()
									&& temp->getOwnerID() == player->getPlayerID()) {

									barter = (Property*)board.getCells()[i];

								}


							}


						}

						if (!barter) {

							//player->setIsBankrupt(true);
							//player->setCash(0);

							player->deductCash(property->getRentPrice());

							sf::RenderWindow prompt(sf::VideoMode(280, 90), "NOT ENOUGH CASH!", sf::Style::Titlebar);

							sf::Text nec;
							nec.setString("NOT ENOUGH CASH\nRenting this property\ninstead.");
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
						else {

							player->addCash(barter->getPurchasePrice());
							barter->setOwnerID(-1);
							player->deductCash(property->getPurchasePrice());
							property->setOwnerID(player->getPlayerID());

						}


					}
					else if (propertiesOwned == 0) {

						player->deductCash(property->getRentPrice());



						sf::RenderWindow prompt(sf::VideoMode(280, 90), "NOT ENOUGH CASH!", sf::Style::Titlebar);

						sf::Text nec;
						nec.setString("NOT ENOUGH CASH\nRenting this property\ninstead.");
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


						//player->setIsBankrupt(true);
						//player->setCash(0);

					}

				}



			}

			if (strcmp(board.getCells()[playerPosition[playerID]]->getSpaceType(), "COMMERCIAL") == 0) {
		

				CommercialProperty* property = (CommercialProperty*)board.getCells()[playerPosition[playerID]];

				if (player->getPlayerID() != property->getOwnerID()) {

					sf::RenderWindow buyOrRentPrompt(sf::VideoMode(265, 166), "Buy or Rent", sf::Style::Titlebar);



					// Assets for the prompt

					//Buttons for Prompt

					sf::Texture buyButtonTexture;
					buyButtonTexture.loadFromFile("assets/property_prompt_rent.png");

					sf::RectangleShape buyButton(sf::Vector2f(128.0f, 74.0f));
					buyButton.setTexture(&buyButtonTexture);
					buyButton.setPosition(15.0f, 100.0f);

					sf::Texture rentButtonTexture;
					rentButtonTexture.loadFromFile("assets/property_prompt_rent.png");

					sf::RectangleShape rentButton(sf::Vector2f(128.0f, 74.0f));
					rentButton.setTexture(&rentButtonTexture);
					rentButton.setPosition(140.0f, 100.0f);

					// 0 index -> propertyName
					// 1 index -> propertyPrice
					// 2 index -> propertyRent
					// 3 index -> buy
					// 4 index -> sell

					int dealChoice = -1; // 0 for purchase, 1 for rent
					bool showPurchasePrompt = false;
					int onRent = -1;

					sf::Text* promptText = new sf::Text[5];


					for (int i = 0; i < 5; i++) {

						promptText[i].setFont(cardFont);
						promptText[i].setCharacterSize(14);
						promptText[i].setFillColor(sf::Color::Black);

					}

					promptText[0].setString(property->getPropertyName());
					promptText[0].setPosition(25.0f, 5.0f);

					promptText[1].setString(to_string(property->getPurchasePrice()));
					promptText[1].setPosition(25.0f, 45.0f);
					promptText[1].setFillColor(sf::Color(53, 73, 94));


					promptText[2].setString(to_string(property->getRentPrice()));
					promptText[2].setPosition(25.0f, 70.0f);
					promptText[2].setFillColor(sf::Color(53, 73, 94));


					promptText[3].setString("BUY");
					promptText[3].setFont(stdFont);
					promptText[3].setPosition(55.0f, 120.0f);
					promptText[3].setFillColor(sf::Color(236, 240, 241));

					promptText[4].setString("RENT");
					promptText[4].setFont(stdFont);
					promptText[4].setPosition(175.0f, 120.0f);
					promptText[4].setFillColor(sf::Color(236, 240, 241));


					sf::Texture closeBtnTexture;
					closeBtnTexture.loadFromFile("assets/upgrade_prompt_close.png");

					sf::RectangleShape closeBtn(sf::Vector2f(20.0f, 20.f));
					closeBtn.setTexture(&closeBtnTexture);
					closeBtn.setPosition(220.0f, 15.0f);

					while (buyOrRentPrompt.isOpen()) {

						sf::Event evt;

						while (buyOrRentPrompt.pollEvent(evt)) {


							if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {




								sf::Vector2f mousePos = buyOrRentPrompt.mapPixelToCoords(sf::Mouse::getPosition(buyOrRentPrompt));
								sf::FloatRect buyButtonBounds = buyButton.getGlobalBounds();
								sf::FloatRect rentButtonBounds = rentButton.getGlobalBounds();

								sf::FloatRect closeBtnBounds = closeBtn.getGlobalBounds();



								if (buyButtonBounds.contains(mousePos)) {


									dealChoice = 0;

									buyOrRentPrompt.close();


								}

								if (rentButtonBounds.contains(mousePos)) {


									dealChoice = 1;

									buyOrRentPrompt.close();


								}

								if (closeBtnBounds.contains(mousePos)) {


									int currTime = -1;


									int* bidAmount = new int[board.getPlayerCount()];
									bool* bidMade = new bool[board.getPlayerCount()];

									for (int i = 0; i < board.getPlayerCount(); i++) {

										bidAmount[i] = 0;
										bidMade[i] = true;

										if (board.getPlayerByID(i)->getIsBankrupt()) bidMade[i] = false;

									}

									int bidTurn = 0;
									int currBidAmount = 0;
									int bidsMade = 0;

									// buyOrRentPrompt.close();

									sf::RenderWindow bid(sf::VideoMode(265, 166), "BIDDING", sf::Style::Titlebar);

									sf::Clock clk;

									sf::Text currBidTitle;
									currBidTitle.setFont(stdFont);
									currBidTitle.setCharacterSize(12);
									currBidTitle.setPosition(25.0f, 15.0f);
									currBidTitle.setFillColor(sf::Color(52, 73, 94));
									currBidTitle.setString("CURRENT\nBID:");

									sf::Text currBidAmountDisplay;
									currBidAmountDisplay.setFont(stdFont);
									currBidAmountDisplay.setCharacterSize(25);
									currBidAmountDisplay.setPosition(25.0f, 45.0f);
									currBidAmountDisplay.setFillColor(sf::Color(46, 204, 113));

									sf::Text btnText[2];

									for (int i = 0; i < 2; i++) {
										btnText[i].setFont(stdFont);
										btnText[i].setCharacterSize(15);
										btnText[i].setPosition(32.0f + (i * 145.0f), 120.0f);
										btnText[i].setFillColor(sf::Color::White);
									}

									btnText[0].setString("BID (+10)");
									btnText[1].setString("QUIT");


									sf::Texture aucBtnTexture;
									aucBtnTexture.loadFromFile("assets/property_prompt_rent.png");

									sf::RectangleShape aucBtn[2];
									for (int i = 0; i < 2; i++) {

										aucBtn[i].setTexture(&aucBtnTexture);
										aucBtn[i].setSize(sf::Vector2f(128.0f, 74.0f));
										aucBtn[i].setPosition(15.0f + (i * 125), 100.0f);

									}


									sf::Text currTurnDisplay;
									currTurnDisplay.setFont(stdFont);
									currTurnDisplay.setString("TURN: ");
									currTurnDisplay.setCharacterSize(15);
									currTurnDisplay.setPosition(160.0f, 25.0f);
									currTurnDisplay.setFillColor(sf::Color(52, 73, 94));


									int bidWon = 0;

									while (bid.isOpen()) {


										// Implement the bidding system
										// Clock has been setup.
										// Check turn management.

										sf::Event bEvt;
										while (bid.pollEvent(bEvt)) {


											if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

												mousePos = buyOrRentPrompt.mapPixelToCoords(sf::Mouse::getPosition(bid));
												sf::FloatRect bidButtonBounds = aucBtn[0].getGlobalBounds();
												sf::FloatRect quitButtonBounds = aucBtn[1].getGlobalBounds();

												if (bidButtonBounds.contains(mousePos) && bidMade[bidTurn]) {

													bidMade[bidTurn] = true;
													bidAmount[bidTurn] = currBidAmount + 10;
													currBidAmount += 10;


													bidTurn = (bidTurn + 1) % board.getPlayerCount();
													bidsMade++;

													cout << "bidTurn -> " << bidTurn << endl;


												}
												else if (bidButtonBounds.contains(mousePos)) {


													while (!bidMade[bidTurn])
														bidTurn = (bidTurn + 1) % board.getPlayerCount();

													bidMade[bidTurn] = true;
													bidAmount[bidTurn] = currBidAmount + 10;
													currBidAmount += 10;


													bidTurn = (bidTurn + 1) % board.getPlayerCount();
													bidsMade++;

													cout << "bidTurn -> " << bidTurn << endl;


												}


												if (quitButtonBounds.contains(mousePos)) {


													if (bidTurn == 0) {
														bidMade[board.getPlayerCount() - 1] = false;
													}
													else {
														bidMade[bidTurn - 1] = false;
													}

												}

												bidWon = 0;
												int bidWonInd = -1;

												for (int i = 0; i < board.getPlayerCount(); i++) {

													bidWon += bidMade[i];
													bidWonInd = i;

												}

												if (bidWon == 1) {

													board.getPlayerByID(bidWonInd)->deductCash(currBidAmount);

													property->setOwnerID(bidWonInd);


													if (bidWonInd != player->getPlayerID()) {

														player->deductCash(property->getRentPrice());
														board.getPlayerByID(bidWonInd)->addCash(property->getRentPrice());

													}

													bid.close();

												}


											}


										}

										bid.clear(sf::Color::White);

										currBidAmountDisplay.setString(to_string(currBidAmount));

										bid.draw(currBidTitle);
										bid.draw(currBidAmountDisplay);


										currTurnDisplay.setString("TURN: " + to_string(bidTurn + 1));

										bid.draw(currTurnDisplay);

										for (int i = 0; i < 2; i++) {

											bid.draw(aucBtn[i]);
											bid.draw(btnText[i]);

										}

										bid.display();


									}




								}




							}



						}

						buyOrRentPrompt.clear(sf::Color::White);
						buyOrRentPrompt.draw(buyButton);
						buyOrRentPrompt.draw(rentButton);
						for (int i = 0; i < 5; i++)
							buyOrRentPrompt.draw(promptText[i]);

						buyOrRentPrompt.draw(closeBtn);

						buyOrRentPrompt.display();




					}

					if (player->getCash() >= property->getPurchasePrice() && dealChoice == 0) {


						if (property->getOwnerID() != -1)
							board.getPlayerByID(property->getOwnerID())->addCash(property->getPurchasePrice());

						property->setOwnerID(player->getPlayerID());
						player->deductCash(property->getPurchasePrice());


					}
					else if (player->getCash() >= property->getRentPrice() && dealChoice == 1) {

						if (property->getOwnerID() != -1 && !property->getMortgaged())
							board.getPlayerByID(property->getOwnerID())->addCash(property->getRentPrice());

						player->setIsRenting(property->getPropertyID());
						player->deductCash(property->getRentPrice());

					}
					else if (propertiesOwned && (dealChoice == 1 || dealChoice == 0)) {

						Property* barter = nullptr;

						for (int i = 0; i < 40 && !barter; i++) {


							if (strcmp(board.getCells()[i]->getSpaceType(), "PRIVATE") == 0
								|| strcmp(board.getCells()[i]->getSpaceType(), "COMMERCIAL") == 0) {

								Property* temp = (Property*)board.getCells()[i];

								if (temp->getPurchasePrice() >= property->getPurchasePrice()
									&& temp->getOwnerID() == player->getPlayerID()) {

									barter = (Property*)board.getCells()[i];

								}


							}


						}

						if (!barter) {

							//player->setIsBankrupt(true);
							//player->setCash(0);
							player->deductCash(property->getRentPrice());

							sf::RenderWindow prompt(sf::VideoMode(280, 90), "NOT ENOUGH CASH!", sf::Style::Titlebar);

							sf::Text nec;
							nec.setString("NOT ENOUGH CASH\nRenting this property\ninstead.");
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
						else {

							player->addCash(barter->getPurchasePrice());
							barter->setOwnerID(-1);
							player->deductCash(property->getPurchasePrice());
							property->setOwnerID(player->getPlayerID());

						}


					}
					else if (propertiesOwned == 0) {

						//player->setIsBankrupt(true);
						//player->setCash(0);

						player->deductCash(property->getRentPrice());


						sf::RenderWindow prompt(sf::VideoMode(280, 90), "NOT ENOUGH CASH!", sf::Style::Titlebar);

						sf::Text nec;
						nec.setString("NOT ENOUGH CASH\nRenting this property\ninstead.");
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

				}


			}

			if (strcmp(board.getCells()[playerPosition[playerID]]->getSpaceType(), "GO") == 0) {

				board.getPlayerByID(playerID)->addCash(500);

			}

			if (strcmp(board.getCells()[playerPosition[playerID]]->getSpaceType(), "COMMUNITY") == 0) {


				int randCard = rand() % 15;

				CommunityCard* c = (CommunityCard*)board.getCommunityChest()[randCard];

				sf::RenderWindow card(sf::VideoMode(500, 250), "COMMUNITY CHEST CARD", sf::Style::Titlebar);


				sf::Texture cardBGTexture;
				cardBGTexture.loadFromFile("assets/communityBG.png");
				sf::RectangleShape cardBG(sf::Vector2f(500.0f, 250.0f));
				cardBG.setTexture(&cardBGTexture);


				sf::Text cardTitle;
				cardTitle.setString("COMMUNITY\nCHEST");
				cardTitle.setCharacterSize(25);
				cardTitle.setFont(stdFont);
				cardTitle.setPosition(15.0f, 15.0f);
				cardTitle.setFillColor(sf::Color::White);


				char* _cardText = c->getCardAction();

				for (int i = 0; i < strlen(_cardText); i++) {
					if (_cardText[i] == '.') {
						_cardText[i] = '\n';

						if (i + 1 < strlen(_cardText)) {

							for (int j = i + 1; j < strlen(_cardText); j++)
								_cardText[j] = _cardText[j + 1];

						}

					}

				}
			

				sf::Text cardText;
				cardText.setFont(cardFont);
				cardText.setString(_cardText);
				cardText.setPosition(15.0f, 100.0f);
				cardText.setCharacterSize(22);
				cardText.setFillColor(sf::Color::White);

				sf::Text closeButton;
				closeButton.setString("X");
				closeButton.setCharacterSize(20);
				closeButton.setFont(stdFont);
				closeButton.setPosition(470.0f, 10.0f);
				closeButton.setFillColor(sf::Color::White);


				while (card.isOpen()) {


					sf::Event evt;
					while (card.pollEvent(evt)) {


						if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

							sf::Vector2f mousePos = card.mapPixelToCoords(sf::Mouse::getPosition(card));
							sf::FloatRect closeButtonBounds = closeButton.getGlobalBounds();

							if (closeButtonBounds.contains(mousePos)) {

								card.close();

							}


						}


					}

					card.clear(sf::Color(52, 152, 219));
					card.draw(cardBG);

					card.draw(cardTitle);
					card.draw(cardText);
					card.draw(closeButton);
					card.display();


				}



				// Functionality of All Cards

				if (randCard == 0) {


					playerPosition[playerID] = 0;
					player->setPlayerPosition(0);
					player->addCash(400);


				}
				else if (randCard == 1) {

					player->addCash(200);

				}
				else if (randCard == 2) {
			
					player->deductCash(200);

				}
				else if (randCard == 3) {
			
					player->addCash(50);

				}
				else if (randCard == 4) {
			
					player->setHasJailRescueCard(player->getHasJailRescueCard() + 1);

				}
				else if (randCard == 5) {

					player->addCash(150);

				}
				else if (randCard == 6) {

					player->addCash(200);
			
				}
				else if (randCard == 7) {
			
					player->deductCash(100);

				}
				else if (randCard == 8) {
			
					player->deductCash(200);

				}
				else if (randCard == 9) {

					player->addCash(50);

				}
				else if (randCard == 10) {

					Space** cells = board.getCells();

					int totalCharges = 0;
				
					for (int i = 0; i < 40; i++) {
				
						if (strcmp(cells[i]->getSpaceType(), "PRIVATE") == 0 ||
							strcmp(cells[i]->getSpaceType(), "COMMERCIAL") == 0) {
					
							Property* temp = (Property*)cells[i];


							if (temp->getOwnerID() == player->getPlayerID()) {
						
								totalCharges += ((temp->getHotelCount() * 125) +
									(temp->getHouseCount() * 50));

							}


					
						}

					}

					player->deductCash(totalCharges);


				}
				else if (randCard == 11) {
			
					player->addCash(300);

				}
				else if (randCard == 12) {

					player->deductCash(50);

				}
				else if (randCard == 13) {
			
					player->deductCash(80);

				}
				else if (randCard == 14) {

					player->deductCash(50);

				}


			}

			if (strcmp(board.getCells()[playerPosition[playerID]]->getSpaceType(), "CHANCE") == 0) {
		
				int randCard = rand() % 15;

				ChanceCard* c = (ChanceCard*)board.getChance()[randCard];

				sf::RenderWindow card(sf::VideoMode(500, 250), "Chance Card", sf::Style::Titlebar);


				sf::Texture cardBGTexture;
				cardBGTexture.loadFromFile("assets/chanceBG.png");
				sf::RectangleShape cardBG(sf::Vector2f(500.0f, 250.0f));
				cardBG.setTexture(&cardBGTexture);


				sf::Text cardTitle;
				cardTitle.setString("CHANCE CARD.");
				cardTitle.setCharacterSize(25);
				cardTitle.setFont(stdFont);
				cardTitle.setPosition(15.0f, 15.0f);
				cardTitle.setFillColor(sf::Color::White);


				
				
				char* _cardText = c->getCardAction();

				for (int i = 0; i < strlen(_cardText); i++) {
					if (_cardText[i] == '.') {
						_cardText[i] = '\n';

						if (i + 1 < strlen(_cardText)) {

							for (int j = i + 1; j < strlen(_cardText); j++)
								_cardText[j] = _cardText[j + 1];

						}

					}
					
				}

				sf::Text cardText;
				cardText.setFont(cardFont);
				cardText.setString(_cardText);
				cardText.setPosition(15.0f, 100.0f);
				cardText.setCharacterSize(22);
				cardText.setFillColor(sf::Color::White);

				sf::Text closeButton;
				closeButton.setString("X");
				closeButton.setCharacterSize(20);
				closeButton.setFont(stdFont);
				closeButton.setPosition(470.0f, 10.0f);
				closeButton.setFillColor(sf::Color::White);

				//sf::Texture closeButtonTexture;
				//closeButtonTexture.loadFromFile("assets/upgrade_prompt_close.png");
				//sf::RectangleShape closeButton(sf::Vector2f(20.0f, 20.0f));
				//closeButton.setTexture(&closeButtonTexture);
				//closeButton.setPosition(470.0f, 10.0f);
				//closeButtonTexture.setSmooth(true);
				//closeButton.setFillColor(sf::Color::White);


				while (card.isOpen()) {


					sf::Event evt;
					while (card.pollEvent(evt)) {


						if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

							sf::Vector2f mousePos = card.mapPixelToCoords(sf::Mouse::getPosition(card));
							sf::FloatRect closeButtonBounds = closeButton.getGlobalBounds();

							if (closeButtonBounds.contains(mousePos)) {

								card.close();

							}


						}


					}

					card.clear(sf::Color::White);
					card.draw(cardBG);

					card.draw(cardTitle);
					card.draw(cardText);
					card.draw(closeButton);
					card.display();


				}



				// card actions

				if (randCard == 0) {
			
					playerPosition[playerID] = 0;
					player->setPlayerPosition(0);
					player->addCash(300);

				}
				else if (randCard == 1) {
			
				
					int diff = abs(player->getPlayerPosition() - 26);
					movePlayer(playerID, diff, window);


				}
				else if (randCard == 2) {

					Space** cells = board.getCells();

					int nearestIndex = 0;

					for (int i = playerPosition[playerID]; i < playerPosition[playerID] + 40 && !nearestIndex; i++) {
				
					

						if (strcmp(cells[i % 40]->getSpaceType(), "COMMERCIAL") == 0) {

							Property* temp = (Property* )cells[i % 40];

							if (strcmp(temp->getPropertyGroup(), "UTILITY") == 0) {
						
								nearestIndex = i % 40;

							}

						}

					}

					Property* nearestUtility = (Property* )cells[nearestIndex];

					if (nearestUtility->getOwnerID() == -1) {
				
						int diff = abs(nearestIndex - playerPosition[playerID]);
					
						movePlayer(playerID, diff, window);


					}
					else {

						int diceRolled = board.getPrevTurns()[board.getPlayerCount()][0] + 
							board.getPrevTurns()[board.getPlayerCount()][1];

						board.getPlayerByID(nearestUtility->getOwnerID())->addCash(diceRolled * 5);
						player->deductCash(diceRolled * 5);

						playerPosition[playerID] = nearestIndex;
						player->setPlayerPosition(nearestIndex);

					}



				}
				else if (randCard == 3 || randCard == 4) {
			



					Space** cells = board.getCells();

					int nearestIndex = 0;

					for (int i = playerPosition[playerID]; i < playerPosition[playerID] + 40 && !nearestIndex; i++) {



						if (strcmp(cells[i % 40]->getSpaceType(), "COMMERCIAL") == 0) {

							Property* temp = (Property*)cells[i % 40];

							if (strcmp(temp->getPropertyGroup(), "STATION") == 0) {

								nearestIndex = i % 40;

							}

						}

					}

					Property* nearestUtility = (Property*)cells[nearestIndex];

					if (nearestUtility->getOwnerID() == -1) {

						int diff = abs(nearestIndex - playerPosition[playerID]);

						movePlayer(playerID, diff, window);


					}
					else {

						board.getPlayerByID(nearestUtility->getOwnerID())->addCash(nearestUtility->getPurchasePrice() * 2);
						player->deductCash(nearestUtility->getPurchasePrice() * 2);

						playerPosition[playerID] = nearestIndex;
						player->setPlayerPosition(nearestIndex);

					}




				}
				else if (randCard == 5) {

			
					int diff = 16 - player->getPlayerPosition();

					if (diff <= 0) {

						player->deductCash(200);
						movePlayer(playerID, abs(diff), window);

					}
					else {

						movePlayer(playerID, diff, window);

					}


				}
				else if (randCard == 6) {

					player->addCash(100);

				}
				else if (randCard == 7) {

					player->setHasJailRescueCard(player->getHasJailRescueCard() + 1);

				}
				else if (randCard == 8) {

					movePlayer(player->getPlayerID(), 36, window);
					player->deductCash(500);

				}
				else if (randCard == 9) {

			
					Space** cells = board.getCells();

					int totalCharges = 0;

					for (int i = 0; i < 40; i++) {

						if (strcmp(cells[i]->getSpaceType(), "PRIVATE") == 0 ||
							strcmp(cells[i]->getSpaceType(), "COMMERCIAL") == 0) {

							Property* temp = (Property*)cells[i];


							if (temp->getOwnerID() == player->getPlayerID()) {

								totalCharges += ((temp->getHotelCount() * 100) +
									(temp->getHouseCount() * 50));

							}


						}
					
					}

			

					player->deductCash(totalCharges);


				}
				else if (randCard == 10) {


					player->deductCash(25);


				}
				else if (randCard == 11) {
				
					Player** allPlayers = board.getPlayers();

					for (int i = 0; i < board.getPlayerCount(); i++) {

						if (!allPlayers[i]->getIsBankrupt()) {

							player->deductCash(25);
							allPlayers[i]->addCash(25);

						}

					}


				}
				else if (randCard == 12) {

					player->addCash(150);

				}
				else if (randCard == 13) {

					playerPosition[playerID] = 35;
					player->setPlayerPosition(35);

				}
				else if (randCard == 14) {

					int diff = abs(player->getPlayerPosition() - 5);
					movePlayer(playerID, diff, window);

				}



			}

			if (strcmp(board.getCells()[playerPosition[playerID]]->getSpaceType(), "PARKING") == 0) {
		
				player->addCash(10);

			}

			if (strcmp(board.getCells()[playerPosition[playerID]]->getSpaceType(), "LANDTAX") == 0) {

				float totalTax = 0;

				Space** cells = board.getCells();

				for (int i = 0; i < 40; i++) {
			
					if (strcmp(board.getCells()[i]->getSpaceType(), "PRIVATE") == 0 || strcmp(board.getCells()[i]->getSpaceType(), "COMMERCIAL") == 0) {
				

						Property* temp = (Property* ) cells[i];

						if (temp->getOwnerID() == player->getPlayerID()) {
					
							totalTax += (int)(temp->getPurchasePrice() * 0.2);

						}
				

					}

				}

				sf::RenderWindow taxPrompt(sf::VideoMode(300, 160), "Land Tax", sf::Style::Titlebar);


				sf::Text cardTitle;
				cardTitle.setString("Land Tax.");
				cardTitle.setCharacterSize(25);
				cardTitle.setFont(stdFont);
				cardTitle.setPosition(15.0f, 15.0f);
				cardTitle.setFillColor(sf::Color::White);

				sf::Text cardText;
				cardText.setFont(cardFont);
				cardText.setString("20% Land Tax has been deducted!");
				cardText.setPosition(15.0f, 100.0f);
				cardText.setCharacterSize(15);
				cardText.setFillColor(sf::Color::White);

				sf::Texture closeButtonTexture;
				closeButtonTexture.loadFromFile("assets/upgrade_prompt_close.png");
				sf::RectangleShape closeButton(sf::Vector2f(20.0f, 20.0f));
				closeButton.setTexture(&closeButtonTexture);
				closeButton.setPosition(270.0f, 10.0f);
				closeButtonTexture.setSmooth(true);





				while (taxPrompt.isOpen()) {
			

					sf::Event evt;
					while (taxPrompt.pollEvent(evt)) {
				
						if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					
							sf::Vector2f mousePos = taxPrompt.mapPixelToCoords(sf::Mouse::getPosition(taxPrompt));
							sf::FloatRect closeButtonBounds = closeButton.getGlobalBounds();


							if (closeButtonBounds.contains(mousePos)) {
						

								taxPrompt.close();
								player->deductCash((int)totalTax);

							}




						}

				
					}


					taxPrompt.clear(sf::Color(26, 188, 156));
					taxPrompt.draw(cardTitle);
					taxPrompt.draw(cardText);
					taxPrompt.draw(closeButton);

					taxPrompt.display();


				}







			}
			
			if (strcmp(board.getCells()[playerPosition[playerID]]->getSpaceType(), "GOTOJAIL") == 0) {
		


				sf::RenderWindow jail(sf::VideoMode(500, 200), " ", sf::Style::Titlebar);

				sf::Text cardTitle;
				cardTitle.setString("JAIL OPTIONS");
				cardTitle.setCharacterSize(25);
				cardTitle.setFont(stdFont);
				cardTitle.setPosition(15.0f, 15.0f);
				cardTitle.setFillColor(sf::Color(52, 73, 94));

				sf::Text cardText;
				cardText.setFont(cardFont);
				cardText.setString("You have been jailed.\nSelect the option that you would like\nto use.");
				cardText.setPosition(15.0f, 60.0f);
				cardText.setCharacterSize(15);
				cardText.setFillColor(sf::Color(52, 73, 94));

				sf::RectangleShape* btn = new sf::RectangleShape[4];
				for (int i = 0; i < 4; i++) {
			
					btn[i].setFillColor(sf::Color(52, 73, 94));
					btn[i].setSize(sf::Vector2f(100.f, 50.0f));
					btn[i].setPosition(15.0f + (i * 120.0f), 130.0f);

				}

				sf::Text* btnText = new sf::Text[4];
				for (int i = 0; i < 4; i++) {

					btnText[i].setCharacterSize(12);
					btnText[i].setFont(stdFont);
					btnText[i].setFillColor(sf::Color(236, 240, 241));

				}

				btnText[0].setString("\t TRY\nDOUBLES");
				btnText[1].setString("USE CARD");
				btnText[2].setString("PAY FINE");
				btnText[3].setString("BUY CARD");

				btnText[0].setPosition(30.0f , 140.0f);
				btnText[1].setPosition(150.0f, 148.0f);
				btnText[2].setPosition(275.0f, 148.0f);
				btnText[3].setPosition(390.0f, 148.0f);

				sf::Text err;
				//err.setString("fine");
				err.setCharacterSize(12);
				err.setPosition(15.0f, 110.0f);
				err.setFillColor(sf::Color(231, 76, 60));
				err.setFont(stdFont);


				while (jail.isOpen()) {
			

					sf::Event evt;
					while (jail.pollEvent(evt)) {

						if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

							sf::Vector2f mousePos = jail.mapPixelToCoords(sf::Mouse::getPosition(jail));

							sf::FloatRect btnBound[4];
							for (int i = 0; i < 4; i++) {

								btnBound[i] = btn[i].getGlobalBounds();


								if (btnBound[i].contains(mousePos)) {


									// options
									// try to roll doubles
									// use "get out of jail card"
									// pay fine
									// buy "get out of jail card"



									if (i == 0) {

										player->setIsInJail(true);
										player->setIsInJailCount(2);

										jail.close();

									}
									else if (i == 1) {

										if (player->getHasJailRescueCard()) {

											player->setHasJailRescueCard(player->getHasJailRescueCard() - 1);
											player->setIsInJail(false);
											player->setIsInJailCount(0);

											jail.close();

										}
										else {
											err.setString("You do not have a jail rescue card!");
										}

									}
									else if (i == 2) {

										if (player->getCash() >= 400) {

											player->deductCash(400);
											player->setIsInJail(false);
											player->setIsInJailCount(0);
											playerPosition[playerID] = 30;
											player->setPlayerPosition(30);

											jail.close();

										}
										else {

											err.setString("You do not have any cash!    * ghamgeen lamha *");

										}

									}
									else if (i == 3) {

										if (player->getCash() >= 400) {

											int jailCardExists = -1;

											Player** allPlayers = board.getPlayers();

											for (int i = 0; i < board.getPlayerCount() && jailCardExists == -1; i++) {

												cout << "Player " << i << " -> " << allPlayers[i]->getHasJailRescueCard() << endl;

												if (!allPlayers[i]->getIsBankrupt() && allPlayers[i]->getHasJailRescueCard()) {

													jailCardExists = i;

												}

											}

											if (jailCardExists != -1) {


												sf::RenderWindow bin(sf::VideoMode(250, 120), "YES OR NO?", sf::Style::Titlebar);

												sf::RectangleShape yn[2];
												sf::Text ynText[2];

												for (int i = 0; i < 2; i++) {

													yn[i].setFillColor(sf::Color(52, 73, 94));
													yn[i].setSize(sf::Vector2f(100.0f, 50.0f));
													yn[i].setPosition(12.0f + (i * 120.0f), 20.0f);

													ynText[i].setCharacterSize(13);
													ynText[i].setFont(stdFont);
													ynText[i].setFillColor(sf::Color::White);
													ynText[i].setPosition(45.0f + (i * 125.0f), 35.0f);
													ynText[i].setString((i == 0) ? "YES" : "NO");

												}

												sf::Text note;
												note.setString("Do you want\nto sell your card?");
												note.setFont(cardFont);
												note.setFillColor(sf::Color(52, 73, 94));
												note.setPosition(12.0f, 80.0f);
												note.setCharacterSize(14);

												while (bin.isOpen()) {


													sf::Event bEvt;
													while (bin.pollEvent(bEvt)) {



														if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {


															mousePos = jail.mapPixelToCoords(sf::Mouse::getPosition(bin));

															sf::FloatRect ynBounds[2];
															ynBounds[0] = yn[0].getGlobalBounds();
															ynBounds[1] = yn[1].getGlobalBounds();


															if (ynBounds[0].contains(mousePos)) {


																player->deductCash(400);
																allPlayers[jailCardExists]->addCash(400);
																allPlayers[jailCardExists]->setHasJailRescueCard(allPlayers[jailCardExists]->getHasJailRescueCard() - 1);

																bin.close();
																jail.close();


															}
															else if (ynBounds[1].contains(mousePos)) {


																err.setString("Player refused to sell.");
															
															
																bin.close();


															}




														}



													}


													bin.clear(sf::Color(236, 240, 241));

													for (int i = 0; i < 2; i++) {

														bin.draw(yn[i]);
														bin.draw(ynText[i]);

													}

													bin.draw(note);

													bin.display();


												}




											}
											else {

												err.setString("No one owns a \"get out of jail card\" yet.");

											}


										}
										else {

											err.setString("You do not have any cash  * ghamgeen lamha *");

										}


									}


								}


							}

						}



					}


					jail.clear(sf::Color(236, 240, 241));
					jail.draw(cardTitle);
					jail.draw(cardText);
				

					for (int i = 0; i < 4; i++) {
						jail.draw(btn[i]);
						jail.draw(btnText[i]);
					}

					jail.draw(err);

					jail.display();
			
				}

				delete[] btn;
				delete[] btnText;

				if (player->getIsInJail() && toJail) {

					player->setPlayerPosition(10);
					playerPosition[playerID] = 10;
					player->setIsInJailCount(2);

				}




			}

			if (strcmp(board.getCells()[playerPosition[playerID]]->getSpaceType(), "PROPERTYTAX") == 0) {
		
				/*
			
				Pay 10% tax on land, 20% tax on houses and 30% on hotels
				and shops on each visit of this cell

				*/


				Space** cells = board.getCells();

				float totalTax = 0;


				for (int i = 0; i < 40; i++) {


					if (strcmp(cells[i]->getSpaceType(), "PRIVATE") == 0) {
				
						PrivateProperty* p = (PrivateProperty*) cells[i];


						totalTax += (
						
							(p->getPurchasePrice() * 0.10f) +
							(p->getHouseCount() * 20.0f) +
							(p->getHotelCount() * 210.0f) +
							(p->getShopCount() * 90.0f)

							);



					}
					else if (strcmp(cells[i]->getSpaceType(), "COMMERCIAL") == 0) {
				
						CommercialProperty* p = (CommercialProperty*)cells[i];

						totalTax += (

							(p->getPurchasePrice() * 0.10f)

							);


					}


				}


				player->deductCash(totalTax);



			}



		}
		else {


			// Check if the player has rolled doubles. If he does, the player gets out.

			int diceRoll[2];

			diceRoll[0] = board.getPrevTurns()[board.getPlayerCount() - 1][0];
			diceRoll[1] = board.getPrevTurns()[board.getPlayerCount() - 1][1];

			if (diceRoll[0] == diceRoll[1] && player->getIsInJailCount() == 2) {

				player->setIsInJail(false);
				player->setIsInJailCount(0);

				movePlayer(diceRoll[0] + diceRoll[1], playerID, window);

			}
			else {

				player->setIsInJailCount(player->getIsInJailCount() - 1);

				if (player->getIsInJailCount() == 0)
					player->setIsInJail(false);

			}


		}


	{


		if (toJail || player->getIsInJail()) {
			// also set the boolean to true
			// also set the inJailCount to 2
			playerPosition[playerID] = 10;
			player->setPlayerPosition(10);

			if (!player->getIsInJailCount())
				player->setIsInJailCount(2);

		}


	}

	}

}

void Monopoly::playDice(sf::RenderWindow& window,
	sf::RectangleShape* dice, sf::Texture* diceTexture, int dealChoice) {


	if (!gameWon) {

		if (board.getTurn() == 0) {


			checkBankruptcy();


		}




		int* diceNum = board.rollDice();

		//if (existsInProp) {

		//if (!bankruptPlayers[board.getTurn()]) {

			board.getPlayerByID(board.getTurn())->setIsRenting(-1);

			if (diceNum[0] > 0 && diceNum[1] > 0 && board.getDRollCount() < 3 && !bankruptPlayers[board.getTurn()]) {


				if (diceNum[0] == diceNum[1] && board.getDRollCount()) {

					movePlayer(board.getTurn(), diceNum[0] + diceNum[1], window, dealChoice);

				}
				else if (diceNum[0] != diceNum[1] && !board.getDRollCount()) {

					movePlayer(board.getPreviousTurn(), diceNum[0] + diceNum[1], window, dealChoice);

				}
				else if (diceNum[0] != diceNum[1] && board.getDRollCount()) {

					board.setDRollCount(0);

					movePlayer(board.getPreviousTurn(), diceNum[0] + diceNum[1], window, dealChoice);

					board.setTurn(

						(board.getTurn() + 1) % board.getPlayerCount()

					);

				}
			}
			else {


				/*if (bankruptPlayers[board.getTurn()]) {
				
					while (bankruptPlayers[board.getTurn()]) {
						board.setTurn(

							(board.getTurn() + 1) % board.getPlayerCount()

						);
					}

				}*/


				if (board.getDRollCount() == 3) {

					movePlayer(board.getTurn(), 0, window, dealChoice, true);


					board.setTurn(

						(board.getTurn() + 1)
						% board.getPlayerCount()

					);

					board.setDRollCount(0);

				}

			}

	}

}

void Monopoly::updateDiceTextures(sf::Texture* diceTexture) {


	diceTexture[0].loadFromFile(
		diceTextureImg[
			board.getPrevTurns()[board.getPlayerCount() - 1][0]
				- (board.getPrevTurns()[board.getPlayerCount() - 1][0] > 0)
		]
	);

	diceTexture[1].loadFromFile(
		diceTextureImg[
			board.getPrevTurns()[board.getPlayerCount() - 1][1]
				- (board.getPrevTurns()[board.getPlayerCount() - 1][1] > 0)
		]
	);


}


Bank Monopoly::getBank() {
	return bank;
}


void Monopoly::setDealChoice(int dealChoice) {

	this->dealChoice = dealChoice;

}



void Monopoly::checkBankruptcy() {


	Player** player = board.getPlayers();


	for (int i = 0; i < board.getPlayerCount(); i++) {
	
		bool isBankrupt = false;

		if (player[i]->getCash() <= 0) {

			Space** cells = board.getCells();


			int propertiesOwned = 0;


			for (int j = 0; j < 40; j++) {


				if (strcmp(cells[j]->getSpaceType(), "PRIVATE") == 0 || strcmp(cells[j]->getSpaceType(), "COMMERCIAL") == 0) {
				

					Property* p = (Property*)cells[j];


					if (p->getOwnerID() == player[i]->getPlayerID()) {
					
						propertiesOwned++;


					}


				}


			}


			//cout << "Properties owned by " << i << ", " << propertiesOwned << endl;


			isBankrupt = (propertiesOwned <= 0);


			if (isBankrupt) {
				
				player[i]->setIsBankrupt(true);
				bankruptPlayers[player[i]->getPlayerID()] = true;
			
			}
			else {


				int deficit = abs(player[i]->getCash());
				int minDiff = INT_MAX;
				int propertyInd = -1;


				for (int j = 0; j < 40; j++) {


					if (strcmp(cells[j]->getSpaceType(), "PRIVATE") == 0 || strcmp(cells[j]->getSpaceType(), "COMMERCIAL") == 0) {


						Property* p = (Property*)cells[j];

						//cout << "Owner -> " << p->getOwnerID() << endl;
						//cout << "Player ID -> " << player[i]->getPlayerID() << endl;
						//cout << "Mortgaged -> " << p->getMortgaged() << endl;
						//cout << "Deficit -> " << deficit << endl;


						if (p->getOwnerID() == player[i]->getPlayerID() && !p->getMortgaged() && deficit > 0) {

							//cout << "Seizing property ->" << p->getPropertyName() << endl;

							p->setOwnerID(-1);
							deficit -= p->getPurchasePrice();
							player[i]->addCash(p->getPurchasePrice());


							if (strcmp(p->getSpaceType(), "PRIVATE") == 0) {
								
								PrivateProperty* _p = (PrivateProperty* ) p;

								_p->setHouseCount(0);
								_p->setShopCount(0);
								_p->setHotelCount(0);

							}


						}


					}


				}

				//cout << "Deficit of Player " << i << " " << deficit << endl;

				if (deficit <= 0) {

					player[i]->setIsBankrupt(false);
					bankruptPlayers[player[i]->getPlayerID()] = false;

				}
				else {

					player[i]->setIsBankrupt(true);
					bankruptPlayers[player[i]->getPlayerID()] = true;


					for (int j = 0; j < 40; j++) {


						if (strcmp(cells[j]->getSpaceType(), "PRIVATE") == 0 || strcmp(cells[j]->getSpaceType(), "COMMERCIAL") == 0) {
						

							Property* p = (Property* ) cells[j];


							if (p->getMortgaged()) {
							
								p->setOwnerID(-1);
								p->setMortgaged(false);


								if (strcmp(p->getSpaceType(), "PRIVATE") == 0) {

									PrivateProperty* _p = (PrivateProperty*) p;

									_p->setHouseCount(0);
									_p->setShopCount(0);
									_p->setHotelCount(0);

								}


							}


						
						}


					}


				}


			}

		}
		else {

			player[i]->setIsBankrupt(false);
			bankruptPlayers[player[i]->getPlayerID()] = false;

		}



	}


	int playersLeft = 0;

	for (int i = 0; i < board.getPlayerCount(); i++) {

		playersLeft += !bankruptPlayers[i];

	}

	gameWon = (playersLeft == 1);

	if (gameWon) cout << "Game Won!" << endl;


}


bool Monopoly::getGameWon() { return gameWon; }


void Monopoly::mortgageProperty(int cellNum) {

	Property* p = (Property *)board.getCells()[cellNum];
	
	int propertyValue = 0;

	if (strcmp(p->getSpaceType(), "PRIVATE") == 0) {

		propertyValue = p->getPurchasePrice() +
			(p->getHouseCount() * 100)
			+ (p->getShopCount() * 300)
			+ (p->getHotelCount() * 1000)
			+ (p->getHasWifi() * 30)
			+ (p->getHasElectricity() * 50)
			+ (p->getHasGas() * 50);

	}
	else {

		propertyValue = p->getPurchasePrice();

	}

	if (p->getOwnerID() != -1 && !p->getMortgaged()) {
	
		p->setMortgaged(true);
		board.getPlayerByID(p->getOwnerID())->addCash(propertyValue / 2);

	}
	else if (p->getOwnerID() != -1 && p->getMortgaged()) {
	
		if (board.getPlayerByID(p->getOwnerID())->getCash() >= ((propertyValue / 2) + (0.2 * propertyValue))) {
		
			p->setMortgaged(false);
			board.getPlayerByID(p->getOwnerID())->deductCash((propertyValue / 2.0) + (0.2f * propertyValue));

		}
		else {

			sf::RenderWindow prompt(sf::VideoMode(280, 90), "NOT ENOUGH CASH!", sf::Style::Titlebar);

			sf::Font stdFont;
			stdFont.loadFromFile("fonts/Montserrat-Black.ttf");

			sf::Text nec;
			nec.setString("NOT ENOUGH CASH!\nYou need 20% more than\noriginal price to unmortgage.");
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

	}


}


void Monopoly::saveGame() {


	sf::RenderWindow window(sf::VideoMode(265, 166), "SAVE GAME", sf::Style::Titlebar);

	sf::Font stdFont;
	stdFont.loadFromFile("fonts/Montserrat-Black.ttf");

	char saveName[100];
	saveName[0] = '\0';

	sf::Text saveGameTitle;
	saveGameTitle.setString("ENTER NAME: ");
	saveGameTitle.setFont(stdFont);
	saveGameTitle.setFillColor(sf::Color(26, 188, 156));
	saveGameTitle.setCharacterSize(18);
	saveGameTitle.setPosition(15.0f, 15.0f);

	sf::Text name;
	name.setString("> ");
	name.setFont(stdFont);
	name.setFillColor(sf::Color(26, 188, 156));
	name.setCharacterSize(18);
	name.setPosition(15.0f, 35.0f);

	sf::Text nameText;
	nameText.setString(saveName);
	nameText.setFont(stdFont);
	nameText.setFillColor(sf::Color(44, 62, 80));
	nameText.setCharacterSize(18);
	nameText.setPosition(35.0f, 35.0f);

	sf::String svName;

	int nameLen = 0;


	while (window.isOpen()) {
	
	
		sf::Event evt;
		while (window.pollEvent(evt)) {
		
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

				window.close();

			}

			if (evt.type == sf::Event::TextEntered) {

				if (evt.text.unicode == 13) {
					
					nameLen++;
					window.close();

					for (int i = 0; i < nameLen; i++)
						saveName[i] = svName.getData()[i];

				}
				else if (evt.text.unicode >= 97 && evt.text.unicode < 128) {
					svName += evt.text.unicode;
					nameText.setString(svName);

					nameLen++;
				}

			}


		}

		window.clear(sf::Color(236, 240, 241));
		
		window.draw(saveGameTitle);
		window.draw(name);
		window.draw(nameText);

		window.display();


	}



	char pDataFileName[100];
	pDataFileName[0] = '\0';
	strcat(pDataFileName, "savedGames/");
	strcat(pDataFileName, saveName);
	strcat(pDataFileName, "_PropertyData.monopoly");

	ofstream fout;
	fout.open(pDataFileName);


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



	for (int i = 0; i < 20; i++) {

		PrivateProperty* p = (PrivateProperty*)board.getCells()[privatePropertySpaces[i]];

		fout <<
			p->getPropertyID() << " " <<
			p->getOwnerID() << " " <<
			p->getHouseCount() << " " <<
			p->getShopCount() << " " <<
			p->getHotelCount() << " " <<
			p->getMortgaged() << " " <<
			p->getHasWifi() << " " <<
			p->getHasElectricity() << " " <<
			p->getHasGas() << endl;


	}
	
	
	for (int i = 0; i < 8; i++) {

		CommercialProperty* p = (CommercialProperty *) board.getCells()[commercialPropertySpaces[i]];

		fout <<
			i << " " <<
			p->getOwnerID() << " " <<
			p->getMortgaged() << endl;


	}


	fout.close();


	char playerDataFileName[100];
	playerDataFileName[0] = '\0';
	strcat(playerDataFileName, "savedGames/");
	strcat(playerDataFileName, saveName);
	strcat(playerDataFileName, "_PlayerData.monopoly");

	fout.open(playerDataFileName);


	fout << board.getPlayerCount() << endl;


	for (int i = 0; i < board.getPlayerCount(); i++) {


		Player* p = board.getPlayerByID(i);

		fout <<
			p->getPlayerID() << " " <<
			p->getCash() << " " <<
			p->getIsInJail() << " " <<
			p->getIsInJailCount() << " " <<
			p->getHasJailRescueCard() << " " <<
			p->getIsRenting() << " " <<
			p->getPlayerPosition() << " " <<
			p->getIsBankrupt() << endl;


	}


	fout.close();

	char boardDataFileName[100];
	boardDataFileName[0] = '\0';
	strcat(boardDataFileName, "savedGames/");
	strcat(boardDataFileName, saveName);
	strcat(boardDataFileName, "_BoardData.monopoly");


	fout.open(boardDataFileName);



	fout <<
		board.getPreviousTurn() << " " <<
		board.getTurn() << " " <<
		board.getDRollCount() << " " <<
		board.getCurrRollAmount() << " " << endl;
		
		for (int i = 0; i < board.getPlayerCount(); i++) {
			
			fout << board.getPrevTurns()[i][0] << " " << board.getPrevTurns()[i][1] << endl;

		}



	fout.close();


	ifstream savedGamesList("savedGames/savedGamesList.txt");

	int listSize = 0;
	savedGamesList >> listSize;

	if (listSize == 0) {

		
		savedGamesList.close();

		ofstream fout("savedGames/savedGamesList.txt");


		fout << "1" << endl;
		fout << saveName << endl;


		fout.close();


	}
	else {

		savedGamesList.ignore();

		char** list = new char* [listSize];

		bool duplicate = false;

		for (int i = 0; i < listSize; i++) {

			char temp[100];
			savedGamesList >> temp;

			list[i] = new char[strlen(temp) + 1];

			strcpy(list[i], temp);

			if (strcmp(list[i], saveName) == 0) duplicate = true;


		}

		if (!duplicate) {
		
			savedGamesList.close();

			ofstream fout("savedGames/savedGamesList.txt");


			fout << listSize + 1 << endl;

			for (int i = 0; i < listSize; i++)
				fout << list[i] << endl;

			fout << saveName << endl;


			fout.close();

		}
		else {

			savedGamesList.close();

		}



	}

}

void Monopoly::loadGame(char* saveName = nullptr) {

	if (saveName == nullptr) {

		sf::RenderWindow window(sf::VideoMode(265, 166), "LOAD GAME", sf::Style::Titlebar);

		sf::Font stdFont;
		stdFont.loadFromFile("fonts/Montserrat-Black.ttf");

		saveName = new char[100];
		saveName[0] = '\0';

		sf::Text saveGameTitle;
		saveGameTitle.setString("ENTER NAME: ");
		saveGameTitle.setFont(stdFont);
		saveGameTitle.setFillColor(sf::Color(26, 188, 156));
		saveGameTitle.setCharacterSize(18);
		saveGameTitle.setPosition(15.0f, 15.0f);

		sf::Text name;
		name.setString("> ");
		name.setFont(stdFont);
		name.setFillColor(sf::Color(26, 188, 156));
		name.setCharacterSize(18);
		name.setPosition(15.0f, 35.0f);

		sf::Text nameText;
		nameText.setString(saveName);
		nameText.setFont(stdFont);
		nameText.setFillColor(sf::Color(44, 62, 80));
		nameText.setCharacterSize(18);
		nameText.setPosition(35.0f, 35.0f);

		sf::String svName;

		int nameLen = 0;


		while (window.isOpen()) {


			sf::Event evt;
			while (window.pollEvent(evt)) {

				if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

					window.close();

				}

				if (evt.type == sf::Event::TextEntered) {

					if (evt.text.unicode == 13) {

						nameLen++;
						window.close();

						for (int i = 0; i < nameLen; i++)
							saveName[i] = svName.getData()[i];

					}
					else if (evt.text.unicode >= 97 && evt.text.unicode < 128) {
						svName += evt.text.unicode;
						nameText.setString(svName);

						nameLen++;
					}

				}


			}

			window.clear(sf::Color(236, 240, 241));

			window.draw(saveGameTitle);
			window.draw(name);
			window.draw(nameText);

			window.display();


		}
	}


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


	char pDataFileName[100];
	pDataFileName[0] = '\0';
	strcat(pDataFileName, "savedGames/");
	strcat(pDataFileName, saveName);
	strcat(pDataFileName, "_PropertyData.monopoly");

	ifstream fin(pDataFileName);

	for (int i = 0; i < 20; i++) {

		int tInt;
		bool tBool;
		
		fin >> tInt;

		PrivateProperty* p = (PrivateProperty*) board.getCells()[privatePropertySpaces[tInt]];

		fin >> tInt;

		p->setOwnerID(tInt);

		fin >> tInt; 

		p->setHouseCount(tInt);

		fin >> tInt;

		p->setShopCount(tInt);

		fin >> tInt;

		p->setHotelCount(tInt);

		fin >> tBool;

		p->setMortgaged(tBool);

		fin >> tBool;

		p->setHasWifi(tBool);

		fin >> tBool;

		p->setHasElectricity(tBool);

		fin >> tBool;

		p->setHasGas(tBool);

		fin.ignore();

	}

	for (int i = 0; i < 8; i++) {

		int tInt;
		bool tBool;

		fin >> tInt;

		CommercialProperty* p = (CommercialProperty*)board.getCells()[commercialPropertySpaces[tInt]];

		fin >> tInt;

		p->setOwnerID(tInt);

		fin >> tBool;

		p->setMortgaged(tBool);

		fin.ignore();

	}

	fin.close();



	char playerDataFileName[100];
	playerDataFileName[0] = '\0';
	strcat(playerDataFileName, "savedGames/");
	strcat(playerDataFileName, saveName);
	strcat(playerDataFileName, "_PlayerData.monopoly");

	fin.open(playerDataFileName);


	int tP;
	fin >> tP;
	fin.ignore();

	setTotalPlayers(tP);

	for (int i = 0; i < tP; i++) {

		Player* p = board.getPlayerByID(i);

		int tInt;
		bool tBool;

		fin >> tInt;
		p->setPlayerID(tInt);

		fin >> tInt;
		p->setCash(tInt);

		fin >> tBool;
		p->setIsInJail(tBool);

		fin >> tInt;
		p->setIsInJailCount(tInt);

		fin >> tInt;
		p->setHasJailRescueCard(tInt);

		fin >> tInt;
		p->setIsRenting(tInt);

		fin >> tInt;
		p->setPlayerPosition(tInt);

		fin >> tBool;
		p->setIsBankrupt(tBool);

		fin.ignore();


	}


	fin.close();

	char boardDataFileName[100];
	boardDataFileName[0] = '\0';
	strcat(boardDataFileName, "savedGames/");
	strcat(boardDataFileName, saveName);
	strcat(boardDataFileName, "_BoardData.monopoly");


	fin.open(boardDataFileName);

	int temp;

	fin >> temp;
	board.setPreviousTurn(temp);

	fin >> temp;
	board.setTurn(temp);

	fin >> temp;
	board.setDRollCount(temp);

	fin >> temp;
	board.setCurrRollAmount(temp);

	for (int i = 0; i < tP; i++) {

		fin >> temp;
		board.getPrevTurns()[i][0] = temp;
		
		fin >> temp;
		board.getPrevTurns()[i][1] = temp;

		fin.ignore();

	}


}

Monopoly:: ~Monopoly() {

	if (bCoord) delete[] bCoord;
	if (playerPosition) delete[] playerPosition;
	if (bankruptPlayers) delete[] bankruptPlayers;

}