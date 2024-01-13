
#include "CheckerBoard.h"
#include "PlayerPiece.h"
#include "PlayerPiece2.h"

#include <iostream>

using namespace std;

CheckerBoard::CheckerBoard() {

}

void CheckerBoard::playGame() {

	bool gameIsDone = false;
	bool moveLoop = false;
	bool turnChangerFlag = false;

	int x;
	int y;
	int x1;
	int y1;

	int setX;
	int setY;

	int count = 0;

	bool multiHit = false;

	// multipliers
	int i = 1;
	int j = 2;
	
	char player1 = 'X';
	char player2 = 'O';
	char player1King = 'M';
	char player2King = 'W';

	PlayerPiece xPiece1('X', false);
	PlayerPiece xPiece2('X', false);
	PlayerPiece xPiece3('X', false);
	PlayerPiece xPiece4('X', false);
	PlayerPiece xPiece5('X', false);
	PlayerPiece xPiece6('X', false);
	PlayerPiece xPiece7('X', false);
	PlayerPiece xPiece8('X', false);
	PlayerPiece xPiece9('X', false);
	PlayerPiece xPiece10('X', true);
	PlayerPiece xPiece11('X', false);
	PlayerPiece xPiece12('X', false);

	PlayerPiece2 oPiece1('O', false);
	PlayerPiece2 oPiece2('O', false);
	PlayerPiece2 oPiece3('O', true);
	PlayerPiece2 oPiece4('O', false);
	PlayerPiece2 oPiece5('O', false);
	PlayerPiece2 oPiece6('O', false);
	PlayerPiece2 oPiece7('O', false);
	PlayerPiece2 oPiece8('O', false);
	PlayerPiece2 oPiece9('O', false);
	PlayerPiece2 oPiece10('O', false);
	PlayerPiece2 oPiece11('O', false);
	PlayerPiece2 oPiece12('O', false);

	char currentPlayer = player1;
	char currentPlayerKing = player1King;
	char selectedPiece;
	bool cancelSelect = true;

	initBoard();

	playerPieces(xPiece1, xPiece2, xPiece3, xPiece4, xPiece5, xPiece6,
		xPiece7, xPiece8, xPiece9, xPiece10, xPiece11, xPiece12);

	playerPieces2(oPiece1, oPiece2, oPiece3, oPiece4, oPiece5, oPiece6,
		oPiece7, oPiece8, oPiece9, oPiece10, oPiece11, oPiece12);

	while (gameIsDone == false) {
		printBoard();


		while (moveLoop == false) {

			x = getXCordSelect();
			y = getYCordSelect();


			selectedPiece = pieceSelector(x, y, currentPlayer, currentPlayerKing);

			while ((selectedPiece != currentPlayer)
				&& (selectedPiece != currentPlayerKing)) {
				x = getXCordSelect();
				y = getYCordSelect();
				selectedPiece = pieceSelector(x, y, currentPlayer, currentPlayerKing);

			}


			x1 = getXCordMove();

			y1 = getYCordMove();



			if (checkHit(x, y, x1, y1, i, j, setX, setY, currentPlayer, currentPlayerKing, player1King, player2King, multiHit) == true) {

				break;
			}



			if (pieceMover(x, y, x1, y1, currentPlayer, currentPlayerKing) == false) {
				cout << "Invalid move spot, select another" << endl;

			}
			else {

				break;
			}


		}

		if (kingMaker(x1, y1, setX, setY, xPiece1, xPiece2, xPiece3, xPiece4, xPiece5,
			xPiece6, xPiece7, xPiece8, xPiece9, xPiece10, xPiece11, xPiece12,
			oPiece1, oPiece2, oPiece3, oPiece4, oPiece5, oPiece6, oPiece7,
			oPiece8, oPiece9, oPiece10, oPiece11, oPiece12) == true) {
			cout << "A King has been made!" << endl;

		}

		turnChanger(currentPlayer, currentPlayerKing, player1, player1King, player2, player2King);


		if ((checkWinGameO(count) == true) || (checkWinGameX(count) == true)) {
			break;
		}
		

	}

}


void CheckerBoard::printBoard() {


	cout << "  1 2 3 4 5 6 7 8\n";
	cout << "  ----------------\n";
	for (int i = 0; i < 8; i++) {
		cout << i + 1 << "|" << board[i][0] << " " << board[i][1] << " " << board[i][2] << " " << board[i][3]
			<< " " << board[i][4] << " " << board[i][5] << " " << board[i][6] << " "
			<< board[i][7] << " " << endl;

	}
	cout << "  ----------------\n";

}

void CheckerBoard::initBoard() {

	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if ((x + y) % 2 == 0) {
				board[y][x] = '*';
			}
			else {
				board[y][x] = '#';
			}

		}

	}

}

bool CheckerBoard::checkWinGameO(int count) {
	
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (board[y][x] == 'X' || board[y][x] == 'M') {
				count++;
		  }

		}

	}

	if (count == 0) {
		cout <<  "Player: O WINS!!!!" << endl;
		return true;
	}

}

bool CheckerBoard::checkWinGameX(int count) {
	

	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (board[y][x] == 'O' || board[y][x] == 'W') {
				count++;
			}

		}

	}

	if (count == 0) {
		cout << "Player: X WINS!!!!" << endl;
		return true;
	}

}



void CheckerBoard::playerPieces(PlayerPiece xPiece1, PlayerPiece xPiece2, PlayerPiece xPiece3,
	PlayerPiece xPiece4, PlayerPiece xPiece5, PlayerPiece xPiece6, PlayerPiece xPiece7,
	PlayerPiece xPiece8, PlayerPiece xPiece9, PlayerPiece xPiece10, PlayerPiece xPiece11,
	PlayerPiece xPiece12) {

	board[0][0] = xPiece1.get_Symbol();

	board[0][2] = xPiece2.get_Symbol();

	board[0][4] = xPiece3.get_Symbol();

	board[0][6] = xPiece4.get_Symbol();

	board[1][1] = xPiece5.get_Symbol();

	board[1][3] = xPiece6.get_Symbol();

	board[1][5] = xPiece7.get_Symbol();

	board[1][7] = xPiece8.get_Symbol();

	board[2][0] = xPiece9.get_Symbol();

	board[2][2] = xPiece10.get_Symbol();

	board[2][4] = xPiece11.get_Symbol();

	board[2][6] = xPiece12.get_Symbol();



}

void CheckerBoard::playerPieces2(PlayerPiece2 oPiece1, PlayerPiece2 oPiece2, PlayerPiece2 oPiece3,
	PlayerPiece2 oPiece4, PlayerPiece2 oPiece5, PlayerPiece2 oPiece6, PlayerPiece2 oPiece7,
	PlayerPiece2 oPiece8, PlayerPiece2 oPiece9, PlayerPiece2 oPiece10, PlayerPiece2 oPiece11,
	PlayerPiece2 oPiece12) {

	board[5][1] = oPiece1.get_Symbol();

	board[5][3] = oPiece2.get_Symbol();

	board[5][5] = oPiece3.get_Symbol();

	board[5][7] = oPiece4.get_Symbol();

	board[6][0] = oPiece5.get_Symbol();

	board[6][2] = oPiece6.get_Symbol();

	board[6][4] = oPiece7.get_Symbol();

	board[6][6] = oPiece8.get_Symbol();

	board[7][1] = oPiece9.get_Symbol();

	board[7][3] = oPiece10.get_Symbol();

	board[7][5] = oPiece11.get_Symbol();

	board[7][7] = oPiece12.get_Symbol();

}


int CheckerBoard::getXCordSelect() {
	int x = 0;
	bool badInput = true;
	while (badInput == true) {
		cout << "Select piece X coordinate." << endl;
		cin >> x;


		if ((x < 1) || (x > 8)) {
			cout << "Sorry bad input" << endl;
			badInput = true;
		}
		else {
			badInput = false;
			return x - 1;
		}
	}


}

int CheckerBoard::getYCordSelect() {
	int y = 0;
	bool badInput = true;
	while (badInput == true) {
		cout << "Select piece Y coordinate." << endl;
		cin >> y;

		if ((y < 1) || (y > 8)) {
			cout << "Sorry bad input" << endl;
			badInput = true;
		}
		else {
			badInput = false;
			return y - 1;
		}
	}


}

int CheckerBoard::getXCordMove() {
	int x = 0;
	bool badInput = true;
	while (badInput == true) {
		cout << "Select move spot X coordinate. To undo selection, select an invalid move coordinate." << endl;
		cin >> x;



		if ((x < 1) || (x > 8)) {
			cout << "Sorry bad input" << endl;
			badInput = true;
		}
		else {
			badInput = false;
			return x - 1;
		}
	}



}


int CheckerBoard::getYCordMove() {
	int y = 0;
	bool badInput = true;
	while (badInput == true) {
		cout << "Select move spot Y coordinate. To undo selection, select an invalid move coordinate." << endl;
		cin >> y;



		if ((y < 1) || (y > 8)) {
			cout << "Sorry bad input" << endl;
			badInput = true;
		}
		else {
			badInput = false;
			return y - 1;
		}
	}



}

char CheckerBoard::pieceSelector(int x, int y, char currentPlayer, char currentPlayerKing) {


	if (board[y][x] == '#') {
		cout << "spot out of bounds" << endl;

	}
	if (((board[y][x] != currentPlayer) && (board[y][x] != currentPlayerKing)) && (board[y][x] != '#') && (board[y][x] != '*')) {
		cout << "That's the other players piece, select your own" << endl;

	}

	if (board[y][x] == '*') {

		cout << "spot selection is empty!" << endl;

	}


	if ((board[y][x] == currentPlayer) || (board[y][x] == currentPlayerKing)) {
		cout << "player " << currentPlayer << "'s piece has been selected" << endl;
		return board[y][x];
	}


}

bool CheckerBoard::pieceMover(int x, int y, int x1, int y1, char currentPlayer, char currentPlayerKing) {



	if (((y1 > (y + 1)) || (y >= y1)) && ((currentPlayer == 'X') && (board[y][x] == 'X'))) {
		return false;
	}

	if ((y1 > (y + 1)) && ((currentPlayerKing == 'M') && (board[y][x] == 'M'))) {
		return false;
	}

	if (((y1 < (y - 1)) || (y <= y1)) && ((currentPlayer == 'O') && (board[y][x] == 'O'))) {
		return false;
	}
	if ((y1 < (y - 1)) && ((currentPlayerKing == 'W') && (board[y][x] == 'W'))) {
		return false;
	}


	if ((board[y][x] == 'M') && (board[y1][x1] == '*')) {
		board[y1][x1] = currentPlayerKing;
		board[y][x] = '*';
		return true;
	}

	
	if ((board[y][x] == 'W') && (board[y1][x1] == '*')) {
		board[y1][x1] = currentPlayerKing;
		board[y][x] = '*';
		return true;
	}

	if (board[y1][x1] != '*') {
		return false;
	}
	else {
		board[y1][x1] = currentPlayer;
		board[y][x] = '*';
		
		return true;
	}
}

void CheckerBoard::turnChanger(char &currentPlayer, char &currentPlayerKing, char &player1, char &player1King, char &player2, char &player2King) {

	if ((currentPlayer == player1) || (currentPlayerKing == player1King)) {
		currentPlayer = player2;
		currentPlayerKing = player2King;

	}
	else {
		currentPlayer = player1;
		currentPlayerKing = player1King;
	}



}

void CheckerBoard::findCurrentPlayerKing(int x, int y, int x1, int y1, int &setX, int &setY, char &currentPlayerKing) {

	board[setY][setX] = currentPlayerKing;
	if (board[setY][setX] == currentPlayerKing) {

		board[setY][setX] = '*';
		


	}
}

void CheckerBoard::findCurrentPlayer(int x, int y, int x1, int y1, int &setX, int &setY, char &currentPlayer) {



			board[setY][setX] = currentPlayer;
			if (board[setY][setX] == currentPlayer) {
				
				board[setY][setX] = '*';
			
				
			}
	
}

bool CheckerBoard::hitMultiplierTopRightX(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayerKing) {


	do {
		if (((board[setY - i][i + setX] == 'O') || (board[setY - i][i + setX] == 'W')) && (board[setY - j][j + setX] == '*')) {
			board[setY - i][i + setX] = '*';
			board[(setY)][(setX)] = '*';
			board[setY - j][j + setX] = currentPlayerKing;
			cout << "A piece has been killed!" << endl;

			setY = (setY - j);
			setX = (j + setX);

			i = i + 2;
			j = j + 2;

			return true;
		}
		else {

			return false;
		}

	} while (((board[setY - i][i + setX] == 'O') || (board[setY - i][i + setX] == 'W')));


}

bool CheckerBoard::hitMultiplierTopLeftX(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayerKing) {


	do {
		if (((board[setY - i][setX - i] == 'O') || (board[setY - i][setX - i] == 'W')) && (board[setY - j][setX - j] == '*')) {
			board[setY - i][setX - i] = '*';
			board[(setY)][(setX)] = '*';
			board[setY - j][setX - j] = currentPlayerKing;
			cout << "A piece has been killed!" << endl;

			setY = (setY - j);
			setX = (setX - j);

			i = i + 2;
			j = j + 2;

			return true;
		}
		else {

			return false;
		}

	} while (((board[setY - i][setX - i] == 'O') || (board[setY - i][setX - i] == 'W')));




}

bool CheckerBoard::hitMultiplierBottomRightX(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayerKing) {


	do {
		if (((board[i + setY][i + setX] == 'O') || (board[i + setY][i + setX] == 'W')) && (board[j + setY][j + setX] == '*')) {
			board[i + setY][i + setX] = '*';
			board[(setY)][(setX)] = '*';
			board[j + setY][j + setX] = currentPlayerKing;
			cout << "A piece has been killed!" << endl;

			setY = (j + setY);
			setX = (j + setX);

			i = i + 2;
			j = j + 2;

			return true;
		}
		else {

			return false;
		}

	} while (((board[i + setY][i + setX] == 'O') || (board[i + setY][i + setX] == 'W')));



	

}

bool CheckerBoard::hitMultiplierBottomLeftX(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayerKing) {


	do {
		if (((board[i + setY][setX - i] == 'O') || (board[i + setY][setX - i] == 'W')) && (board[j + setY][setX - j] == '*')) {
			board[i + setY][setX - i] = '*';
			board[(setY)][(setX)] = '*';
			board[j + setY][setX - j] = currentPlayerKing;
			cout << "A piece has been killed!" << endl;

			setY = (j + setY);
			setX = (setX - j);

			i = i + 2;
			j = j + 2;

			return true;
		}
		else {


			return false;
		}

	} while (((board[i + setY][setX - i] == 'O') || (board[i + setY][setX - i] == 'W')));


}

bool CheckerBoard::hitMultiplierTopRightO(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayerKing) {

	do {
		if (((board[setY - i][i + setX] == 'X') || (board[setY - i][i + setX] == 'M')) && (board[setY - j][j + setX] == '*')) {
			board[setY - i][i + setX] = '*';
			board[(setY)][(setX)] = '*';
			board[setY - j][j + setX] = currentPlayerKing;
			cout << "A piece has been killed!" << endl;

			setY = (setY - j);
			setX = (j + setX);

			i = i + 2;
			j = j + 2;

			return true;
		}
		else {

			return false;
		}

	} while (((board[i + setY][setX - i] == 'X') || (board[i + setY][setX - i] == 'M')));
}

bool CheckerBoard::hitMultiplierTopLeftO(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayerKing) {

	do {
		if (((board[setY - i][setX - i] == 'X') || (board[setY - i][setX - i] == 'M')) && (board[setY - j][setX - j] == '*')) {
			board[setY - i][setX - i] = '*';
			board[(setY)][(setX)] = '*';
			board[setY - j][setX - j] = currentPlayerKing;
			cout << "A piece has been killed!" << endl;

			setY = (setY - j);
			setX = (setX - j);

			i = i + 2;
			j = j + 2;

			return true;
		}
		else {

			return false;
		}

	} while (((board[i + setY][setX - i] == 'X') || (board[i + setY][setX - i] == 'M')));

}

bool CheckerBoard::hitMultiplierBottomRightO(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayerKing) {

	do {
		if (((board[i + setY][i + setX] == 'X') || (board[i + setY][i + setX] == 'M')) && (board[j + setY][j + setX] == '*')) {
			board[i + setY][i + setX] = '*';
			board[(setY)][(setX)] = '*';
			board[j + setY][j + setX] = currentPlayerKing;
			cout << "A piece has been killed!" << endl;

			setY = (j + setY);
			setX = (j + setX);

			i = i + 2;
			j = j + 2;

			return true;
		}
		else {

			return false;
		}

	} while (((board[i + setY][setX - i] == 'X') || (board[i + setY][setX - i] == 'M')));

}

bool CheckerBoard::hitMultiplierBottomLeftO(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayerKing) {
	do {
		if (((board[i + setY][setX - i] == 'X') || (board[i + setY][setX - i] == 'M')) && (board[j + setY][setX - j] == '*')) {
			board[i + setY][setX - i] = '*';
			board[(setY)][(setX)] = '*';
			board[j + setY][setX - j] = currentPlayerKing;
			cout << "A piece has been killed!" << endl;

			setY = (j + setY);
			setX = (setX - j);

			i = i + 2;
			j = j + 2;

			return true;
		}
		else {


			return false;
		}

	} while (((board[i + setY][setX - i] == 'X') || (board[i + setY][setX - i] == 'M')));
}

// MULTIPLE HITS FOR NORMAL PEOPLE 

bool CheckerBoard::hitMultiplierTopRightXNormal(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayer, bool &multiHit) {


	do {
		if (((board[setY - i][i + setX] == 'O') || (board[setY - i][i + setX] == 'W')) && (board[setY - j][j + setX] == '*')) {
			multiHit = true;
			board[setY - i][i + setX] = '*';
			
			board[setY - j][j + setX] = currentPlayer;
			cout << "A piece has been killed!" << endl;

			setY = (setY - j);
			setX = (j + setX);

			i = i + 2;
			j = j + 2;

		
			return true;
		}
		else {

			return false;
		}

	} while (((board[setY - i][i + setX] == 'O') || (board[setY - i][i + setX] == 'W')));


}

bool CheckerBoard::hitMultiplierTopLeftXNormal(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayer, bool &multiHit) {


	do {
		if (((board[setY - i][setX - i] == 'O') || (board[setY - i][setX - i] == 'W')) && (board[setY - j][setX - j] == '*')) {
			multiHit = true;
			board[setY - i][setX - i] = '*';
			
			board[setY - j][setX - j] = currentPlayer;
			cout << "A piece has been killed!" << endl;

			setY = (setY - j);
			setX = (setX - j);
			
			i = i + 2;
			j = j + 2;

			
			return true;
		}
		else {

			return false;
		}

	} while (((board[setY - i][setX - i] == 'O') || (board[setY - i][setX - i] == 'W')));


}

bool CheckerBoard::hitMultiplierBottomRightXNormal(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayer, bool &multiHit) {

	
	do {
		
		
		if (((board[i + setY][i + setX] == 'O') || (board[i + setY][i + setX] == 'W')) && (board[j + setY][j + setX] == '*')) {
			multiHit = true;
			
			board[i + setY][i + setX] = '*';
			
			board[j + setY][j + setX] = currentPlayer;
			
			cout << "A piece has been killed!" << endl;
			

			setY = (j + setY);
			setX = (j + setX);

			i = i + 2;
			j = j + 2;
		
			
			return true;
		}
		else {

			return false;
		}

	} while (((board[i + setY][i + setX] == 'O') || (board[i + setY][i + setX] == 'W')) && (board[j + setY][j + setX] == '*'));


}

bool CheckerBoard::hitMultiplierBottomLeftXNormal(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayer, bool &multiHit) {


	do {
		if (((board[i + setY][setX - i] == 'O') || (board[i + setY][setX - i] == 'W')) && (board[j + setY][setX - j] == '*')) {
			multiHit = true;
			board[i + setY][setX - i] = '*';
			
			board[j + setY][setX - j] = currentPlayer;
			cout << "A piece has been killed!" << endl;

			setY = (j + setY);
			setX = (setX - j);

			i = i + 2;
			j = j + 2;

		
			return true;
		}
		else {


			return false;
		}

	} while (((board[i + setY][setX - i] == 'O') || (board[i + setY][setX - i] == 'W')));


}

bool CheckerBoard::hitMultiplierTopRightONormal(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayer, bool &multiHit) {

	do {
		if (((board[setY - i][i + setX] == 'X') || (board[setY - i][i + setX] == 'M')) && (board[setY - j][j + setX] == '*')) {
			multiHit = true;
			board[setY - i][i + setX] = '*';
			
			board[setY - j][j + setX] = currentPlayer;
			cout << "A piece has been killed!" << endl;

			i = i + 2;
			j = j + 2;

			return true;
		}
		else {

			return false;
		}

	} while (((board[i + setY][setX - i] == 'X') || (board[i + setY][setX - i] == 'M')));
}

bool CheckerBoard::hitMultiplierTopLeftONormal(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayer, bool &multiHit) {

	do {
		if (((board[setY - i][setX - i] == 'X') || (board[setY - i][setX - i] == 'M')) && (board[setY - j][setX - j] == '*')) {
			multiHit = true;
			board[setY - i][setX - i] = '*';
			
			board[setY - j][setX - j] = currentPlayer;
			cout << "A piece has been killed!" << endl;

			i = i + 2;
			j = j + 2;

			return true;
		}
		else {

			return false;
		}

	} while (((board[i + setY][setX - i] == 'X') || (board[i + setY][setX - i] == 'M')));

}

bool CheckerBoard::hitMultiplierBottomRightONormal(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayer, bool &multiHit) {

	do {
		if (((board[i + setY][i + setX] == 'X') || (board[i + setY][i + setX] == 'M')) && (board[j + setY][j + setX] == '*')) {
			multiHit = true;
			board[i + setY][i + setX] = '*';
			
			board[j + setY][j + setX] = currentPlayer;
			cout << "A piece has been killed!" << endl;

			i = i + 2;
			j = j + 2;

			return true;
		}
		else {

			return false;
		}

	} while (((board[i + setY][setX - i] == 'X') || (board[i + setY][setX - i] == 'M')));

}

bool CheckerBoard::hitMultiplierBottomLeftONormal(int x1, int y1, int &setX, int &setY, int i, int j, char currentPlayer, bool &multiHit) {
	do {
		if (((board[i + setY][setX - i] == 'X') || (board[i + setY][setX - i] == 'M')) && (board[j + setY][setX - j] == '*')) {
			multiHit = true;
			board[i + setY][setX - i] = '*';
			
			board[j + setY][setX - j] = currentPlayer;
			cout << "A piece has been killed!" << endl;

			i = i + 2;
			j = j + 2;

			return true;
		}
		else {


			return false;
		}

	} while (((board[i + setY][setX - i] == 'X') || (board[i + setY][setX - i] == 'M')));
}

bool CheckerBoard::checkHit(int x, int y, int x1, int y1, int i, int j, int &setX, int &setY, char currentPlayer, char currentPlayerKing, char player1King, char player2King, bool &multiHit) {


	bool topRightCheck = false;
	bool topLeftCheck = false;
	bool bottomRightCheck = false;
	bool bottomLeftCheck = false;

	if ((board[y][x] == 'M') && ((board[y1][x1] == 'O') || (board[y1][x1] == 'W'))
		&& ((x < x1) && (y > y1)) && (board[(y1 - 1)][(1 + x1)] == '*')) {


		board[y][x] = '*';
		board[y1][x1] = '*';

		board[(y1 - 1)][(1 + x1)] = currentPlayerKing;
		setY = (y1 - 1);
		setX = (1 + x1);
		board[setY][setX] = '*';
		do {

			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			topRightCheck = hitMultiplierTopRightX(x1, y1, setX, setY, i, j, currentPlayerKing);
			
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			topLeftCheck = hitMultiplierTopLeftX(x1, y1, setX, setY, i, j, currentPlayerKing);
			
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			bottomRightCheck = hitMultiplierBottomRightX(x1, y1, setX, setY, i, j, currentPlayerKing);
			
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			bottomLeftCheck = hitMultiplierBottomLeftX(x1, y1, setX, setY, i, j, currentPlayerKing);
			
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			board[setY][setX] = currentPlayerKing;
		}

		while (topRightCheck == true || topLeftCheck == true || bottomLeftCheck == true || bottomRightCheck == true);
		board[setY][setX] = currentPlayerKing;


		return true;
	}

	if ((board[y][x] == 'M') && ((board[y1][x1] == 'O') || (board[y1][x1] == 'W'))
		&& ((x > x1) && (y > y1)) && (board[(y1 - 1)][(x1 - 1)] == '*')) {


		board[y][x] = '*';
		board[y1][x1] = '*';

		board[(y1 - 1)][(x1 - 1)] = currentPlayerKing;
		setY = (y1 - 1);
		setX = (x1 - 1);
		board[setY][setX] = '*';
		do {

			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			topRightCheck = hitMultiplierTopRightX(x1, y1, setX, setY, i, j, currentPlayerKing);
		
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			topLeftCheck = hitMultiplierTopLeftX(x1, y1, setX, setY, i, j, currentPlayerKing);
	
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			bottomRightCheck = hitMultiplierBottomRightX(x1, y1, setX, setY, i, j, currentPlayerKing);
	
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			bottomLeftCheck = hitMultiplierBottomLeftX(x1, y1, setX, setY, i, j, currentPlayerKing);
		
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			board[setY][setX] = currentPlayerKing;
		}

		while (topRightCheck == true || topLeftCheck == true || bottomLeftCheck == true || bottomRightCheck == true);
		board[setY][setX] = currentPlayerKing;

		return true;
	}

	if ((board[y][x] == 'M') && ((board[y1][x1] == 'O') || (board[y1][x1] == 'W'))
		&& ((x < x1) && (y < y1)) && (board[(1 + y1)][(1 + x1)] == '*')) {

		board[y][x] = '*';
		board[y1][x1] = '*';

		board[(1 + y1)][(1 + x1)] = currentPlayerKing;
		setY = (1 + y1);
		setX = (1 + x1);
		board[setY][setX] = '*';
		do {

			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			topRightCheck = hitMultiplierTopRightX(x1, y1, setX, setY, i, j, currentPlayerKing);
			
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			topLeftCheck = hitMultiplierTopLeftX(x1, y1, setX, setY, i, j, currentPlayerKing);
			
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			bottomRightCheck = hitMultiplierBottomRightX(x1, y1, setX, setY, i, j, currentPlayerKing);
		
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			bottomLeftCheck = hitMultiplierBottomLeftX(x1, y1, setX, setY, i, j, currentPlayerKing);
		
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			board[setY][setX] = currentPlayerKing;
		}

		while (topRightCheck == true || topLeftCheck == true || bottomLeftCheck == true || bottomRightCheck == true);
		
		return true;
	}

	if ((board[y][x] == 'M') && ((board[y1][x1] == 'O') || (board[y1][x1] == 'W'))
		&& ((x > x1) && (y < y1)) && (board[(1 + y1)][(x1 - 1)] == '*')) {

		board[y][x] = '*';
		board[y1][x1] = '*';

		board[(1 + y1)][(x1 - 1)] = currentPlayerKing;
		setY = (1 + y1);
		setX = (x1 - 1);
		board[setY][setX] = '*';
		do {

			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			topRightCheck = hitMultiplierTopRightX(x1, y1, setX, setY, i, j, currentPlayerKing);
			
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			topLeftCheck = hitMultiplierTopLeftX(x1, y1, setX, setY, i, j, currentPlayerKing);
			
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			bottomRightCheck = hitMultiplierBottomRightX(x1, y1, setX, setY, i, j, currentPlayerKing);
		
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			bottomLeftCheck = hitMultiplierBottomLeftX(x1, y1, setX, setY, i, j, currentPlayerKing);
	
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			board[setY][setX] = currentPlayerKing;
		}

		while (topRightCheck == true || topLeftCheck == true || bottomLeftCheck == true || bottomRightCheck == true);
		board[setY][setX] = currentPlayerKing;
		return true;
	}

	if ((board[y][x] == 'W') && ((board[y1][x1] == 'X') || (board[y1][x1] == 'M'))
		&& ((x < x1) && (y > y1)) && (board[(y1 - 1)][(1 + x1)] == '*')) {
		board[y][x] = '*';
		board[y1][x1] = '*';

		board[(y1 - 1)][(1 + x1)] = currentPlayerKing;
		setY = (y1 - 1);
		setX = (1 + x1);
		board[setY][setX] = '*';
		do {

			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			topRightCheck = hitMultiplierTopRightO(x1, y1, setX, setY, i, j, currentPlayerKing);
			
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			topLeftCheck = hitMultiplierTopLeftO(x1, y1, setX, setY, i, j, currentPlayerKing);
		
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			bottomRightCheck = hitMultiplierBottomRightO(x1, y1, setX, setY, i, j, currentPlayerKing);
		
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			bottomLeftCheck = hitMultiplierBottomLeftO(x1, y1, setX, setY, i, j, currentPlayerKing);
		
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			board[setY][setX] = currentPlayerKing;
		}

		while (topRightCheck == true || topLeftCheck == true || bottomLeftCheck == true || bottomRightCheck == true);
		return true;
	}


	if ((board[y][x] == 'W') && ((board[y1][x1] == 'X') || (board[y1][x1] == 'M'))
		&& ((x > x1) && (y > y1)) && (board[(y1 - 1)][(x1 - 1)] == '*')) {
		board[y][x] = '*';
		board[y1][x1] = '*';

		board[(y1 - 1)][(x1 - 1)] = currentPlayerKing;
		setY = (y1 - 1);
		setX = (x1 - 1);
		board[setY][setX] = '*';
		do {

			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			topRightCheck = hitMultiplierTopRightO(x1, y1, setX, setY, i, j, currentPlayerKing);
			
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			topLeftCheck = hitMultiplierTopLeftO(x1, y1, setX, setY, i, j, currentPlayerKing);
			
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			bottomRightCheck = hitMultiplierBottomRightO(x1, y1, setX, setY, i, j, currentPlayerKing);
			
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			bottomLeftCheck = hitMultiplierBottomLeftO(x1, y1, setX, setY, i, j, currentPlayerKing);
		
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			board[setY][setX] = currentPlayerKing;
		}

		while (topRightCheck == true || topLeftCheck == true || bottomLeftCheck == true || bottomRightCheck == true);
		return true;
	}

	if ((board[y][x] == 'W') && ((board[y1][x1] == 'X') || (board[y1][x1] == 'M'))
		&& ((x < x1) && (y < y1)) && (board[(1 + y1)][(1 + x1)] == '*')) {
		board[y][x] = '*';
		board[y1][x1] = '*';

		board[(1 + y1)][(1 + x1)] = currentPlayerKing;
		setY = (1 + y1);
		setX = (1 + x1);
		board[setY][setX] = '*';
		do {

			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			topRightCheck = hitMultiplierTopRightO(x1, y1, setX, setY, i, j, currentPlayerKing);
			
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			topLeftCheck = hitMultiplierTopLeftO(x1, y1, setX, setY, i, j, currentPlayerKing);
			
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			bottomRightCheck = hitMultiplierBottomRightO(x1, y1, setX, setY, i, j, currentPlayerKing);
			
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			bottomLeftCheck = hitMultiplierBottomLeftO(x1, y1, setX, setY, i, j, currentPlayerKing);
			
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			board[setY][setX] = currentPlayerKing;
		}

		while (topRightCheck == true || topLeftCheck == true || bottomLeftCheck == true || bottomRightCheck == true);
		return true;
	}

	if ((board[y][x] == 'W') && ((board[y1][x1] == 'X') || (board[y1][x1] == 'M'))
		&& ((x > x1) && (y < y1)) && (board[(1 + y1)][(x1 - 1)] == '*')) {
		board[y][x] = '*';
		board[y1][x1] = '*';

		board[(1 + y1)][(x1 - 1)] = currentPlayerKing;
		setY = (1 + y1);
		setX = (x1 - 1);
		board[setY][setX] = '*';
		do {

			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			topRightCheck = hitMultiplierTopRightO(x1, y1, setX, setY, i, j, currentPlayerKing);
			
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			topLeftCheck = hitMultiplierTopLeftO(x1, y1, setX, setY, i, j, currentPlayerKing);
		
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			bottomRightCheck = hitMultiplierBottomRightO(x1, y1, setX, setY, i, j, currentPlayerKing);
			
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			bottomLeftCheck = hitMultiplierBottomLeftO(x1, y1, setX, setY, i, j, currentPlayerKing);
			
			findCurrentPlayerKing(x, y, x1, y1, setX, setY, currentPlayerKing);
			board[setY][setX] = currentPlayerKing;
		}

		while (topRightCheck == true || topLeftCheck == true || bottomLeftCheck == true || bottomRightCheck == true);
		return true;
	}

	if (((board[y][x] == 'X') && (board[y][x] == currentPlayer)) && ((board[y1][x1] == 'O') || (board[y1][x1] == 'W'))
		&& ((x < x1) && (y < y1)) && (y1 == (y + 1)) && (board[(1 + y1)][(1 + x1)] == '*')) {
		board[y][x] = '*';
		board[y1][x1] = '*';
		board[(1 + y1)][(1 + x1)] = currentPlayer;
		setY = (1 + y1);
		setX = (1 + x1);
	
		do {

	
			findCurrentPlayer(x, y, x1, y1, setX, setY, currentPlayer);
			
			bottomRightCheck = hitMultiplierBottomRightXNormal(x1, y1, setX, setY, i, j, currentPlayer, multiHit);
	
			
			findCurrentPlayer(x, y, x1, y1, setX, setY, currentPlayer);
			
			bottomLeftCheck = hitMultiplierBottomLeftXNormal(x1, y1, setX, setY, i, j, currentPlayer, multiHit);
			
			
			board[setY][setX] = currentPlayer;
			
		}

		while (topRightCheck == true || topLeftCheck == true || bottomLeftCheck == true || bottomRightCheck == true);


		if ((multiHit == false) && (currentPlayer != 'M')) {
		
			board[(1 + y1)][(1 + x1)] = currentPlayer;
		}

		multiHit = false;
	
		cout << "A piece has been killed!" << endl;
		return true;
	}

	if (((board[y][x] == 'X') && (board[y][x] == currentPlayer)) && ((board[y1][x1] == 'O') || (board[y1][x1] == 'W'))
		&& ((x > x1) && (y < y1)) && (y1 == (y + 1)) && (board[(1 + y1)][(x1 - 1)] == '*')) {
		board[y][x] = '*';
		board[y1][x1] = '*';
		board[(1 + y1)][(x1 - 1)] = currentPlayer;
		setY = (1 + y1);
		setX = (x1 - 1);

		do {


			findCurrentPlayer(x, y, x1, y1, setX, setY, currentPlayer);

			bottomRightCheck = hitMultiplierBottomRightXNormal(x1, y1, setX, setY, i, j, currentPlayer, multiHit);
	

			findCurrentPlayer(x, y, x1, y1, setX, setY, currentPlayer);

			bottomLeftCheck = hitMultiplierBottomLeftXNormal(x1, y1, setX, setY, i, j, currentPlayer, multiHit);

			
			board[setY][setX] = currentPlayer;

			
		}

		while (topRightCheck == true || topLeftCheck == true || bottomLeftCheck == true || bottomRightCheck == true);



		if (multiHit == false) {
			board[(1 + y1)][(x1 - 1)] = currentPlayer;
		}

		multiHit = false;

		cout << "A piece has been killed!" << endl;
		return true;
	}

	if (((board[y][x] == 'O') && (board[y][x] == currentPlayer)) && ((board[y1][x1] == 'X') || (board[y1][x1] == 'M'))
		&& ((x < x1) && (y > y1)) && (y1 == (y - 1)) && (board[(y1 - 1)][(1 + x1)] == '*')) {
		board[y][x] = '*';
		board[y1][x1] = '*';
		board[(y1 - 1)][(1 + x1)] = currentPlayer;
		setY = (y1 - 1);
		setX = (1 + x1);


		do {


			findCurrentPlayer(x, y, x1, y1, setX, setY, currentPlayer);

			bottomRightCheck = hitMultiplierBottomRightXNormal(x1, y1, setX, setY, i, j, currentPlayer, multiHit);
	

			findCurrentPlayer(x, y, x1, y1, setX, setY, currentPlayer);

			bottomLeftCheck = hitMultiplierBottomLeftXNormal(x1, y1, setX, setY, i, j, currentPlayer, multiHit);

			
			board[setY][setX] = currentPlayer;

		
		}

		while (topRightCheck == true || topLeftCheck == true || bottomLeftCheck == true || bottomRightCheck == true);


		if ((multiHit == false ) && (currentPlayer != 'M')) {
			
			board[(y1 - 1)][(1 + x1)] = currentPlayer;
		}

		multiHit = false;
		
		cout << "A piece has been killed!" << endl;
		return true;
	}

	if (((board[y][x] == 'O') && (board[y][x] == currentPlayer)) && ((board[y1][x1] == 'X') || (board[y1][x1] == 'M'))
		&& ((x > x1) && (y > y1)) && (y1 == (y - 1)) && (board[(y1 - 1)][(x1 - 1)] == '*')) {
		board[y][x] = '*';
		board[y1][x1] = '*';
		board[(y1 - 1)][(x1 - 1)] = currentPlayer;
		setY = (y1 - 1);
		setX = (x1 - 1);

		do {


			findCurrentPlayer(x, y, x1, y1, setX, setY, currentPlayer);

			bottomRightCheck = hitMultiplierBottomRightXNormal(x1, y1, setX, setY, i, j, currentPlayer, multiHit);
		

			findCurrentPlayer(x, y, x1, y1, setX, setY, currentPlayer);

			bottomLeftCheck = hitMultiplierBottomLeftXNormal(x1, y1, setX, setY, i, j, currentPlayer, multiHit);

		
			board[setY][setX] = currentPlayer;

			
		}

		while (topRightCheck == true || topLeftCheck == true || bottomLeftCheck == true || bottomRightCheck == true);

	
		if (multiHit == false) {
			board[(y1 - 1)][(x1 - 1)] = currentPlayer;
		}

		multiHit = false;

		cout << "A piece has been killed!" << endl;
		return true;
	}

	else {
		return false;
	}


}

bool CheckerBoard::kingMaker(int x1, int y1, int &setX, int &setY, PlayerPiece xPiece1, PlayerPiece xPiece2, PlayerPiece xPiece3,
	PlayerPiece xPiece4, PlayerPiece xPiece5, PlayerPiece xPiece6, PlayerPiece xPiece7, PlayerPiece xPiece8,
	PlayerPiece xPiece9, PlayerPiece xPiece10, PlayerPiece xPiece11, PlayerPiece xPiece12,
	PlayerPiece2 oPiece1, PlayerPiece2 oPiece2, PlayerPiece2 oPiece3, PlayerPiece2 oPiece4,
	PlayerPiece2 oPiece5, PlayerPiece2 oPiece6, PlayerPiece2 oPiece7, PlayerPiece2 oPiece8,
	PlayerPiece2 oPiece9, PlayerPiece2 oPiece10, PlayerPiece2 oPiece11, PlayerPiece2 oPiece12) {



	if (((y1 == 7) || (setY == 7)) && ((board[y1][x1] == 'X') || (board[setY][setX] == 'X'))) {
		xPiece1.set_Symbol('M');
		xPiece1.set_isKing(true);
		if (y1 == 7) {
			board[y1][x1] = xPiece1.get_Symbol();
		}
		if (setY == 7) {
			board[setY][setX] = xPiece1.get_Symbol();
		}
		
		return true;
	}


	if (((y1 == 0) || (setY == 0)) && ((board[y1][x1] == 'O') || (board[setY][setX] == 'O'))) {
		oPiece1.set_Symbol('W');
		oPiece1.set_isKing(true);
		if (y1 == 0) {
			board[y1][x1] = oPiece1.get_Symbol();
		}
		if (setY == 0) {
			board[setY][setX] = oPiece1.get_Symbol();
		}
		
		return true;
	}


	else {
		
		return false;

	}
}

