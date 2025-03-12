#include <iostream>	
#include <Windows.h>
#include <random>

using namespace std;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dis(1, 5);	//dis(gen)
std::uniform_int_distribution<int> dis2(1, 5);	//dis(gen)



#define BLACK 0
#define BLUE 1
#define GREEN 2
#define WHITE 15

void textcolor(int foreground,int background)
{
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

class Find_directions {
private:
	char board[30][30];


public:
	void reset_board();
	void make_ways();
	void print_board();

	void game_start();
};

// 보드판 초기화
void Find_directions::reset_board()
{
	for (auto& row : board) {
		for (auto& col : row) {
			col = '1';
		}
	}
}

// 보드판 길 만들기
void Find_directions::make_ways()
{
	int x = 0, y = 0;
	board[x][y] = '0';

	int length = dis(gen);		// 1~5
	
}

// 보드판 출력
void Find_directions::print_board()
{
	for (auto& row : board) {
		for (auto& col : row) {
				
			textcolor(BLUE, BLACK);
			cout << col << ' ';
			
		}
		cout << endl;
	}

	textcolor(WHITE, BLACK);
}

void Find_directions::game_start()
{
	reset_board();

	print_board();
}

int main()
{
	Find_directions fd;

	fd.game_start();
}