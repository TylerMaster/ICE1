#include "PlayerPiece.h"

#include <iostream>

using namespace std;

PlayerPiece::PlayerPiece(char symbol, bool isKing) {

	this->symbol = symbol;
	this->isKing = isKing;
}

bool PlayerPiece::get_isKing() {

	return isKing;
}
void PlayerPiece::set_isKing(bool isKing) {

	this->isKing = isKing;
}

char PlayerPiece::get_Symbol() {
	return symbol;
}
void PlayerPiece::set_Symbol(char symbol) {
	this->symbol = symbol;
}