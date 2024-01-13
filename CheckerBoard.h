#pragma once
#include <iostream>
#include "PlayerPiece.h"
#include "PlayerPiece2.h"


class CheckerBoard
{

public:

	CheckerBoard();
	void playGame();	


private:

	void printBoard();
	void initBoard();
	
	char player1;
	char player2;


	
	char board[8][8];
	int getXCordSelect();
	int getYCordSelect();
	int getXCordMove();
	int getYCordMove();

	bool checkHit(int x, int y, int x1, int y1, int i, int j, int &setX, int &setY, char currentPlayer, char currentPlayerKing, char player1King, char player2King, bool &multiHit);

	char pieceSelector(int x, int y, char currentPlayer, char currentPlayerKing);

	bool pieceMover(int x, int y, int x1, int y1, char currentPlayer, char currentPlayerKing);

	bool checkWinGameO(int count);
	bool checkWinGameX(int count);

	bool hitMultiplierTopRightX(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayerKing);

	bool hitMultiplierTopLeftX(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayerKing);

	bool hitMultiplierBottomRightX(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayerKing);

	bool hitMultiplierBottomLeftX(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayerKing);

	bool hitMultiplierTopRightO(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayerKing);

	bool hitMultiplierTopLeftO(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayerKing);

	bool hitMultiplierBottomRightO(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayerKing);

	bool hitMultiplierBottomLeftO(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayerKing);

	// NORMALS

	bool hitMultiplierTopRightXNormal(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayer, bool &multiHit);

	bool hitMultiplierTopLeftXNormal(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayer, bool &multiHit);

	bool hitMultiplierBottomRightXNormal(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayer, bool &multiHit);

	bool hitMultiplierBottomLeftXNormal(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayer, bool &multiHit);

	bool hitMultiplierTopRightONormal(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayer, bool &multiHit);

	bool hitMultiplierTopLeftONormal(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayer, bool &multiHit);

	bool hitMultiplierBottomRightONormal(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayer, bool &multiHit);

	bool hitMultiplierBottomLeftONormal(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayer, bool &multiHit);




	bool kingMaker(int x1, int y1, int &setX, int &setY, PlayerPiece xPiece1, PlayerPiece xPiece2, PlayerPiece xPiece3,
		PlayerPiece xPiece4, PlayerPiece xPiece5, PlayerPiece xPiece6, PlayerPiece xPiece7,
		PlayerPiece xPiece8, PlayerPiece xPiece9, PlayerPiece xPiece10, PlayerPiece xPiece11,
		PlayerPiece xPiece12, PlayerPiece2 oPiece1, PlayerPiece2 oPiece2, PlayerPiece2 oPiece3,
		PlayerPiece2 oPiece4, PlayerPiece2 oPiece5, PlayerPiece2 oPiece6, PlayerPiece2 oPiece7,
		PlayerPiece2 oPiece8, PlayerPiece2 oPiece9, PlayerPiece2 oPiece10, PlayerPiece2 oPiece11,
		PlayerPiece2 oPiece12);

	void turnChanger(char &currentPlayer, char &currentPlayerKing, char &player1, char &player1King, char &player2, char &player2King);

	void findCurrentPlayerKing(int x, int y, int x1, int y1, int &setX, int &setY, char &currentPlayerKing);

	void findCurrentPlayer(int x, int y, int x1, int y1, int &setX, int &setY, char &currentPlayer);
	
	void playerPieces(PlayerPiece xPiece1, PlayerPiece xPiece2, PlayerPiece xPiece3,
		PlayerPiece xPiece4, PlayerPiece xPiece5, PlayerPiece xPiece6, PlayerPiece xPiece7,
		PlayerPiece xPiece8, PlayerPiece xPiece9, PlayerPiece xPiece10, PlayerPiece xPiece11,
		PlayerPiece xPiece12);

	void playerPieces2(PlayerPiece2 oPiece1, PlayerPiece2 oPiece2, PlayerPiece2 oPiece3,
		PlayerPiece2 oPiece4, PlayerPiece2 oPiece5, PlayerPiece2 oPiece6, PlayerPiece2 oPiece7,
		PlayerPiece2 oPiece8, PlayerPiece2 oPiece9, PlayerPiece2 oPiece10, PlayerPiece2 oPiece11,
		PlayerPiece2 oPiece12);
};

