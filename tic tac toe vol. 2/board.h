#pragma once

#include <vector>
#include <string>


const int TIE_VAL = -1;
// Indexes for board values
const int NO_VAL = 0;
const int X_VAL = 1;
const int O_VAL = 2;

// Game board class
class Board {

    // Adds a horizontal line to string for printing
    void addHorizontalLine(std::string& s) const;
    // Adds a horizontal axis guide line to string for printing
    void addGuide(std::string& s) const;

    std::vector<int> _board;
	int _size;
	int _markAmount;
	int _frame;

public: 
   
    // Initializes the board
    void init(int size, int mark);
    // Clears the board
    void clear();
    // Prints the board to standard output
    void print() const;

    // If no player won, returns NO_VAL. Otherwise returns X_VAL, O_VAL or TIE_VAL
    int checkVictory() const;

    // Sets value at x,y spot
    void setVal(int x, int y, int val) {
        _board[y * _size + x] = val;
    }

    // Gets value at x,y spot
    int getVal(int x, int y) const {
        return _board[y * _size + x];
    }

    int getSize() const { return _size; }

	int getMark() const { return _markAmount; }

	int getFrame() const { return _frame; }
};


// Arrays of chars for printing X, O or blank
const char glyphs_less_than_8[3][7][7] = {
	{
		{' ',' ',' ',' ',' ',' ',' '},
		{' ',' ',' ',' ',' ',' ',' '},
		{' ',' ',' ',' ',' ',' ',' '},
		{' ',' ',' ',' ',' ',' ',' '},
		{' ',' ',' ',' ',' ',' ',' '},
		{' ',' ',' ',' ',' ',' ',' '},
		{' ',' ',' ',' ',' ',' ',' '}
	},
	{
		{'  ','_','  ',' ',' ','_',' '},
		{'\\','#','\\',' ','/','#','/'},
		{' ','\\','#',' ','#','/',' '},
		{' ',' ',' ','#',' ',' ',' '},
		{' ','/','#',' ','#','\\',' '},
		{'/','#','/',' ','\\','#','\\'},
		{' ',' ',' ',' ',' ',' ',' '}
	},
	{
		{' ',' ','_','_','_',' ',' '},
		{' ','/','#','#','#','\\',' '},
		{'|','#','|',' ','|','#','|'},
		{'|','#','|',' ','|','#','|'},
		{'|','#','|','_','|','#','|'},
        {' ','\\','#','#','#','/',' '},
		{' ',' ',' ',' ',' ',' ',' '}
	}
};
const char glyphs_more[3][3][3] = {
	{
		{ ' ', ' ', ' ' },
		{ ' ', ' ', ' ' },
		{ ' ', ' ', ' ' }
	},
	{
		{ 'X', ' ', 'X' },
		{ ' ', 'X', ' ' },
		{ 'X', ' ', 'X' }
	},
	{
		{ 'O', 'O', 'O' },
		{ 'O', ' ', 'O' },
		{ 'O', 'O', 'O' }
	}
};


void Board::init(int size, int mark) {
	_size = size;
	_markAmount = mark;

	if (3 <= _size && _size <= 7) {
		_frame = 7;
	}
	else if (8 <= _size) {
		_frame = 3;
	}
	
	// Set the board size
	_board.resize(size * size);
	// Clear it
	clear();
}


void Board::clear() {
	for (size_t i = 0; i < _board.size(); i++) {
		_board[i] = NO_VAL;
	}
}

void Board::print() const {

	printf("\n");
	std::string text = "";

	// reservation of the memory
	text.reserve(_size * _size * 4);

	// Make top guide
	addGuide(text);

	// Loop through columns
	for (int y = 0; y < _size; y++) {
		// Add line
		addHorizontalLine(text);
		// Loop through glyph y
		for (int gy = 0; gy < _frame; gy++) {
			// Add number or space
			if (gy == 1) {
				text += std::to_string(_size - y);
				if ((_size - y) < 10) {
					text += "  ";
				}
				else if ((_size - y) < 100) {
					text += " ";
				}
			}
			else {
					text += "   ";
			}
			// Loop through row cells
			for (int x = 0; x < _size; x++) {
				// Loop through glyph x 
				text += "|";
				for (int gx = 0; gx < _frame; gx++) {
					if (_frame == 7) {
						text += glyphs_less_than_8[getVal(x, _size - y - 1)][gy][gx];
					}
					else if (_frame == 3){
						text += glyphs_more[getVal(x, _size - y - 1)][gy][gx];
					}
				}
			}
			text += "|";
			// Add number or newline
			if (gy == 1) {
				text += std::to_string(_size - y) + "\n";
			}
			else {
				text += "\n";
			}
		}
	}
	// Add bottom line
	addHorizontalLine(text); 
		// Make bottom guide
		addGuide(text);

	// Print the string
	printf("%s\n", text.c_str());
}

int Board::checkVictory() const {
	bool victory;
	int c;

	// Check every posible win condition
	for (int i = 0; i < _size - _markAmount + 1; i++) {

		// Check the rows
		/*
		|X|X|X|X|
		|X|X|X|X|
		| | | | |
		| | | | |
		*/
		for (int r_size = 0; r_size < _size; r_size++) {
			int x0 = i;
			int y0 = r_size;

				c = getVal(x0, y0);
				if (c != NO_VAL) {
					victory = true;
					for (int x = 0; x < _markAmount; x++) {
						if (getVal(x0 + x, y0) != c) {
							victory = false;
							break;
						}
					}
					if (victory) return c;
				}
		}


		// Check the columns
		/*
		|X|X| | |
		|X|X| | |
		|X|X| | |
		|X|X| | |
		*/
		for (int c_size = 0; c_size < _size; c_size++) {

			int x5 = c_size;
			int y5 = i;

			c = getVal(x5, y5);
			if (c != NO_VAL) {
				victory = true;
				for (int y = 0; y < _markAmount; y++) {
					if (getVal(x5, y5 + y) != c) {
						victory = false;
						break;
					}
				}
				if (victory) return c;
			}
		}

		// Check every posible diagonal


		/*
		| | | |X|
		| | |X|X|
		| |X|X|X|
		|X|X|X|X|
		*/
		int x1 = i;
		int y1 = 0;
		// IMPORTANT FOR to not check diagonal with too less space
		for (int j = _markAmount + i; j < _size + 1; j++) { 
			c = getVal(x1, y1);
			if (c != NO_VAL) {
				victory = true;
				for (int xy = 0; xy < _markAmount; xy++) {
					if (getVal(x1 + xy, y1 + xy) != c) {
						victory = false;
						break;
					}
				}
				if (victory) return c;
			}
			x1++;
			y1++;
		}
		/*
		|X|X|X|X|
		|X|X|X| |
		|X|X| | |
		|X| | | |
		*/
		int x2 = 0;
		int y2 = i;
		// IMPORTANT FOR to not check diagonal with too less space
		for (int j = _markAmount + i; j < _size + 1; j++) {
			c = getVal(x2, y2);
			if (c != NO_VAL) {
				victory = true;
				for (int xy = 0; xy < _markAmount; xy++) {
					if (getVal(x2 + xy, y2 + xy) != c) {
						victory = false;
						break;
					}
				}
				if (victory) return c;
			}
			x2++;
			y2++;
		}
		/*
		|X| | | |
		|X|X| | |
		|X|X|X| |
		|X|X|X|X|
		*/
		int x3 = _size - 1 - i;
		int y3 = 0;
		// IMPORTANT FOR to not check diagonal with too less space
		for (int j = _markAmount + i; j < _size + 1; j++) {
			c = getVal(x3, y3);
			if (c != NO_VAL) {
				victory = true;
				for (int xy = 0; xy < _markAmount; xy++) {
					if (getVal(x3 - xy, y3 + xy) != c) {
						victory = false;
						break;
					}
				}
				if (victory) return c;
			}
			x3--;
			y3++;
		}
		/*
		|X|X|X|X|
		| |X|X|X|
		| | |X|X|
		| | | |X|
		*/
		int x4 = _size - 1;
		int y4 = i;
		// IMPORTANT FOR to not check diagonal with too less space
		for (int j = _markAmount + i; j < _size + 1; j++) {
			c = getVal(x4, y4);
			if (c != NO_VAL) {
				victory = true;
				for (int xy = 0; xy < _markAmount; xy++) {
					if (getVal(x4 - xy, y4 + xy) != c) {
						victory = false;
						break;
					}
				}
				if (victory) return c;
			}
			x4--;
			y4++;
		}
	}
	// Check for tie game
	for (size_t i = 0; i < _board.size(); i++) {
		if (_board[i] == NO_VAL) return NO_VAL;
	}
	// If we get here, every spot was filled, so return tie
	return TIE_VAL;
}

void Board::addHorizontalLine(std::string& s) const {
	s += "---";

	if (3 <= _size && _size <= 7) {
		for (int x = 0; x < _size; x++) {
			s += "--------";
		}
	} 
	else if (8 <= _size) {
		for (int x = 0; x < _size; x++) {
			s += "----";
		}
	}

	s += "----\n";
}

void Board::addGuide(std::string& s) const {
	s += "   ";

	if (3 <= _size && _size <= 7) {
		for (int i = 1; i <= _size; i++) {
			s += "|   " + std::to_string(i) + "   ";
		}
	}
	else if (8 <= _size) {
		for (int i = 1; i <= _size; i++) {
			if (i < 10) {
				s += "| " + std::to_string(i) + " ";
			}
			else if (i < 100) {
				s += "| " + std::to_string(i) + "";
			}
			else {
				s += "|" + std::to_string(i) + "";
			}
			
		}
	}

	s += "|\n";
}
