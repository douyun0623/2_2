#include <iostream>	
#include <vector>	
#include <random>

using namespace std;

// �־��� ��Ŀ��� Ư�� ��� ���� ������ ������� ��ȯ
vector<vector<int>> getMinor(const vector<vector<int>>& mat, int row, int col) {
	vector<vector<int>> minor;
	for (int i = 0; i < mat.size(); i++) {
		if (i == row) continue;
		vector<int> minorRow;
		for (int j = 0; j < mat[i].size(); j++) {
			if (j == col) continue;
			minorRow.push_back(mat[i][j]);
		}
		minor.push_back(minorRow);
	}
	return minor;
}

// �־��� ����� ��Ľ� ���
int determinant(const vector<vector<int>>& mat) {
	int size = mat.size();

	// 1x1 ����� ���
	if (size == 1) return mat[0][0];

	// 2x2 ����� ���
	if (size == 2) {
		return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
	}

	// 3x3 �̻� ����� ���
	int det = 0;
	for (int i = 0; i < size; i++) {
		// ������� ���� �� ��������� ��Ľ� ���
		vector<vector<int>> minor = getMinor(mat, 0, i);
		int sign = (i % 2 == 0) ? 1 : -1;
		det += sign * mat[0][i] * determinant(minor);
	}
	return det;
}

void print_pro(vector<vector<int>>, vector<vector<int>>);
void print_pro(vector<vector<int>>);
void _num(vector<vector<int>>& pro1, vector<vector<int>>& pro2, int num);

int main() 
{
	srand(unsigned(time(NULL)));

	vector<vector<int>> pro_1(4, vector<int>(4, 0));
	vector<vector<int>> pro_2(4, vector<int>(4, 0));
	vector<vector<int>> pro_3(4, vector<int>(4, 0));	// ��� 
	vector<int> v;

	// 2���� cnt
	int count_1 = 0;
	int count_2 = 0; 

	// e cnt;
	int count_e = 0;

	int det_pro_1 = 0;
	int det_pro_2 = 0;

	// deter

	// �ʱ� �� ����
	for (int i{}; i < 4; ++i) {
		for (int j{}; j < 4; ++j) {
			int temp1;
			int temp2;

			// count 1
			if (count_1 == 2) {
				temp1 = rand() % 2;
			}
			else {
				temp1 = rand() % 3;
				++count_1;
			}

			// count 2
			if (count_2 == 2) {
				temp2 = rand() % 2;
			}
			else {
				temp2 = rand() % 3;
				++count_2;
			}

			pro_1[i][j] = temp1;
			pro_2[i][j] = temp2;
		}
	}


	// �⺻ ��� (1,2)
	print_pro(pro_1, pro_2);
	// ��� ���
	

	while (1) {
		cout << '\n' << "��ɾ �Է����ּ��� : ";
		// ��ɾ� �Է�
		char com;
		cin >> com;
		cout << endl;

		// ��ɾ� ����
		switch (com)
		{
			// ����� ����
		case 'm':
			for (int i{}; i < 4; ++i) {
				for (int j{}; j < 4; ++j) {
					int sum = 0;
					for (int k{}; k < 4; ++k) {
						pro_3[i][j] += pro_1[i][k] * pro_2[k][j];
					}
				}
			}
			print_pro(pro_3);
			break;

			// ����� ����
		case 'a':
			for (int i{}; i < 4; ++i) {
				for (int j{}; j < 4; ++j) {
					pro_3[i][j] = pro_1[i][j] + pro_2[i][j];
				}
			}
			print_pro(pro_3);
			break;

			// ����
		case 'd':
			for (int i{}; i < 4; ++i) {
				for (int j{}; j < 4; ++j) {
					pro_3[i][j] = pro_1[i][j] - pro_2[i][j];
				}
			}
			print_pro(pro_3);
			break;

			// ��Ľ��� ��
		case 'r':
			det_pro_1 = determinant(pro_1);
			det_pro_2 = determinant(pro_2);

			cout << "��� 1�� ��Ľ� : " << det_pro_1 << endl;
			cout << "��� 2�� ��Ľ� : " << det_pro_2 << endl;
			break;

			// ��ġ
		case 't':
			for (int i = 0; i < 3; ++i) {
				for (int j = 1 + i; j < 4; ++j) {
					int temp1 = pro_1[i][j];
					pro_1[i][j] = pro_1[j][i];
					pro_1[j][i] = temp1;

					int temp2 = pro_2[i][j];
					pro_2[i][j] = pro_2[j][i];
					pro_2[j][i] = temp2;
				}
			}
			print_pro(pro_1, pro_2);
			break;

			// ¦����, Ȧ����, �������
		case 'e':
			count_e = (count_e + 1) % 3;
			switch (count_e)
			{
			case 1:
				for (int i{}; i < 4; ++i) {
					for (int j{}; j < 4; ++j) {
						if (pro_1[i][j] % 2 == 0) {
							cout << pro_1[i][j] << ' ';
						}
						else {
							cout << ". ";
						}
					}

					cout << '\t';

					for (int j{}; j < 4; ++j	){
						if (pro_2[i][j] % 2 == 0) {
							cout << pro_2[i][j] << ' ';
						}
						else {
							cout << ". ";
						}
					}
					cout << '\n';
				}
				break;
			case 2:
				for (int i{}; i < 4; ++i) {
					for (int j{}; j < 4; ++j) {
						if (pro_1[i][j] % 2 == 1) {
							cout << pro_1[i][j] << ' ';
						}
						else {
							cout << ". ";
						}
					}

					cout << '	';

					for (int j{}; j < 4; ++j) {
						if (pro_2[i][j] % 2 == 1) {
							cout << pro_2[i][j] << ' ';
						}
						else {
							cout << ". ";
						}
					}
					cout << '\n';
				}
				break;
			case 0:
				print_pro(pro_1, pro_2);
				break;
			}
			break;

			// �����ϰ� ����
		case 's':
			count_1 = 0;
			count_2 = 0;
			for (int i{}; i < 4; ++i) {
				for (int j{}; j < 4; ++j) {
					int temp1;
					int temp2;

					// count 1
					if (count_1 == 2) {
						temp1 = rand() % 2;
					}
					else {
						temp1 = rand() % 3;
						++count_1;
					}

					// count 2
					if (count_2 == 2) {
						temp2 = rand() % 2;
					}
					else {
						temp2 = rand() % 3;
						++count_2;
					}

					pro_1[i][j] = temp1;
					pro_2[i][j] = temp2;
				}
			}
			print_pro(pro_1, pro_2);
			break;

		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			_num(pro_1, pro_2, com - '0');
			print_pro(pro_1, pro_2);
			break;
		case 'q':
			exit(0);
			break;

		default:
			cout << "�ٽ� �Է����ּ���. ";
			break;
		}
	}
}

// ��� ��� �Լ�(1,2)
void print_pro(vector<vector<int>> pro1, vector<vector<int>> pro2) {
	for (int i{}; i < 4; ++i) {
		// ��� 1 ���
		for (int j{}; j < 4; ++j) {
			cout << pro1[i][j] << ' ';
		}

		cout << '\t';

		// ��� 2 ���
		for (int j{}; j < 4; ++j) {
			cout << pro2[i][j] << ' ';
		}

		cout << '\n';
	}
}
void print_pro(vector<vector<int>> pro1) {
	for (int i{}; i < 4; ++i) {
		for (int j{}; j < 4; ++j) {
			cout << pro1[i][j] << ' ';
		}
		cout << '\n';
	}
}

// ��Į�� ��
void _num(vector<vector<int>>& pro1, vector<vector<int>>& pro2, int num) {
	for (int i{}; i < 4; ++i) {
		for (int j{}; j < 4; ++j) {
			pro1[i][j] *= num;
			pro2[i][j] *= num;
		}
	}
}