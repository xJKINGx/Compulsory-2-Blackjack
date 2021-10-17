#include "blackjack.h"

int main() {
	std::srand(time(0));
	while (loginLoop) { // We use a loop to simply force the user to login, they have 3 attempts
		std::cout << "Welcome to blackjack, please login to continue" << std::endl;
		if (tries >= 4) {
			std::cout << "Exiting...";
			exit(0);
		}

		std::cout << "Username: ";
		getline(std::cin, username);
		std::cout << "Password: ";
		getline(std::cin, password);
		// If the username and passwords don't match, they must try again, otherwise exit the loop and continue
		if (username == "Joachim" && password == "iloveblackjack") {
			system("cls");
			loginLoop = false;
		}
		else {
			std::cout << "Wrong entry, you have " << (3 - tries) << " tries left" << std::endl;
			tries++;
		}
	}
	// After the login, blackjack will start here

	while (gameLoop) {
		while (betLoop) { // A betLoop is needed to make sure the player gives something valid to bet
			std::cout << "Welcome to blackjack, you have $" << playerBal <<
				"," << "The house has $" << AIBal << ", how much do you want to bet? (Minimum $10): ";
			std::cin >> betAmount;
			// The player can not bet lower than the minimum bet amount, in this case it is $10
			if (betAmount < minBet) {
				betAmount = 0;
				std::cout << "Minimum $10 allowed to bet" << std::endl;
			}
			else {
				// If the bet is higher than the minimum amount, check if both the player and house has 
				// enough money to bet
				if (playerBal >= betAmount && AIBal >= betAmount) {
					betLoop = false;
				}
				else {
					std::cout << "Either the player or the house doesn't have enough money" << std::endl;
				}
			}
			clearCin(); 
			// It is important to clear cin, otherwise the player can type in a character or string
			// and mess up the code.
		}
		// We have now left the betloop while loop. Every time we start the gameLoop while loop over again
		// these two if sentences will run, and nothing else.
		// We will therefore have to check whether or not the player typed "stand" during their turn
		// because then it will be the AI's turn
		if (!playerStand) {
			playerTurn();
		}

		if (playerStand) {
			AITurn();
		}
	}

	// Once the game is complete, the user will be asked if they wish to go again
	std::cout << "Do you wish to continue? (Y/N): ";
	getline(std::cin, exitQuestion);
	if (exitQuestion == "N" || exitQuestion == "n") {
		exit(0);
	}
	else {
		// Here they wish to continue, so we refresh the variables, clear the screen and get ready for another round
		// They will not need to login again
		refreshVariables();
		system("cls");
		main();
		// Not sure if calling main within main is cursed C++, but hey it works
	}
	return 0;
}

void playerTurn() {
	bool hitOrStandLoop = true;
	playerCardValue += drawCard(); // The player will draw a card and add it to their total card value
	std::cout << "Your current card value: " << playerCardValue << std::endl;
	while (hitOrStandLoop) {
		// Here the player can hit or stand, hit gives another card, stand gives the turn to the AI
		// If they do not give a valid response, the while loop will go again
		// Here the hitOrStandLoop boolean will be set to false, so that it exits the loop and they can draw another card
		// however, the boolean will be turned to true at the start of the function so they can be asked again later
		std::cout << "Hit or stand? ";
		getline(std::cin, hitOrStand);
		if (hitOrStand == "hit" || hitOrStand == "Hit") {
			hitOrStandLoop = false;
		}
		else if (hitOrStand == "stand" || hitOrStand == "Stand") {
			playerStand = true;
			hitOrStandLoop = false;
		}
	}
}

void AITurn() {
	AICardValue += drawCard();
	std::cout << "The house's card value is now " << AICardValue << std::endl;
	// The AI draws a card and tries to figure out if they wish to draw another
	// If the AI has a 21 or the player has busted or the AI has a higher card value while the player hasn't busted
	// the AI will then initialize the endGame function to find out a winner
	if (AICardValue > playerCardValue || AICardValue == 21 || playerCardValue > 21) {
		endGame();
	}
}

int drawCard() {
	int value{};
	bool aceSelectBool = true;
	// Since there are 4 ways to get a 10, we will need to get a random number to choose between these 13 "cards"
	// The card given will be a random number in the "deck" called tempArray
	int tempArray[13] = { 1,2,3,4,5,6,7,8,9,10,10,10,10 };
	value = tempArray[std::rand() % 13];

	std::string aceSelection{};
	if (playerStand == false) {
		// First we should check if the player got an ace, so they can choose between 1 and 11
		if (value == 1) {
			// Player can choose between 1 and 11
			std::cout << "You got an ace! Which value should it have? 1 or 11? ";
			while (aceSelectBool) {
				getline(std::cin, aceSelection);
				if (aceSelection == "1") {
					value = 1;
					aceSelectBool = false;
				}
				else if (aceSelection == "11") {
					value = 11;
					aceSelectBool = false;
				}
				else {
					std::cout << "Select by typing either 1 or 11." << std::endl;
				}
			}
		}
		else {
			// If they didn't get an ace, just output the card they were dealt
			std::cout << "You got a " << value << "." << std::endl;
		}
	}
	else {
		// If it reaches this point, it means the player typed stand and it is the AI's turn
		if (value == 1) {
			// The AI got and ace and will choose either 1 or 11
			// Implementation will be a bit more difficult as we need to make it somewhat smart
			std::cout << "The house draws an ace, they choose it will be a ";
			if ((AICardValue + 11) > 21) { // If 11 would get them over 21
				value = 1; // The ace will have a value of 1
			}
			if (AICardValue + 11 == 21) { // If 11 would get them to 21
				value = 11; // The ace will have a value of 11
			}
			if (AICardValue + 1 == 21) { // If 1 would get them to 21
				value = 1; // The ace will have a value of 1
			}
			if (AICardValue + 11 < 21) { // If 11 wouldn't get them to or go beyond 21
				value == 11; // The ace will have a value of 11
			}
			std::cout << value << std::endl;
		}
		else {
			// If the AI didn't get an ace, it will output the card
			std::cout << "The house draws a " << value << std::endl;
		}
	}

	return value;
}

void endGame() {
	int ending{};
	// if ending = 1, it's a draw
	// if ending = 2, house wins
	// if ending = 3, player wins

	if ((AICardValue <= 21 || playerCardValue <= 21)) {
		// This if sentence is to check either of them are below or equal to 21
		if ((AICardValue > playerCardValue || playerCardValue > 21 || AICardValue == 21) && AICardValue <= 21) {
			// If the AI has a higher value or the player has busted or the AI has 21 and also the AI hasn't busted
			ending = 2;
		}
		if ((AICardValue < playerCardValue || AICardValue > 21 || playerCardValue == 21) && playerCardValue <= 21) {
			// If the player has a higher value or the AI has busted or the player has 21 and also the player hasn't busted
			ending = 3;
		}
		if (AICardValue == playerCardValue) {
			// If both the player and the house got the same value, it's a draw
			ending = 1;
		}
	}
	else {
		// If they are both above 21, it's a draw
		ending = 1;
	}

	// Now a switch case to process the endings, if it's not a draw, give the bet to the winner
	switch (ending) {
	case 1:
		std::cout << "It's a draw!" << std::endl;
		break;
	case 2:
		std::cout << "The house wins!" << std::endl;
		AIBal += betAmount;
		playerBal -= betAmount;
		break;
	case 3:
		std::cout << "The player wins!" << std::endl;
		AIBal -= betAmount;
		playerBal += betAmount;
		break;
	}

	// Here we check if the game is over and if anyone has won
	// If the AI has no money, the player wins. If the player has no money, the AI wins
	if (AIBal == 0) {
		std::cout << "Game over - the player wins!" << std::endl;
		exit(0);
	}
	else if (playerBal == 0) {
		std::cout << "Game over - the house wins!" << std::endl;
		exit(0);
	}
	gameLoop = false;
}

// The refreshVariables function is to setup the game for the next iteration
void refreshVariables() {
	gameLoop = true;
	betLoop = true;
	playerStand = false;
	playerCardValue = 0;
	AICardValue = 0;
}

void clearCin() {
	std::cin.clear();
	std::cin.ignore(32767, '\n');
}