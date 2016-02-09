#include <cstdio>
#include <time.h>
#include <utility>
#include <vector>
#include <list>
#include <iterator>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;
using position = pair<int, int>;

bool isComplete(list<position> tour, int sizeBoard);
bool isFeasiblePosition(position targetPosition, vector<vector<int>> & board);
int onwardMoves(position pos, vector<position> & moves, vector<vector<int>> & board);
list<position> sortPositions(list<position> positions, vector<position> & moves, vector<vector<int>> & board);
list<position> possiblePositions(vector<position> & moves, vector<vector<int>> & board, position currentPosition);
list<position> backtracking(list<position> tour, vector<vector<int>> &board, vector<pair<int, int>> moves);
void printResults(list<position> tour, vector<vector<int>>  & board);
void solve(pair<int, int> startingPosition, int sizeBoard);
list<position> possiblePositionsSorted(vector<position> & moves, vector<vector<int>> & board, position currentPosition);

bool isComplete(list<position> tour, int sizeBoard) {
	return tour.size() == sizeBoard * sizeBoard;
}

bool isFeasiblePosition(position targetPosition, vector<vector<int>> & board) {
	if (targetPosition.first >= 0 &&
		targetPosition.first <= board.size() - 1 &&
		targetPosition.second >= 0 &&
		targetPosition.second <= board.size() - 1) {
		if (board[targetPosition.first][targetPosition.second] == 0) {
			return true;
		}
	}
	return false;
}

int onwardMoves(position pos, vector<position> & moves, vector<vector<int>> & board) {
	return possiblePositions(moves, board, pos).size();
}

list<position> sortPositions(list<position> positions, vector<position> & moves, vector<vector<int>> & board) {
	vector<position> v = {begin(positions),end(positions)};
	list<pair<int,int>> numberMoves;

	int i = 0;
	for (position pos : positions) {
		numberMoves.push_back(make_pair(onwardMoves(pos, moves,board),i));
		i++;
	}

	numberMoves.sort([](const pair<int, int>  & a, const pair<int, int> & b) {return a.first < b.first; });
	list<position> result;
	for (position pos : numberMoves) {
		result.push_back(v[pos.second]);
	}

	return result;
}

list<position> possiblePositionsSorted(vector<position> & moves, vector<vector<int>> & board, position currentPosition) {
	return sortPositions(possiblePositions(moves, board, currentPosition), moves, board);
}
list<position> possiblePositions(vector<position> & moves, vector<vector<int>> & board, position currentPosition) {
	list<position> feasiblePositions;
	for (auto move : moves) {
		position targetPosition(move.first + currentPosition.first, move.second + currentPosition.second);
		if (isFeasiblePosition(targetPosition, board)) {
			feasiblePositions.push_back(targetPosition);
		}
	}
	return feasiblePositions;
}

list<position> backtracking(list<position> tour, vector<vector<int>> &board, vector<pair<int, int>> moves) {
	list<position> result;
	if (isComplete(tour, board.size())) {
		return tour;
	}
	position currentPosition = tour.back();
	list<position> nextPositions = possiblePositionsSorted(moves, board, currentPosition);

	for (position position : nextPositions) {
		tour.push_back(position);
		board[position.first][position.second] = 1;
		result = backtracking(tour, board, moves);
		if (result.size() != 0) {
			return result;
		}
		tour.pop_back();
		board[position.first][position.second] = 0;
	}
	return result;
}

void printResults(list<position> tour, vector<vector<int>>  & board) {
	int i = 1;
	for (position pos : tour) {
		board[pos.first][pos.second] = i;
		i++;
	}

	const unsigned width = (6 * board.size()) + 1;
	const size_t msize = width + 1;
	
	char * dashedLine = (char *)malloc(msize);
	
	for (unsigned i = 0; i < width; ++i) dashedLine[i] = '-';
	dashedLine[width] = '\0';

	printf("%s\n", dashedLine);
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board.size(); j++) {
			printf("| %3d ", board[i][j]);
		}
		printf("|\n%s\n",dashedLine);
	}
}

void solve(pair<int, int> startingPosition, int sizeBoard)
{
	//Initialize the Board
	vector<vector<int>> board(sizeBoard, vector<int>(sizeBoard));
	for (int i = 0; i < sizeBoard; i++) {
		for (int j = 0; j < sizeBoard; j++) {
			board[i][j] = 0;
		}
	}
	//Create the tour's list and add the first position
	list <position> tour;
	tour.push_back(startingPosition);
	//Moves
	vector<pair<int, int>> moves = {
		make_pair(1,2),
		make_pair(1,-2),
		make_pair(-1,2),
		make_pair(-1,-2),
		make_pair(2,1),
		make_pair(2,-1),
		make_pair(-2,1),
		make_pair(-2,-1),
	};

	//First movement
	board[startingPosition.first][startingPosition.second] = 1;
	list<position> result = backtracking(tour, board, moves);
	
	printResults(result, board);
}

int main(int argc, char **argv) {

	printf("Welcome to the knight's tour resolutioner. Firstly, type de size of the square board as a number N\n");
	int size = 0;
	while (size < 5) {
		cin >> size;
		if (size < 5)
			printf("Please, the size should be at least 5\nEnter it again:\n");
	}
	cin.get();
	printf("Now type the starting position of the knight! (Example: a0 )\n");
	string startingPosition;
	cin >> startingPosition;
	cin.get();
	int x0 = startingPosition[0] - 'a';
	int y0 = startingPosition[1] - '0';
	
	clock_t t;
	t = clock();
	solve(make_pair(x0,y0), size);
	t = clock() - t;
	printf("Finished in %f seconds\n",(float)t/CLOCKS_PER_SEC);
	//Keep the console busy 
	cin.get();
	return 0;
}