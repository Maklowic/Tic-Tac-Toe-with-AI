#pragma once

#include <iostream>
#include "bot.h"
#include "board.h"


enum class GameState { PLAYING, EXIT };

class MainGame {

    // Initializes the game
    void init();
    // Performs a human controlled move
    void playerMove();
    // Performs an AI move
    void botMove();
    // Changes players
    void changePlayer();
    // Ends a game and prompts for quit or re-try
    void endGame(bool wasTie);

    // Member Variables
    Board _board; ///< The tic-tac-toe board
    int _currentPlayer; ///< The index of the current player
    int _botPlayer; ///< Index of the bot player
    GameState _gameState; ///< The state of the game
    Bot _bot; ///< The bot player
    bool _isMultiplayer;

public:
    // Runs the main loop
    void run();
};

void MainGame::run() {
	init();
	// Game loop
	while (_gameState != GameState::EXIT) {
		// Print out the board
		_board.print();
		// Say which player is playing
		if (_currentPlayer == X_VAL) {
			printf("  Player X's turn!\n\n");
		}
		else {
			printf("  Player O's turn!\n\n");
		}
		// Perform a move
		if (!_isMultiplayer && _currentPlayer == _botPlayer) {
			botMove();
		}
		else {
			playerMove();
		}
		// Check victory condition
		int rv = _board.checkVictory();
		if (rv != NO_VAL) {
			endGame(rv == TIE_VAL);
		}
		else {
			changePlayer();
			// printf is faster than cout
			// Add a bunch of blank space to "clear" the command prompt
			for (int i = 0; i < 3; i++) {
				printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			}
		}
	}
}

void MainGame::init() {
	_gameState = GameState::PLAYING;

	_currentPlayer = X_VAL;

	// Welcoming
	std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nWELCOME TO:" << std::endl;
	std::cout << " _____   _____    ____          _____    ___     ____         _____    ___    _____  " << std::endl;
	std::cout << "|#####| |#####|  /####|        |#####|  /###\\   /####|       |#####|  /###\\  |####/ " << std::endl;
	std::cout << "  |#|     |#|   |#|      ___     |#|   |#|_|#| |#|      ___    |#|   |#| |#| |#|__   " << std::endl;
	std::cout << "  |#|     |#|   |#|     |###|    |#|   |# _ #| |#|     |###|   |#|   |#| |#| |####|  " << std::endl;
	std::cout << "  |#|    _|#|_  |#|___           |#|   |#| |#| |#|___          |#|   |#|_|#| |#|__  " << std::endl;
	std::cout << "  |#|   |#####|  \\####|          |#|   |#| |#|  \\####|         |#|    \\###/  |####\\ \n" << std::endl;

	
	int boardsize;
	int mark_number;
	bool isValid1 = false;

	printf("How big would you like your game board to be: ");
	do {
		isValid1 = true;
		if (!(std::cin >> boardsize)) {
			std::cerr << "Error: Wrong input! Try again: ";
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			isValid1 = false;
		}
		else if (boardsize >= 3){
			bool isValid2 = false;
			printf("\n	How many marks in row, column or diagonal should leed to victory: ");
			do {
				isValid2 = true;
				if (!(std::cin >> mark_number)) {
					std::cerr << "Error: Wrong input! Try again: ";
					std::cin.clear();
					std::cin.ignore(1000, '\n');
					isValid2 = false;
				}
				else if (mark_number >= 3) {
					_board.init(boardsize,mark_number);
				}
				else {
					std::cerr << "Error: To low mark amount. \n";
					std::cout << "		Normal game should have at least 3 markers.Try again: ";
					std::cin.clear();
					std::cin.ignore(1000, '\n');
					isValid2 = false;
				}
			} while (isValid2 == false);
		}
		else {
			std::cerr << "Error: Board size need to bigger than 3! Try again: ";
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			isValid1 = false;
		}
	} while (isValid1 == false);

// Check for multiplayer
	printf("\n \nWould you like to play with other player? (y/n): ");
	char input = ' ';
	bool isValid = false;
	do {
		isValid = true;
		if (!(std::cin >> input)) {
			std::cout << "Error: Wrong input! Try again: ";
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			isValid = false;
		}
		else if (input == 'y' || input == 'Y') {
			_isMultiplayer = true;
		}
		else if (input == 'n' || input == 'N') {
			_isMultiplayer = false;
		}
		else {
			std::cout << "Error: Choose Y or N: ";
			isValid = false;
		}
	} while (isValid == false);

	if (!_isMultiplayer) {
		// Get player
		printf("\nX always starts!!!");
		printf("\nWould you like to be X or O:");
		do {
			isValid = true;
			if (!(std::cin >> input)) {
				std::cout << "Error: Wrong input! Try again: ";
				std::cin.clear();
				std::cin.ignore(1000, '\n');
				isValid = false;
			}
			else if (input == 'x' || input == 'X') {
				_botPlayer = O_VAL;
			}
			else if (input == 'o' || input == 'O') {
				_botPlayer = X_VAL;
			}
			else {
				std::cout << "Must enter X or O! Try again: ";
				isValid = false;
			}
		} while (isValid == false);
		// Init the bot player
		_bot.init(_botPlayer);
	}
	printf("\n\n");
}

void MainGame::playerMove() {
	// Get input
	bool wasValid = false;
	printf("Enter:\n");
	int x, y;
	do {
		printf("       X: ");
		while (!(std::cin >> x)) {
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			printf("Error: Wrong input! Enter X again: ");
		}
		printf("       Y: ");
		while (!(std::cin >> y)) {
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			printf("Error: Wrong input! Enter Y again: ");
		}

		if (x < 1 || y < 1 || x > _board.getSize() || y > _board.getSize()) {
			printf("Error: Incorrect X or Y! Try again: \n");
		}
		else if (_board.getVal(x - 1, y - 1) != 0) {
			printf("Error: Spot occupied! Try again: \n");
		}
		else {
			wasValid = true;
		}

	} while (!wasValid);

	// Now place the token
	_board.setVal(x - 1, y - 1, _currentPlayer);
}

void MainGame::botMove() {
	// TODO: Implement this
	_bot.performMove(_board);
}

void MainGame::changePlayer() {
	if (_currentPlayer == X_VAL) {
		_currentPlayer = O_VAL;
	}
	else {
		_currentPlayer = X_VAL;
	}
}

void MainGame::endGame(bool wasTie) {

	_board.print();

	if (wasTie) {
		printf("   _____   _____   _____  ___ \n");
		printf("  |#####| |#####| |####/ |###|\n");
		printf("    |#|     |#|   |#|__  |###|\n");
		printf("    |#|     |#|   |####|  \\#/ \n");
		printf("    |#|    _|#|_  |#|__    _  \n");
		printf("    |#|   |#####| |####\\  |#|   ");
	}
	else {
		if (_currentPlayer == X_VAL) {
			printf("   _    _       _       _   _____   __    _   _____   ___ \n");
			printf("  \\#\\ /#/      |#|     |#| |#####| |##\\  |#| |#####| |###|\n");
			printf("   \\# #/       |#|     |#|   |#|   |#\\#\\ |#| |#|___  |###|\n");
			printf("     #         |#\\  _  /#|   |#|   |#|\\#\\|#| |#####|  \\#/ \n");
			printf("   /# #\\        \\#\\/#\\/#/   _|#|_  |#| \\#\\#|  ___|#|   _  \n");
			printf("  /#/ \\#\\        \\#/ \\#/   |#####| |#|  \\##| |#####|  |#|   ");
		}
		else {
			printf("    ___         _       _   _____   __    _   _____   ___ \n");
			printf("   /###\\       |#|     |#| |#####| |##\\  |#| |#####| |###|\n");
			printf("  |#| |#|      |#|     |#|   |#|   |#\\#\\ |#| |#|___  |###|\n");
			printf("  |#| |#|      |#\\  _  /#|   |#|   |#|\\#\\|#| |#####|  \\#/ \n");
			printf("  |#|_|#|       \\#\\/#\\/#/   _|#|_  |#| \\#\\#|  ___|#|   _  \n");
			printf("   \\###/         \\#/ \\#/   |#####| |#|  \\##| |#####|  |#|   ");
		}
	}
	printf("\n\n     Enter any key to play again, or C to exit: ");

	char input;
	std::cin >> input;
	if (input == 'C' || input == 'c') {
		_gameState = GameState::EXIT;
	}
	else {
		// Re init the game
		init();
	}
}
