#include <iostream>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <cctype>
#include <cstdlib>

using namespace std;

char r_board[30][30];

class draw_shape {
private:
	char input_order;

	int shape_area;

	bool once = true;

public:
	int x1, x2;
	int y1, y2;

	char board[30][30];

	draw_shape();
	~draw_shape();
	void reset();

	void print_board();
	void input_point();

	void draw_rect();

	void move_shape(char);
	void size_change_shape(char);

	void play_draw_shape();
};

// ������
draw_shape::draw_shape()
{
	reset();
}

draw_shape::~draw_shape()
{

}

// ������ �ʱ�ȭ
void draw_shape::reset()
{
	// ���� ��� for���� ����� �ʱ�ȭ
	for (auto& row : board) {
		for (auto& col : row) {
			col = '.';
		}
	}
}

// board ���
void draw_shape::print_board()
{
	for (const auto& row : board) {
		for (const auto& col : row) {
			cout << col << ' ';
		}
		cout << endl;
	}
}

// ��ǥ �Է�
void draw_shape::input_point()
{
	cout << "��ǥ�� �Է��Ͻÿ� : ";
	cin >> x1 >> y1 >> x2 >> y2;

	if (x1 > size(board[30]) || x2 > size(board[30])
		|| y1 > size(board) || y2 > size(board)) {
		system("cls");
		cout << "�ٽ� �Է��Ͻÿ�. " << endl;
		input_point();
	}

	once = true;
	draw_rect();
}

// �ش� ��ǥ�� �簢�� ����
void draw_shape::draw_rect()
{
	reset();

	// ó�� �簢���� �׸����� ũ�� ������ ���� 1�� ������ 2�� ū�ɷ�
	if (once) {
		x1 = min(x1, x2), x2 = max(x1, x2);
		y1 = min(y1, y2), y2 = max(y1, y2);

		once = false;
	}

	// �Ѿ ��Ȳ
	if (x1 > x2 && y1 > y2) {
		for (int i{}; i <= y2; ++i) {
			for (int j{}; j <= x2; ++j) {
				board[i][j] = '0';
			}

			for (int j = x1; j <= 29; ++j) {
				board[i][j] = '0';
			}
		}

		for (int i = y1; i <= 29; ++i) {
			for (int j{}; j <= x2; ++j) {
				board[i][j] = '0';
			}

			for (int j = x1; j <= 29; ++j) {
				board[i][j] = '0';
			}
		}
		return;
	}

	if (x1 > x2) {
		for (int i = y1; i <= y2; ++i) {
			for (int j{}; j <= x2; ++j) {
				board[i][j] = '0';
			}

			for (int j = x1; j <= 29; ++j) {
				board[i][j] = '0';
			}
		}
		return;
	}

	if (y1 > y2) {
		for (int i = x1; i <= x2; ++i) {
			for (int j{}; j <= y2; ++j) {
				board[j][i] = '0';
			}

			for (int j = y1; j <= 29; ++j) {
				board[j][i] = '0';
			}
		}
		return;
	}

	// �Ϲ����� ��Ȳ
	for (int i = y1; i <= y2; ++i) {
		for (int j = x1; j <= x2; ++j) {
			board[i][j] = '0';
		}
	}
}

// ���� �����̱�
void draw_shape::move_shape(char user_input)
{
	switch (user_input)
	{
	case 'x':	// ��
		x1 = (x1 + 1) % 30;
		x2 = (x2 + 1) % 30;
		break;
	case 'X':	// ��
		x1 = (x1 - 1 + 30) % 30;
		x2 = (x2 - 1 + 30) % 30;
		break;
	case 'y':	// ��
		y1 = (y1 + 1) % 30;
		y2 = (y2 + 1) % 30;
		break;
	case 'Y':	// ��
		y1 = (y1 - 1 + 30) % 30;
		y2 = (y2 - 1 + 30) % 30;
		break;
	}

	draw_rect();
}

// ũ�� ����
void draw_shape::size_change_shape(char user_input)
{
	switch (user_input)
	{
	case 's':		// xy ���
		if (!(x2 - 1 == x1 || y2 - 1 == y1)) {
			--x2;
			--y2;
			return;
		}
		else {
			cout << "������ �Ѿ�ϴ�. ũ�⸦ ������ �� �����ϴ�. " << endl;
		}
		break;
	case 'S':		// xy Ȯ��
		if (!(x2 + 1 > 29 || y2 + 1 > 29)) {
			++x2;
			++y2;
			return;
		}
		else {
			cout << "������ �Ѿ�ϴ�. ũ�⸦ ������ �� �����ϴ�. " << endl;
		}
		break;
	}
}

// ����
void draw_shape::play_draw_shape()
{
	input_point();

	while (true)
	{
		print_board();

		cout << "��ɾ �Է��Ͻÿ� : ";
		cin >> input_order;
		cout << endl;

		system("cls");

		switch (input_order)
		{
			// ���� ����
		case 'x':
		case 'X':
		case 'y':
		case 'Y':
			move_shape(input_order);
			break;

			// ũ�� ����
		case 's':
		case 'S':
		case 'i':
		case 'I':
		case 'j':
		case 'J':
		case 'a':
		case 'A':
			size_change_shape(input_order);
			draw_rect();
			break;

		case 'r':
			reset();
			system("cls");
			cout << "���µǾ����ϴ�. " << endl;
			input_point();
			break;
		case 'q':
			exit(0);
			break;
		default:
			break;
		}
	}
}

// �÷� ���
void textcolor(int foreground, int background)
{
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// board ���
void print_board();

draw_shape p1, p2;

int main()
{
	p1.input_point();
	p2.input_point();

	p1.draw_rect();
	p2.draw_rect();

	char input;

	while (true)
	{
		system("cls");

		print_board();

		cout << "��ɾ� �Է� : ";
		cin >> input;


		cout << "\n";

		switch (input)
		{
			// p1 ������
		case 'x':
		case 'X':
		case 'y':
		case 'Y':
			p1.move_shape(input);
			break;

			// p2 ������
		case 'w':
			p2.move_shape('x');
			break;
		case 'W':
			p2.move_shape('X');
			break;
		case 'a':
			p2.move_shape('y');
			break;
		case 'A':
			p2.move_shape('Y');
			break;

			// p1 (������)
		case 's':		// xy ��� ���
		case 'S':		// Ȯ��
			p1.size_change_shape(input);
			p1.draw_rect();
			break;
			
			// p2
		case 'd':		// xy ��� ��� 
			p2.size_change_shape('s');
			p2.draw_rect();
			break;
		case 'D':		// Ȯ��
			p2.size_change_shape('S');
			p2.draw_rect();
			break;

			// reset
		case 'r':
			break;

		case 'q':
			exit(0);
			break;
		}
	}
}


// board ���
void print_board()
{
	for (int i{}; i < 30; ++i) {
		for (int j{}; j < 30; ++j) {
			if (p1.board[i][j] != '.' && p2.board[i][j] != '.') {
				textcolor(1, 0);
				cout << '@' << ' ';
				textcolor(15, 0);
				continue;
			}
			if (p1.board[i][j] != '.') {
				cout << '#' << ' ';
				continue;
			}
			if (p2.board[i][j] != '.') {
				cout << '0' << ' ';
				continue;
			}

			else {
				cout << '.' << ' ';
			}
		}
		cout << '\n';
	}

	cout << endl;
}