#pragma once
class PlayerPiece
{


public:
	PlayerPiece(char symbol, bool isKing);


	bool get_isKing();
	void set_isKing(bool isKing);
	char get_Symbol();
	void set_Symbol(char symbol);


private:


	char symbol;
	bool isKing = false;
};

