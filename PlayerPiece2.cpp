#include "PlayerPiece2.h"

#include <iostream>

using namespace std;

PlayerPiece2::PlayerPiece2(char symbol, bool isKing) {

	this->symbol = symbol;
	this->isKing = isKing;
}

bool PlayerPiece2::get_isKing() {

	return isKing;
}
void PlayerPiece2::set_isKing(bool isKing) {

	this->isKing = isKing;
}

char PlayerPiece2::get_Symbol() {
	return symbol;
}
void PlayerPiece2::set_Symbol(char symbol) {
	this->symbol = symbol;
}