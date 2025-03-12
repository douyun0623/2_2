#include <iostream>
#include <Windows.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <utility>

using namespace std;

void print_text(vector<string>& data) {
	for (int i{}; i < data.size() - 1; ++i) {
		cout << data[i] << '\n';
	}
}

int main() 
{
	ifstream rf;
	rf.open("data.txt");

	vector<string> data{};
	vector<string> copy{};
	vector<pair<int, int>> data_line_word_count{};
	string c{};
	string find_word{};
	int word_count = 0;
	int Big_count = 0;


	int cnt_C = {};
	bool se_C = false;
	bool se_D = false;
	bool se_E = false;
	bool se_F = false;
	bool se_G = false;
	bool once = true;
	int se_R = 0;		// 0 1 2

	if (!rf.is_open()) {
		cout << "������ ���� ���߽��ϴ�" << endl;
		return 1;
	}
	else {
		cout << "������ ����" << endl;
	}

	while (getline(rf, c)) {
		data.push_back(c);
	}

	// ���
	print_text(data);

	while (1) {
		cout << "\n" << "��ɾ �Է��Ͻÿ� : ";
		char user_input;
		cin >> user_input;

		switch (user_input) {
		case 'c':	// �빮�ڷ� ���۵Ǵ� �ܾ ã�� �� �ܾ �ٸ� ������ ����ϰ� � �ִ����� ����Ͽ� ���
			se_C = !se_C;
			Big_count = 0;
			if (se_C) {
				for (int j{}; j < data.size() - 1; ++j){
					istringstream iss(data[j]);
					string word;

					while (iss >> word) {
						for (int i{}; i < word.size(); ++i) {
							if (isupper(word[i]) ) {	// �빮������ Ȯ��
								SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
								if (once) {
									++Big_count;
									once = false;
								}
							}

							if (word[i] == ' ') {	// || word[i] == '\''
								SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
							}
						}

						cout << word << ' ';

						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
						once = true;
					}

					cout << '\n';
				}

				cout << "�빮���� ���� : " << Big_count << '\n';
			}
			else {
				print_text(data);
			}

			break;

		case 'd':
			se_D = !se_D;

			if (se_D) {
				for (int i{}; i < data.size() - 1; ++i) {
					string word = data[i];

					for (int i = word.size() - 1; i >= 0; --i) {
						cout << word[i];
					}

					cout << '\n';
				}
			}
			else {
				print_text(data);
			}

			break;
		case 'e':
			se_E = !se_E;

			if (se_E) {
				for (int i{}; i < data.size() - 1; ++i) {
					string word = data[i];

					int cnt = 0;

					for (int j{}; j < word.size() - 1; ++j) {
						if (cnt > 2) {	// @ ���
							cout << '@';
						}
						else {		// �Ϲ� ���
							cout << word[j];
						}

						cnt = (cnt + 1) % 5;
					}
					cout << endl;;
				}
			}
			else {
				print_text(data);
			}

			break;
		case 'f':
			se_F = !se_F;

			if (se_F) {
				for (int j{}; j < data.size() - 1; ++j) {
					istringstream iss(data[j]);
					string word;

					while (iss >> word) {
						for (int i = word.size() - 1; i >= 0; --i) {
							cout << word[i];
						}
						cout << ' ';
					}
					cout << '\n';
				}
			}
			else {
				print_text(data);
			}

			break;
		case 'g':

			se_G = !se_G;

			if (se_G) {
				char change_word;
				char new_word;

				cout << '\n' << "�ٲ� ���ڸ� �Է��Ͻÿ�. ";
				cin >> change_word;
				cout << "���Ӱ� �Է��� ���ڸ� �Է��Ͻÿ�. ";
				cin >> new_word;

				for (int j{}; j < data.size() - 1; ++j) {
					string word = data[j];

					for (int i{}; i < word.size(); ++i) {
						if (word[i] == change_word) {
							cout << new_word;
						}
						else {
							cout << word[i];
						}
					}
					cout << '\n';
				}
			}
			else {
				print_text(data);
			}

			break;
		case 'h':

			for (int i{}; i < data.size(); ++i) {
				istringstream iss(data[i]);
				string word;
				int cnt = 0;

				while (iss >> word) {
					++cnt;
				}
				cout << i + 1 << "���� �ܾ��� ���� : " << cnt << endl;
			}

			break;
		case 'r':
			se_R = (se_R + 1) % 3;
			data_line_word_count.clear();

			for (int i{}; i < data.size(); ++i) {
				istringstream iss(data[i]);
				string word;
				int cnt = 0;

				while (iss >> word) {
					++cnt;
				}

				data_line_word_count.push_back({ i, cnt });
			}

			if (se_R == 1) {
				sort(data_line_word_count.begin(), data_line_word_count.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
					return a.second < b.second;
					});
			}
			if (se_R == 2) {
				sort(data_line_word_count.begin(), data_line_word_count.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
					return a.second > b.second;
					});
			}

			/*for (auto& a : data_line_word_count) {
				cout <<"���° �� : " << a.first << " �ܾ��� �� : " << a.second << '\n';
			}*/

			for (int i{}; i < data.size(); ++i) {
				cout << data[data_line_word_count[i].first] <<'\n';
			}

			break;
		case 's':
			word_count = 0;
			cout << "ã�� �ܾ �Է����ּ��� : ";
			cin >> find_word;

			for (int i{}; i < data.size(); ++i) {
				istringstream iss(data[i]);
				string word;

				while (iss >> word) {
					if (word == find_word) {	// �ܾ� ã��
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
						++word_count;
					}
					else {
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
					}
					cout << word << ' ';
				}
				cout << '\n';
			}

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			cout << "\n\n�ܾ��� ������ : " << word_count << endl;;
			break;

		case 'q':
			cout << "���α׷� ����" << endl;
			exit(0);
			break;

		default:
			break;
		}
	}
}