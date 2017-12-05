#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

auto humanTurn = true;

auto scoreMin = 100, scoreMax = 0, maxDepth = 0, nodeCount = 0, maxPruneCount = 0, minPruneCount = 0;

enum GameState {ONGOING, DRAW, BLACK_WIN, WHITE_WIN};

int gameState = ONGOING;

struct Space{ /*object that gives info on what is in a space on the board*/

	char color; /*colors are either w for white, b for black, n for none, or x for invalid*/
	int xCoord; 
	char yCoord;
	int vectorNum;
	bool isCastle;

};

vector<Space> boardVec(89); /*vector that has 88 space objects, one for each space on the board and one to represent invalid coordinates*/

void displayBoard(vector<Space>& bvec){ /*displays the board using the values from the space vector*/
	cout << '\n'
		<< "               _______" << '\n'
		<< "              | " << bvec[86].color << " | " << bvec[87].color << " | " << '\n'
		<< " n         ___|___|___|___" << '\n'
		<< "          | " << bvec[82].color << " | " << bvec[83].color << " | " << bvec[84].color << " | " << bvec[85].color << " | " << '\n'
		<< " m     ___|___|___|___|___|___" << '\n'
		<< "      | " << bvec[76].color << " | " << bvec[77].color << " | " << bvec[78].color << " | " << bvec[79].color << " | " << bvec[80].color << " | " << bvec[81].color << " | " << '\n'
		<< " l ___|___|___|___|___|___|___|___" << '\n'
		<< "  | " << bvec[68].color << " | " << bvec[69].color << " | " << bvec[70].color << " | " << bvec[71].color << " | " << bvec[72].color << " | " << bvec[73].color << " | " << bvec[74].color << " | " << bvec[75].color << " | " << '\n'
		<< " k|___|___|___|___|___|___|___|___|" << '\n'
		<< "  | " << bvec[60].color << " | " << bvec[61].color << " | " << bvec[62].color << " | " << bvec[63].color << " | " << bvec[64].color << " | " << bvec[65].color << " | " << bvec[66].color << " | " << bvec[67].color << " | " << '\n'
		<< " j|___|___|___|___|___|___|___|___|" << '\n'
		<< "  | " << bvec[52].color << " | " << bvec[53].color << " | " << bvec[54].color << " | " << bvec[55].color << " | " << bvec[56].color << " | " << bvec[57].color << " | " << bvec[58].color << " | " << bvec[59].color << " | " << '\n'
		<< " i|___|___|___|___|___|___|___|___|" << '\n'
		<< "  | " << bvec[44].color << " | " << bvec[45].color << " | " << bvec[46].color << " | " << bvec[47].color << " | " << bvec[48].color << " | " << bvec[49].color << " | " << bvec[50].color << " | " << bvec[51].color << " | " << '\n'
		<< " h|___|___|___|___|___|___|___|___|" << '\n'
		<< "  | " << bvec[36].color << " | " << bvec[37].color << " | " << bvec[38].color << " | " << bvec[39].color << " | " << bvec[40].color << " | " << bvec[41].color << " | " << bvec[42].color << " | " << bvec[43].color << " | " << '\n'
		<< " g|___|___|___|___|___|___|___|___|" << '\n'
		<< "  | " << bvec[28].color << " | " << bvec[29].color << " | " << bvec[30].color << " | " << bvec[31].color << " | " << bvec[32].color << " | " << bvec[33].color << " | " << bvec[34].color << " | " << bvec[35].color << " | " << '\n'
		<< " f|___|___|___|___|___|___|___|___|" << '\n'
		<< "  | " << bvec[20].color << " | " << bvec[21].color << " | " << bvec[22].color << " | " << bvec[23].color << " | " << bvec[24].color << " | " << bvec[25].color << " | " << bvec[26].color << " | " << bvec[27].color << " | " << '\n'
		<< " e|___|___|___|___|___|___|___|___|" << '\n'
		<< "  | " << bvec[12].color << " | " << bvec[13].color << " | " << bvec[14].color << " | " << bvec[15].color << " | " << bvec[16].color << " | " << bvec[17].color << " | " << bvec[18].color << " | " << bvec[19].color << " | " << '\n'
		<< " d|___|___|___|___|___|___|___|___|" << '\n'
		<< "      | " << bvec[6].color << " | " << bvec[7].color << " | " << bvec[8].color << " | " << bvec[9].color << " | " << bvec[10].color << " | " << bvec[11].color << " | " << '\n'
		<< " c    |___|___|___|___|___|___|" << '\n'
		<< "          | " << bvec[2].color << " | " << bvec[3].color << " | " << bvec[4].color << " | " << bvec[5].color << " | " << '\n'
		<< " b        |___|___|___|___|" << '\n'
		<< "              | " << bvec[0].color << " | " << bvec[1].color << " | " << '\n'
		<< " a            |___|___|" << '\n'
		<< "    1   2   3   4   5   6   7   8" << '\n'
		<< '\n';
		return;
}

void initVec(vector<Space>& bvec) { /*generates initial values for the spaces in the space vector*/
	for (auto i = 0; i < 2; i++){
		bvec[i].color = ' ';
		bvec[i].xCoord = i + 4;
		bvec[i].yCoord = 'a';
		bvec[i].vectorNum = i;
		bvec[i].isCastle = false;
	}
	for (auto i = 2; i < 6; i++){
		bvec[i].color = ' ';
		bvec[i].xCoord = i + 1;
		bvec[i].yCoord = 'b';
		bvec[i].vectorNum = i;
		bvec[i].isCastle = false;
	}
	for (auto i = 6; i < 12; i++){
		bvec[i].color = ' ';
		bvec[i].xCoord = i - 4;
		bvec[i].yCoord = 'c';
		bvec[i].vectorNum = i;
		bvec[i].isCastle = false;
	}
	for (auto i = 12; i < 20; i++){
		bvec[i].color = ' ';
		bvec[i].xCoord = i - 11;
		bvec[i].yCoord = 'd';
		bvec[i].vectorNum = i;
		bvec[i].isCastle = false;
	}
	for (auto i = 20; i < 28; i++){
		bvec[i].color = ' ';
		bvec[i].xCoord = i - 19;
		bvec[i].yCoord = 'e';
		bvec[i].vectorNum = i;
		bvec[i].isCastle = false;
	}
	for (auto i = 22; i < 26; i++){
		bvec[i].color = 'b';
	}
	for (auto i = 28; i < 36; i++){
		bvec[i].color = ' ';
		bvec[i].xCoord = i - 27;
		bvec[i].yCoord = 'f';
		bvec[i].vectorNum = i;
		bvec[i].isCastle = false;
	}
	bvec[31].color = 'b';
	bvec[32].color = 'b';
	for (auto i = 36; i < 44; i++){
		bvec[i].color = ' ';
		bvec[i].xCoord = i - 35;
		bvec[i].yCoord = 'g';
		bvec[i].vectorNum = i;
		bvec[i].isCastle = false;
	}
	for (auto i = 44; i < 52; i++){
		bvec[i].color = ' ';
		bvec[i].xCoord = i - 43;
		bvec[i].yCoord = 'h';
		bvec[i].vectorNum = i;
		bvec[i].isCastle = false;
	}
	for (auto i = 52; i < 60; i++){
		bvec[i].color = ' ';
		bvec[i].xCoord = i - 51;
		bvec[i].yCoord = 'i';
		bvec[i].vectorNum = i;
		bvec[i].isCastle = false;
	}
	bvec[55].color = 'w';
	bvec[56].color = 'w';
	for (auto i = 60; i < 68; i++){
		bvec[i].color = ' ';
		bvec[i].xCoord = i - 59;
		bvec[i].yCoord = 'j';
		bvec[i].vectorNum = i;
		bvec[i].isCastle = false;
	}
	for (auto i = 62; i < 66; i++){
		bvec[i].color = 'w';
	}
	for (auto i = 68; i < 76; i++){
		bvec[i].color = ' ';
		bvec[i].xCoord = i - 65;
		bvec[i].yCoord = 'k';
		bvec[i].vectorNum = i;
		bvec[i].isCastle = false;
	}
	for (auto i = 76; i < 82; i++){
		bvec[i].color = ' ';
		bvec[i].xCoord = i - 74;
		bvec[i].yCoord = 'l';
		bvec[i].vectorNum = i;
		bvec[i].isCastle = false;
	}
	for (auto i = 82; i < 86; i++){
		bvec[i].color = ' ';
		bvec[i].xCoord = i - 79;
		bvec[i].yCoord = 'm';
		bvec[i].vectorNum = i;
		bvec[i].isCastle = false;
	}
	for (auto i = 86; i < 88; i++){
		bvec[i].color = ' ';
		bvec[i].xCoord = i - 82;
		bvec[i].yCoord = 'n';
		bvec[i].vectorNum = i;
		bvec[i].isCastle = true;
	}
	bvec[88].color = 'x';
	bvec[88].xCoord = 1000;
	bvec[88].yCoord = 'x';
	bvec[88].vectorNum = 88;
	bvec[88].isCastle = false;
}

//checks to see if coordinates surrounding a specified Space exist
bool invalidCoordCheck(int i, char c){
	if (i < 1 || i > 8){ return true; }
	else if (int(c) > 110 || int(c) < 97){ return true; }
	else if ((c == 'a' || c == 'n') && (i < 4 || i > 5)){ return true; }
	else if ((c == 'b' || c == 'm') && (i < 3 || i > 6)){ return true; }
	else if ((c == 'c' || c == 'p') && (i < 2 || i > 7)){ return true; }
	else{ return false; }
}

//gets user input for piece selection
string getUserPieceSelect(){
	string i;
	cout << "\nSelect a piece to move this turn: ";
	cin >> i;
	return i;
}

//gets user input for move selection
string getUserMoveSelect(string& piece){
	string i;
	cout << "\nYour selected piece is " << piece << ".\n" << "Select a space to move to: ";
	cin >> i;
	return i;
}

//checks to see if a player chosen space exists
bool checkUserInput(string& input){
	if (input.length() != 2){ 
		cout << "\nThat's not a valid input. Try again.\n";
		return false; 
	}
	char c;
	char d;
	istringstream ss(input);
	ss >> c >> d;
	if (int(c) > 96 && int(c) < 111 && int(d) > 48 && int(d) < 58){
		int e = d - '0';
		if (invalidCoordCheck(e, c)){
			cout << "\nThat's not a valid input. Try again.\n";
			return false;
		}
		else { return true; }
	}
}

//converts the user input into the corresponding Space
Space convertUserInput(string& input, vector<Space>& bvec){
	char c;
	char d;
	istringstream ss(input);
	ss >> c >> d;
	int e = d - '0';
	for (Space& i : bvec){
		if (i.xCoord == e && i.yCoord == c){
			return i;
		}
	}
}

//gets the Space in a cardinal direction from the specified Space, or if coordinates are invalid, gets the invalid Space
Space getSpaceOneN(Space& og, vector<Space>& bvec){
	int sXcoord = og.xCoord;
	char sYcoord = char(int(og.yCoord) + 1);
	if (invalidCoordCheck(sXcoord, sYcoord)){ return bvec[88]; }
	for (Space& i : bvec){
		if (i.xCoord == sXcoord && i.yCoord == sYcoord){
			return i;
		}
	}
}

Space getSpaceTwoN(Space& og, vector<Space>& bvec){
	int sXcoord = og.xCoord;
	char sYcoord = char(int(og.yCoord) + 2);
	if (invalidCoordCheck(sXcoord, sYcoord)){ return bvec[88]; }
	for (Space& i : bvec){
		if (i.xCoord == sXcoord && i.yCoord == sYcoord){
			return i;
		}
	}
}

Space getSpaceOneS(Space& og, vector<Space>& bvec){
	int sXcoord = og.xCoord;
	char sYcoord = char(int(og.yCoord) - 1);
	if (invalidCoordCheck(sXcoord, sYcoord)){ return bvec[88]; }
	for (Space& i : bvec){
		if (i.xCoord == sXcoord && i.yCoord == sYcoord){
			return i;
		}
	}
}

Space getSpaceTwoS(Space& og, vector<Space>& bvec){
	int sXcoord = og.xCoord;
	char sYcoord = char(int(og.yCoord) - 2);
	if (invalidCoordCheck(sXcoord, sYcoord)){ return bvec[88]; }
	for (Space& i : bvec){
		if (i.xCoord == sXcoord && i.yCoord == sYcoord){
			return i;
		}
	}
}

Space getSpaceOneE(Space& og, vector<Space>& bvec){
	int sXcoord = og.xCoord + 1;
	char sYcoord = og.yCoord;
	if (invalidCoordCheck(sXcoord, sYcoord)){ return bvec[88]; }
	for (Space& i : bvec){
		if (i.xCoord == sXcoord && i.yCoord == sYcoord){
			return i;
		}
	}
}

Space getSpaceTwoE(Space& og, vector<Space>& bvec){
	int sXcoord = og.xCoord + 2;
	char sYcoord = og.yCoord;
	if (invalidCoordCheck(sXcoord, sYcoord)){ return bvec[88]; }
	for (Space& i : bvec){
		if (i.xCoord == sXcoord && i.yCoord == sYcoord){
			return i;
		}
	}
}

Space getSpaceOneW(Space& og, vector<Space>& bvec){
	int sXcoord = og.xCoord - 1;
	char sYcoord = og.yCoord;
	if (invalidCoordCheck(sXcoord, sYcoord)){ return bvec[88]; }
	for (Space& i : bvec){
		if (i.xCoord == sXcoord && i.yCoord == sYcoord){
			return i;
		}
	}
}

Space getSpaceTwoW(Space& og, vector<Space>& bvec){
	int sXcoord = og.xCoord - 2;
	char sYcoord = og.yCoord;
	if (invalidCoordCheck(sXcoord, sYcoord)){ return bvec[88]; }
	for (Space& i : bvec){
		if (i.xCoord == sXcoord && i.yCoord == sYcoord){
			return i;
		}
	}
}

Space getSpaceOneNE(Space& og, vector<Space>& bvec){
	int sXcoord = og.xCoord + 1;
	char sYcoord = char(int(og.yCoord) + 1);
	if (invalidCoordCheck(sXcoord, sYcoord)){ return bvec[88]; }
	for (Space& i : bvec){
		if (i.xCoord == sXcoord && i.yCoord == sYcoord){
			return i;
		}
	}
}

Space getSpaceTwoNE(Space& og, vector<Space>& bvec){
	int sXcoord = og.xCoord + 2;
	char sYcoord = char(int(og.yCoord) + 2);
	if (invalidCoordCheck(sXcoord, sYcoord)){ return bvec[88]; }
	for (Space& i : bvec){
		if (i.xCoord == sXcoord && i.yCoord == sYcoord){
			return i;
		}
	}
}

Space getSpaceOneSE(Space& og, vector<Space>& bvec){
	int sXcoord = og.xCoord + 1;
	char sYcoord = char(int(og.yCoord) - 1);
	if (invalidCoordCheck(sXcoord, sYcoord)){ return bvec[88]; }
	for (Space& i : bvec){
		if (i.xCoord == sXcoord && i.yCoord == sYcoord){
			return i;
		}
	}
}

Space getSpaceTwoSE(Space& og, vector<Space>& bvec){
	int sXcoord = og.xCoord + 2;
	char sYcoord = char(int(og.yCoord) - 2);
	if (invalidCoordCheck(sXcoord, sYcoord)){ return bvec[88]; }
	for (Space& i : bvec){
		if (i.xCoord == sXcoord && i.yCoord == sYcoord){
			return i;
		}
	}
}

Space getSpaceOneNW(Space& og, vector<Space>& bvec){
	int sXcoord = og.xCoord - 1;
	char sYcoord = char(int(og.yCoord) + 1);
	if (invalidCoordCheck(sXcoord, sYcoord)){ return bvec[88]; }
	for (Space& i : bvec){
		if (i.xCoord == sXcoord && i.yCoord == sYcoord){
			return i;
		}
	}
}

Space getSpaceTwoNW(Space& og, vector<Space>& bvec){
	int sXcoord = og.xCoord - 2;
	char sYcoord = char(int(og.yCoord) + 2);
	if (invalidCoordCheck(sXcoord, sYcoord)){ return bvec[88]; }
	for (Space& i : bvec){
		if (i.xCoord == sXcoord && i.yCoord == sYcoord){
			return i;
		}
	}
}

Space getSpaceOneSW(Space& og, vector<Space>& bvec){
	int sXcoord = og.xCoord - 1;
	char sYcoord = char(int(og.yCoord) - 1);
	if (invalidCoordCheck(sXcoord, sYcoord)){ return bvec[88]; }
	for (Space& i : bvec){
		if (i.xCoord == sXcoord && i.yCoord == sYcoord){
			return i;
		}
	}
}

Space getSpaceTwoSW(Space& og, vector<Space>& bvec){
	int sXcoord = og.xCoord - 2;
	char sYcoord = char(int(og.yCoord) - 2);
	if (invalidCoordCheck(sXcoord, sYcoord)){ return bvec[88]; }
	for (Space& i : bvec){
		if (i.xCoord == sXcoord && i.yCoord == sYcoord){
			return i;
		}
	}
}

//white (human player) turn and input checks
bool checkIfWhiteCanCapture(vector<Space>& bvec){ //checks to see if the player can capture an enemy piece
	for (Space i : bvec){
		if (i.color == 'w'){
			Space osn = getSpaceOneN(i, bvec);
			Space oss = getSpaceOneS(i, bvec);
			Space ose = getSpaceOneE(i, bvec);
			Space osw = getSpaceOneN(i, bvec);
			Space osne = getSpaceOneNE(i, bvec);
			Space osse = getSpaceOneSE(i, bvec);
			Space osnw = getSpaceOneNW(i, bvec);
			Space ossw = getSpaceOneSW(i, bvec);

			Space tsn = getSpaceTwoN(i, bvec);
			Space tss = getSpaceTwoS(i, bvec);
			Space tse = getSpaceTwoE(i, bvec);
			Space tsw = getSpaceTwoN(i, bvec);
			Space tsne = getSpaceTwoNE(i, bvec);
			Space tsse = getSpaceTwoSE(i, bvec);
			Space tsnw = getSpaceTwoNW(i, bvec);
			Space tssw = getSpaceTwoSW(i, bvec);

			if ((osn.color == 'b' && tsn.color == ' ') || (oss.color == 'b' && tss.color == ' ') || (ose.color == 'b' && tse.color == ' ') || (osw.color == 'b' && tsw.color == ' ') ||
				(osne.color == 'b' && tsne.color == ' ') || (osse.color == 'b' && tsse.color == ' ') || (osnw.color == 'b' && tsnw.color == ' ') || (ossw.color == 'b' && tssw.color == ' ')){
				cout << "\nYou are obligated to capture an enemy piece.\n";
				return true;
			}
		}
	}
	return false;
}

bool checkIfPlayerCanMove(Space& og, vector<Space>& bvec){ //checks to see if the player's selected Space is valid or can move (only when player cannot capture)
	if (og.color == 'b'){
		cout << "\nThat's not your piece. Try again.\n";
		return false;
	}
	else if (og.color == ' '){
		cout << "\nThere isn't a piece on that space. Try again.\n";
		return false;
	}
	Space osn = getSpaceOneN(og, bvec);
	Space oss = getSpaceOneS(og, bvec);
	Space ose = getSpaceOneE(og, bvec);
	Space osw = getSpaceOneN(og, bvec);
	Space osne = getSpaceOneNE(og, bvec);
	Space osse = getSpaceOneSE(og, bvec);
	Space osnw = getSpaceOneNW(og, bvec);
	Space ossw = getSpaceOneSW(og, bvec);

	if (osn.color == ' ' || oss.color == ' ' || ose.color == ' ' || osw.color == ' ' || osne.color == ' ' || osse.color == ' ' || osnw.color == ' ' || ossw.color == ' '){ return true; }

	Space tsn = getSpaceTwoN(og, bvec);
	Space tss = getSpaceTwoS(og, bvec);
	Space tse = getSpaceTwoE(og, bvec);
	Space tsw = getSpaceTwoN(og, bvec);
	Space tsne = getSpaceTwoNE(og, bvec);
	Space tsse = getSpaceTwoSE(og, bvec);
	Space tsnw = getSpaceTwoNW(og, bvec);
	Space tssw = getSpaceTwoSW(og, bvec);

	if ((osn.color == 'w' && tsn.color == ' ') || (oss.color == 'w' && tss.color == ' ') || (ose.color == 'w' && tse.color == ' ') || (osw.color == 'w' && tsw.color == ' ') ||
		(osne.color == 'w' && tsne.color == ' ') || (osse.color == 'w' && tsse.color == ' ') || (osnw.color == 'w' && tsnw.color == ' ') || (ossw.color == 'w' && tssw.color == ' ')){
		return true;
	}

	cout << "\nThis piece has no valid moves. Try again.\n";
	return false;
}

bool checkIfPlayerCanCapture(Space& og, vector<Space>& bvec){ //checks to see if the player's selected Space is valid or can capture (only when capture is obligatory)
	if (og.color == 'b'){
		cout << "\nThat's not your piece. Try again.\n";
		return false;
	}
	else if (og.color == ' '){
		cout << "\nThere isn't a piece on that space. Try again.\n";
		return false;
	}
	Space osn = getSpaceOneN(og, bvec);
	Space oss = getSpaceOneS(og, bvec);
	Space ose = getSpaceOneE(og, bvec);
	Space osw = getSpaceOneN(og, bvec);
	Space osne = getSpaceOneNE(og, bvec);
	Space osse = getSpaceOneSE(og, bvec);
	Space osnw = getSpaceOneNW(og, bvec);
	Space ossw = getSpaceOneSW(og, bvec);

	Space tsn = getSpaceTwoN(og, bvec);
	Space tss = getSpaceTwoS(og, bvec);
	Space tse = getSpaceTwoE(og, bvec);
	Space tsw = getSpaceTwoN(og, bvec);
	Space tsne = getSpaceTwoNE(og, bvec);
	Space tsse = getSpaceTwoSE(og, bvec);
	Space tsnw = getSpaceTwoNW(og, bvec);
	Space tssw = getSpaceTwoSW(og, bvec);

	if ((osn.color == 'b' && tsn.color == ' ') || (oss.color == 'b' && tss.color == ' ') || (ose.color == 'b' && tse.color == ' ') || (osw.color == 'b' && tsw.color == ' ') ||
		(osne.color == 'b' && tsne.color == ' ') || (osse.color == 'b' && tsse.color == ' ') || (osnw.color == 'b' && tsnw.color == ' ') || (ossw.color == 'b' && tssw.color == ' ')){
		return true;
	}
	cout << "\nThis piece has no valid moves. Try again.\n";
	return false;
}

bool checkIfPlayerDestValid(Space& piece, Space& destination, vector<Space>& bvec){ //checks to see if the player chosen destination for the player chosen piece is valid (only when player cannot capture)
	if (destination.color != ' '){ 
		cout << "\nYou cannot move to an occupied space.Try again.\n";
		return false; 
	}

	Space osn = getSpaceOneN(destination, bvec);
	Space oss = getSpaceOneS(destination, bvec);
	Space ose = getSpaceOneE(destination, bvec);
	Space osw = getSpaceOneN(destination, bvec);
	Space osne = getSpaceOneNE(destination, bvec);
	Space osse = getSpaceOneSE(destination, bvec);
	Space osnw = getSpaceOneNW(destination, bvec);
	Space ossw = getSpaceOneSW(destination, bvec);

	if (osn.vectorNum == piece.vectorNum || oss.vectorNum == piece.vectorNum || ose.vectorNum == piece.vectorNum || osw.vectorNum == piece.vectorNum || 
		osne.vectorNum == piece.vectorNum || osse.vectorNum == piece.vectorNum || osnw.vectorNum == piece.vectorNum || ossw.vectorNum == piece.vectorNum){ 
		return true; 
	}

	Space tsn = getSpaceTwoN(destination, bvec);
	Space tss = getSpaceTwoS(destination, bvec);
	Space tse = getSpaceTwoE(destination, bvec);
	Space tsw = getSpaceTwoN(destination, bvec);
	Space tsne = getSpaceTwoNE(destination, bvec);
	Space tsse = getSpaceTwoSE(destination, bvec);
	Space tsnw = getSpaceTwoNW(destination, bvec);
	Space tssw = getSpaceTwoSW(destination, bvec);

	if ((osn.color == 'w' && tsn.vectorNum == piece.vectorNum) || (oss.color == 'w' && tss.vectorNum == piece.vectorNum) || (ose.color == 'w' && tse.vectorNum == piece.vectorNum) || 
		(osw.color == 'w' && tsw.vectorNum == piece.vectorNum) || (osne.color == 'w' && tsne.vectorNum == piece.vectorNum) || (osse.color == 'w' && tsse.vectorNum == piece.vectorNum) || 
		(osnw.color == 'w' && tsnw.vectorNum == piece.vectorNum) || (ossw.color == 'w' && tssw.vectorNum == piece.vectorNum)){
		return true;
	}

	cout << "\nYou can't move to this space. Try again.\n";
	return false;
}

bool checkIfPlayerCapValid(Space& piece, Space& destination, vector<Space>& bvec){ //checks to see if the player chosen capturing destination for the player chosen piece is valid (only when capture is obligatory)
	if (destination.color != ' '){
		cout << "\nYou cannot move to an occupied space.Try again.\n";
		return false;
	}

	Space osn = getSpaceOneN(destination, bvec);
	Space oss = getSpaceOneS(destination, bvec);
	Space ose = getSpaceOneE(destination, bvec);
	Space osw = getSpaceOneN(destination, bvec);
	Space osne = getSpaceOneNE(destination, bvec);
	Space osse = getSpaceOneSE(destination, bvec);
	Space osnw = getSpaceOneNW(destination, bvec);
	Space ossw = getSpaceOneSW(destination, bvec);

	Space tsn = getSpaceTwoN(destination, bvec);
	Space tss = getSpaceTwoS(destination, bvec);
	Space tse = getSpaceTwoE(destination, bvec);
	Space tsw = getSpaceTwoN(destination, bvec);
	Space tsne = getSpaceTwoNE(destination, bvec);
	Space tsse = getSpaceTwoSE(destination, bvec);
	Space tsnw = getSpaceTwoNW(destination, bvec);
	Space tssw = getSpaceTwoSW(destination, bvec);

	if ((osn.color == 'b' && tsn.vectorNum == piece.vectorNum) || (oss.color == 'b' && tss.vectorNum == piece.vectorNum) || (ose.color == 'b' && tse.vectorNum == piece.vectorNum) ||
		(osw.color == 'b' && tsw.vectorNum == piece.vectorNum) || (osne.color == 'b' && tsne.vectorNum == piece.vectorNum) || (osse.color == 'b' && tsse.vectorNum == piece.vectorNum) ||
		(osnw.color == 'b' && tsnw.vectorNum == piece.vectorNum) || (ossw.color == 'b' && tssw.vectorNum == piece.vectorNum)){
		return true;
	}

	cout << "\nYou can't move to this space. Try again.\n";
	return false;
}

void whiteMoveToSpace(Space& piece, Space& destination, vector<Space>& bvec){ //moves the player's selected piece to the player's selected destination (only when player cannot capture)
	bvec[piece.vectorNum].color = ' ';
	bvec[destination.vectorNum].color = 'w';
}

void whiteCapToSpace(Space& piece, Space& destination, vector<Space>& bvec){ //moves the player's selected piece to the player's selected destination (only when capture is obligatory)
	bvec[piece.vectorNum].color = ' ';
	bvec[destination.vectorNum].color = 'w';
	int c = (piece.xCoord - destination.xCoord)/2 + destination.xCoord;
	char d = char((piece.yCoord - destination.yCoord) / 2 + destination.yCoord);
	for (Space i : bvec){
		if (i.xCoord == c && i.yCoord == d) {
			i.color = ' ';
			return;
		}
	}
}

//black (ai) turn and input checks
bool checkIfBlackCanCapture(vector<Space>& bvec){ //checks to see if the ai can capture an enemy piece
	for (Space i : bvec){
		if (i.color == 'b'){
			Space osn = getSpaceOneN(i, bvec);
			Space oss = getSpaceOneS(i, bvec);
			Space ose = getSpaceOneE(i, bvec);
			Space osw = getSpaceOneN(i, bvec);
			Space osne = getSpaceOneNE(i, bvec);
			Space osse = getSpaceOneSE(i, bvec);
			Space osnw = getSpaceOneNW(i, bvec);
			Space ossw = getSpaceOneSW(i, bvec);

			Space tsn = getSpaceTwoN(i, bvec);
			Space tss = getSpaceTwoS(i, bvec);
			Space tse = getSpaceTwoE(i, bvec);
			Space tsw = getSpaceTwoN(i, bvec);
			Space tsne = getSpaceTwoNE(i, bvec);
			Space tsse = getSpaceTwoSE(i, bvec);
			Space tsnw = getSpaceTwoNW(i, bvec);
			Space tssw = getSpaceTwoSW(i, bvec);

			if ((osn.color == 'w' && tsn.color == ' ') || (oss.color == 'w' && tss.color == ' ') || (ose.color == 'w' && tse.color == ' ') || (osw.color == 'w' && tsw.color == ' ') ||
				(osne.color == 'w' && tsne.color == ' ') || (osse.color == 'w' && tsse.color == ' ') || (osnw.color == 'w' && tsnw.color == ' ') || (ossw.color == 'w' && tssw.color == ' ')){
				return true;
			}
		}
	}
	return false;
}

vector<Space*> getBlackCaptureArray(vector<Space>& bvec){ //returns a pointer to an array with capture moves
	vector<Space*> spvec;
	for (Space i : bvec){
		if (i.color == 'b'){
			Space osn = getSpaceOneN(i, bvec);
			Space oss = getSpaceOneS(i, bvec);
			Space ose = getSpaceOneE(i, bvec);
			Space osw = getSpaceOneN(i, bvec);
			Space osne = getSpaceOneNE(i, bvec);
			Space osse = getSpaceOneSE(i, bvec);
			Space osnw = getSpaceOneNW(i, bvec);
			Space ossw = getSpaceOneSW(i, bvec);

			Space tsn = getSpaceTwoN(i, bvec);
			Space tss = getSpaceTwoS(i, bvec);
			Space tse = getSpaceTwoE(i, bvec);
			Space tsw = getSpaceTwoN(i, bvec);
			Space tsne = getSpaceTwoNE(i, bvec);
			Space tsse = getSpaceTwoSE(i, bvec);
			Space tsnw = getSpaceTwoNW(i, bvec);
			Space tssw = getSpaceTwoSW(i, bvec);

			if (osn.color == 'w' && tsn.color == ' '){
				Space* sp1 = &bvec[osn.vectorNum];
				spvec.push_back(sp1);
			}
			if (oss.color == 'w' && tsn.color == ' '){
				Space* sp2 = &bvec[osn.vectorNum];
				spvec.push_back(sp2);
			}
			if (ose.color == 'w' && tsn.color == ' '){
				Space* sp3 = &bvec[osn.vectorNum];
				spvec.push_back(sp3);
			}
			if (osw.color == 'w' && tsn.color == ' '){
				Space* sp4 = &bvec[osn.vectorNum];
				spvec.push_back(sp4);
			}
			if (osne.color == 'w' && tsn.color == ' '){
				Space* sp5 = &bvec[osn.vectorNum];
				spvec.push_back(sp5);
			}
			if (osse.color == 'w' && tsn.color == ' '){
				Space* sp6 = &bvec[osn.vectorNum];
				spvec.push_back(sp6);
			}
			if (osnw.color == 'w' && tsn.color == ' '){
				Space* sp7 = &bvec[osn.vectorNum];
				spvec.push_back(sp7);
			}
			if (ossw.color == 'w' && tsn.color == ' '){
				Space* sp8 = &bvec[osn.vectorNum];
				spvec.push_back(sp8);
			}
		}
	}
	return spvec;
}

vector<Space*> getBlackMoveArray(vector<Space>& bvec){
	vector<Space*> spvec;
	for (Space i : bvec){
		if (i.color == 'b'){
			Space osn = getSpaceOneN(i, bvec);
			Space oss = getSpaceOneS(i, bvec);
			Space ose = getSpaceOneE(i, bvec);
			Space osw = getSpaceOneN(i, bvec);
			Space osne = getSpaceOneNE(i, bvec);
			Space osse = getSpaceOneSE(i, bvec);
			Space osnw = getSpaceOneNW(i, bvec);
			Space ossw = getSpaceOneSW(i, bvec);

			Space tsn = getSpaceTwoN(i, bvec);
			Space tss = getSpaceTwoS(i, bvec);
			Space tse = getSpaceTwoE(i, bvec);
			Space tsw = getSpaceTwoN(i, bvec);
			Space tsne = getSpaceTwoNE(i, bvec);
			Space tsse = getSpaceTwoSE(i, bvec);
			Space tsnw = getSpaceTwoNW(i, bvec);
			Space tssw = getSpaceTwoSW(i, bvec);

			if (osn.color == ' '){
				Space* sp1 = &bvec[osn.vectorNum];
				spvec.push_back(sp1);
			}
			if (oss.color == ' '){
				Space* sp2 = &bvec[osn.vectorNum];
				spvec.push_back(sp2);
			}
			if (ose.color == ' '){
				Space* sp3 = &bvec[osn.vectorNum];
				spvec.push_back(sp3);
			}
			if (osw.color == ' '){
				Space* sp4 = &bvec[osn.vectorNum];
				spvec.push_back(sp4);
			}
			if (osne.color == ' '){
				Space* sp5 = &bvec[osn.vectorNum];
				spvec.push_back(sp5);
			}
			if (osse.color == ' '){
				Space* sp6 = &bvec[osn.vectorNum];
				spvec.push_back(sp6);
			}
			if (osnw.color == ' '){
				Space* sp7 = &bvec[osn.vectorNum];
				spvec.push_back(sp7);
			}
			if (ossw.color == ' '){
				Space* sp8 = &bvec[osn.vectorNum];
				spvec.push_back(sp8);
			}

			if (osn.color == 'b' && tsn.color == ' '){
				Space* sp9 = &bvec[osn.vectorNum];
				spvec.push_back(sp9);
			}
			if (oss.color == 'b' && tsn.color == ' '){
				Space* sp10 = &bvec[osn.vectorNum];
				spvec.push_back(sp10);
			}
			if (ose.color == 'b' && tsn.color == ' '){
				Space* sp11 = &bvec[osn.vectorNum];
				spvec.push_back(sp11);
			}
			if (osw.color == 'b' && tsn.color == ' '){
				Space* sp12 = &bvec[osn.vectorNum];
				spvec.push_back(sp12);
			}
			if (osne.color == 'b' && tsn.color == ' '){
				Space* sp13 = &bvec[osn.vectorNum];
				spvec.push_back(sp13);
			}
			if (osse.color == 'b' && tsn.color == ' '){
				Space* sp14 = &bvec[osn.vectorNum];
				spvec.push_back(sp14);
			}
			if (osnw.color == 'b' && tsn.color == ' '){
				Space* sp15 = &bvec[osn.vectorNum];
				spvec.push_back(sp15);
			}
			if (ossw.color == 'b' && tsn.color == ' '){
				Space* sp16 = &bvec[osn.vectorNum];
				spvec.push_back(sp16);
			}
		}
	}
	return spvec;
}

float getBlackCastleDist(int i, char c){
	float xDelta = i - 4.5;
	float yDelta = 'n' - c;
	float dist = sqrt(xDelta*xDelta + yDelta*yDelta);
	dist = roundf(dist * 100) / 100;
	return dist;
}

Space* getBlackBestChoice(vector<Space*>& spvec){
	float bestDist = 1000;
	int bestChoice = 0;
	for (int i = 0; i < spvec.size(); i++){
		float dist = getBlackCastleDist(spvec[i]->xCoord, spvec[i]->yCoord);
		if (dist < bestDist){
			bestDist = dist;
			bestChoice = i;
		}
	}
	return spvec[bestChoice];
}

void blackMoveToSpace(Space* sp, vector<Space>& bvec){
	Space osn = getSpaceOneN(*sp, bvec);
	Space oss = getSpaceOneS(*sp, bvec);
	Space ose = getSpaceOneE(*sp, bvec);
	Space osw = getSpaceOneN(*sp, bvec);
	Space osne = getSpaceOneNE(*sp, bvec);
	Space osse = getSpaceOneSE(*sp, bvec);
	Space osnw = getSpaceOneNW(*sp, bvec);
	Space ossw = getSpaceOneSW(*sp, bvec);

	Space tsn = getSpaceTwoN(*sp, bvec);
	Space tss = getSpaceTwoS(*sp, bvec);
	Space tse = getSpaceTwoE(*sp, bvec);
	Space tsw = getSpaceTwoN(*sp, bvec);
	Space tsne = getSpaceTwoNE(*sp, bvec);
	Space tsse = getSpaceTwoSE(*sp, bvec);
	Space tsnw = getSpaceTwoNW(*sp, bvec);
	Space tssw = getSpaceTwoSW(*sp, bvec);

	if (osn.color == 'b'){
		bvec[osn.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (oss.color == 'b'){
		bvec[oss.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (ose.color == 'b'){
		bvec[ose.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (osw.color == 'b'){
		bvec[osw.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (osne.color == 'b'){
		bvec[osne.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (osse.color == 'b'){
		bvec[osse.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (osnw.color == 'b'){
		bvec[osnw.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (ossw.color == 'b'){
		bvec[ossw.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (osn.color == 'b' && tsn.color == 'b'){
		bvec[tsn.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (oss.color == 'b' && tss.color == 'b'){
		bvec[tss.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (ose.color == 'b' && tse.color == 'b'){
		bvec[tse.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (osw.color == 'b' && tsw.color == 'b'){
		bvec[tsw.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (osne.color == 'b' && tsne.color == 'b'){
		bvec[tsne.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (osse.color == 'b' && tsse.color == 'b'){
		bvec[tsse.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (osnw.color == 'b' && tsnw.color == 'b'){
		bvec[tsnw.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (ossw.color == 'b' && tssw.color == 'b'){
		bvec[tssw.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
}

void blackCapToSpace(Space* sp, vector<Space>& bvec){
	Space osn = getSpaceOneN(*sp, bvec);
	Space oss = getSpaceOneS(*sp, bvec);
	Space ose = getSpaceOneE(*sp, bvec);
	Space osw = getSpaceOneN(*sp, bvec);
	Space osne = getSpaceOneNE(*sp, bvec);
	Space osse = getSpaceOneSE(*sp, bvec);
	Space osnw = getSpaceOneNW(*sp, bvec);
	Space ossw = getSpaceOneSW(*sp, bvec);

	Space tsn = getSpaceTwoN(*sp, bvec);
	Space tss = getSpaceTwoS(*sp, bvec);
	Space tse = getSpaceTwoE(*sp, bvec);
	Space tsw = getSpaceTwoN(*sp, bvec);
	Space tsne = getSpaceTwoNE(*sp, bvec);
	Space tsse = getSpaceTwoSE(*sp, bvec);
	Space tsnw = getSpaceTwoNW(*sp, bvec);
	Space tssw = getSpaceTwoSW(*sp, bvec);

	if (osn.color == 'w' && tsn.color == 'b'){
		bvec[tsn.vectorNum].color = ' ';
		bvec[osn.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (oss.color == 'w' && tss.color == 'b'){
		bvec[tss.vectorNum].color = ' ';
		bvec[oss.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (ose.color == 'w' && tse.color == 'b'){
		bvec[tse.vectorNum].color = ' ';
		bvec[ose.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (osw.color == 'w' && tsw.color == 'b'){
		bvec[tsw.vectorNum].color = ' ';
		bvec[osw.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (osne.color == 'w' && tsne.color == 'b'){
		bvec[tsne.vectorNum].color = ' ';
		bvec[osne.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (osse.color == 'w' && tsse.color == 'b'){
		bvec[tsse.vectorNum].color = ' ';
		bvec[osse.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (osnw.color == 'w' && tsnw.color == 'b'){
		bvec[tsnw.vectorNum].color = ' ';
		bvec[osnw.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
	if (ossw.color == 'w' && tssw.color == 'b'){
		bvec[tssw.vectorNum].color = ' ';
		bvec[ossw.vectorNum].color = ' ';
		sp->color = 'b';
		return;
	}
}

//checks the state of the game (ongoing, win, or draw)
void checkGameState(vector<Space>& bvec){
	auto whitepc = 0;
	auto blackpc = 0;
	for (Space i : bvec){
		if (i.color = 'w') { whitepc += 1; }
		if (i.color = 'b') { blackpc += 1; }
	}
	if (whitepc == 1 && blackpc == 1) { gameState = DRAW; }
	else if ((whitepc > 1 && blackpc == 0) || (bvec[0].color == 'w' && bvec[1].color == 'w')) { gameState = WHITE_WIN; }
	else if ((blackpc > 1 && whitepc == 0) || (bvec[86].color == 'b' && bvec[87].color == 'b')) { gameState = BLACK_WIN; }
	else { gameState = ONGOING; }
}

//win and draw messages
void blackWins(){
	cout << "\nComputer wins! Better luck next time.\n";
}

void whiteWins(){
	cout << "\nYou win! Congratulations!\n";
}

void gameDraw(){
	cout << "\nIt's a draw!\n";
}

//alpha-beta search algorithm
Space* alphaBetaBestChoice(vector <Space*>& spvec){

}

//player's turn function
void playerTurn(){
	cout << "\nYour turn.\n";
	if (checkIfWhiteCanCapture(boardVec)){											//checks if user is able to capture this turn
		displayBoard(boardVec);
		string upc = getUserPieceSelect();											//gets the user input
		while (checkUserInput(upc) == false){ 										//checks to make sure input is valid
			displayBoard(boardVec); 
			upc = getUserPieceSelect(); 
		}
		Space piece = convertUserInput(upc, boardVec);								//converts user input into a Space
		while (checkIfPlayerCanCapture(piece, boardVec) == false){					//checks if user Space can capture, and forces player to select a different Space if it cannot
			while (checkUserInput(upc) == false){ 
				displayBoard(boardVec);
				upc = getUserPieceSelect(); 
			}
			piece = convertUserInput(upc, boardVec);
		}
		displayBoard(boardVec);
		string ucap = getUserMoveSelect(upc);										//gets the user input
		while (checkUserInput(ucap) == false){ 										//checks to make sure input is valid
			displayBoard(boardVec);
			ucap = getUserMoveSelect(upc);
		}
		Space capspot = convertUserInput(ucap, boardVec);							//converts user input into a Space
		while (checkIfPlayerCapValid(piece, capspot, boardVec) == false){			//checks if user Space is a valid capture space of previous Space, and forces player to select a different Space if it is not
			while (checkUserInput(ucap) == false){ 
				displayBoard(boardVec);
				ucap = getUserMoveSelect(upc);
			}
			capspot = convertUserInput(ucap, boardVec);
		}
		whiteCapToSpace(piece, capspot, boardVec);									//performs the player capture
	}
	else {
		displayBoard(boardVec);
		string upc = getUserPieceSelect();											//gets the user input
		while (checkUserInput(upc) == false){ 										//checks to make sure input is valid
			displayBoard(boardVec);
			upc = getUserPieceSelect(); 
		}
		Space piece = convertUserInput(upc, boardVec);								//converts user input into a Space
		while (checkIfPlayerCanMove(piece, boardVec) == false){						//checks if user Space can capture, and forces player to select a different Space if it cannot
			while (checkUserInput(upc) == false){
				displayBoard(boardVec);
				upc = getUserPieceSelect(); 
			}
			piece = convertUserInput(upc, boardVec);
		}
		displayBoard(boardVec);
		string udst = getUserMoveSelect(upc);										//gets the user input
		while (checkUserInput(ucap) == false){ 										//checks to make sure input is valid
			displayBoard(boardVec);
			udst = getUserMoveSelect(upc); 
		}
		Space dest = convertUserInput(udst, boardVec);								//converts user input into a Space
		while (checkIfPlayerDestValid(piece, dest, boardVec) == false){				//checks if user Space is a valid capture space of previous Space, and forces player to select a different Space if it is not
			while (checkUserInput(udst) == false){ 
				displayBoard(boardVec);
				udst = getUserMoveSelect(upc); 
			}
			dest = convertUserInput(udst, boardVec);
		}
		whiteMoveToSpace(piece, dest, boardVec);									//performs the player move
	}
	humanTurn = false;
	displayBoard(boardVec);
}

void aiTurn(){
	cout << "\nComputer's turn.\n";
	if (checkIfBlackCanCapture(boardVec)){
		vector<Space*> spVec = getBlackCaptureArray(boardVec);
		Space* spp = getBlackBestChoice(spVec);
		blackCapToSpace(spp, boardVec);
	}
	else{
		vector<Space*> spVec = getBlackMoveArray(boardVec);
		Space* spp = getBlackBestChoice(spVec);
		blackMoveToSpace(spp, boardVec);
	}
	humanTurn = true;
	displayBoard(boardVec);
}

int main(){
	initVec(boardVec);
	cout << "\nWelcome to Mini-Camelot! Please read the readme before you play.\n";
	while (gameState == ONGOING){
		if (humanTurn){ playerTurn(); }
		else { aiTurn(); }
		checkGameState(boardVec);
	}
	if (gameState == BLACK_WIN){ blackWins(); }
	else if (gameState == WHITE_WIN){ whiteWins(); }
	else if (gameState == DRAW){ gameDraw(); }
	else { cout << "Something went wrong."; }
	//system("pause");
	cin.get();
}
