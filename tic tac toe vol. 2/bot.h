#pragma once

#include <vector>
#include "board.h"


struct botPlay
{
	botPlay() {}
	botPlay(int Score) : score(Score){}
	int x;
	int y;
	int score;
};

class Bot {
public:
    // Initializes the Bot player
    void init(int botPlayer);
    // Performs the Bot move
    void performMove(Board& board);
private:

	botPlay getBestMove(Board& board, int player, int depth);

    int _botPlayer; ///< Index of the Bot
    int _humanPlayer; ///< Index of the player
};

void Bot::init(int botPlayer) {
	_botPlayer = botPlayer;
	_humanPlayer = _botPlayer == X_VAL ? O_VAL : X_VAL;
}

void Bot::performMove(Board& board) {

	float a;
	if (board.getSize() == 3) {
		a = 1000;
	}
	else {
		a = 1.4;
	}

	int cal_depth = a * board.getSize() - board.getMark();

	botPlay bestMove = getBestMove(board, _botPlayer, cal_depth);
	board.setVal(bestMove.x, bestMove.y, _botPlayer);
}


botPlay Bot::getBestMove(Board& board, int whoPlays, int depth)
{
	int retv = board.checkVictory();

	if (depth == 0) return 0;

	if (retv == _botPlayer) // if Bot won, return great
	{
		return botPlay(10 + depth);
	}
	else if (retv == _humanPlayer) // if human won, return low
	{
		return botPlay(-depth - 10);
	}
	else if (retv == TIE_VAL) // if nobody won, return 0
	{
		return botPlay(0);
	}

	std::vector<botPlay> moves;

	// loop through board
	for (int y = 0; y < board.getSize(); y++)
	{
		for (int x = 0; x < board.getSize(); x++)
		{
			if (board.getVal(x, y) == NO_VAL)
			{
				botPlay move;
				move.x = x;
				move.y = y;

				board.setVal(x, y, whoPlays);

				// check if a good move
				move.score = getBestMove(board, whoPlays == _botPlayer ? _humanPlayer : _botPlayer, depth - 1).score;

				moves.push_back(move);

				board.setVal(x, y, NO_VAL);
			}
		}
	}

	// pick the best move
	int bestMove = 0;
	if (whoPlays == _botPlayer)
	{
		int bestScore = -1000;
		for (size_t i = 0; i < moves.size(); i++)
		{
			if (moves[i].score > bestScore)
			{
				bestMove = i;
				bestScore = moves[i].score;
			}
		}
	}
	else
	{
		int bestScore = 1000;
		for (size_t i = 0; i < moves.size(); i++)
		{
			if (moves[i].score < bestScore)
			{
				bestMove = i;

				bestScore = moves[i].score;
			}
		}
	}
	
	// return it
	return moves[bestMove];
}