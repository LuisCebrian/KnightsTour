
#include <cstdio>
#include <utility>
#include <vector>
#include <list>
using namespace std;
using position = pair<int, int>;

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
	list<position> nextPositions = possiblePositions(moves, board, currentPosition);

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

	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board.size(); j++) {
			printf("%d ", board[i][j]);
		}
		printf("\n");
	}
	printf("%d\n", tour.size());
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

	printf("Now type the starting position x y of the knight!\n");
	int a[] = { 1,2 };
	printf("Empieza\n");
	solve(make_pair(0, 0), 5);
	printf("Acaba\n");
	//Keep the console busy 
	const int buff = 16;
	static char buffer[buff];
	fgets(buffer, buff, stdin);
	return 0;
}