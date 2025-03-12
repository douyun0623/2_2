#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

typedef pair<int, double> p;

struct save_point {
	int x;
	int y;
	int z;

	int index;
	bool exist = false;
};

// 값을 추가할 수 있는지 확인
bool can_push(vector<save_point> a) {
	for (const auto& k : a) {
		if (k.exist == false)
			return true;
	}
	return false;
}

// 값을 삭제할 수 있는지 확인
bool can_pop(vector<save_point> a) {
	for (const auto& k : a) {
		if (k.exist == true)
			return true;
	}
	return false;	// 아무것도 없는 경우
}

void print_points(const vector<save_point>& a) {
	for (const auto& k : a) {
		cout << "index : " << k.index << "	x : " << k.x << "	y : " << k.y << "	z : " << k.z << "	exist :	" << k.exist << '\n';
	}
}

int find_front_pointer(const vector<save_point>& a) {
	for (const auto& k : a) {
		if (k.exist) {
			return k.index;
		}
	}
}

int find_back_pointer(const vector<save_point>& a) {
	for (int i = a.size() - 1; i >= 0; --i){
		if (a[i].exist) {
			return a[i].index;
		}
	}
}

double distance(const vector<save_point>& p1, const int& i) {
	return sqrt(pow(p1[i].x, 2) + pow(p1[i].y, 2) + pow(p1[i].z, 2));
}

int find_max_dis(const vector<save_point>& p1) {
	double max_dis = -1;
	int save_index = 0;

	for (int i{}; i < p1.size(); ++i) {
		if (distance(p1, i) > max_dis  && p1[i].exist) {
			max_dis = distance(p1, i);
			save_index = i;
		}
	}

	return save_index;
}

int find_min_dis(const vector<save_point>& p1) {
	double max_dis = 10000;
	int save_index = 0;

	for (int i{}; i < p1.size(); ++i) {
		if (distance(p1, i) < max_dis && p1[i].exist) {
			max_dis = distance(p1, i);
			save_index = i;
		}
	}

	return save_index;
}

void initialize_dis(vector<p>& dis, const vector<save_point>& p) {
	for (int i{}; i < dis.size(); ++i) {
		if(p[i].exist){
			dis[i].first = p[i].index;
			dis[i].second = distance(p, i);
		}
		else {
			dis[i].second = -1;
		}
	}
}

// 오름차순
void sort_dis_a(vector<p>& dis) {
	sort(dis.begin(), dis.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
	if (a.second == -1)return false;
		if (b.second == -1)return true;
		return a.second < b.second;
		});
}

// 내림차순
void sort_dis_b(vector<p>& dis) {
	sort(dis.begin(), dis.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
		if (a.second == -1)return false;
		if (b.second == -1)return true;
		return a.second > b.second;
		});
}

// dis 출력
void print_dis(const vector<p>& dis) {
	for (const auto& k : dis) {
		cout << "index : " << k.first << "	distance : " << k.second << endl;
	}
}

int main()
{
	vector<save_point> sp (20);
	vector<save_point> original_sp(20);		// 원래 순서를 저장할 벡터
	vector<p> dis(20);

	for (int i{}; i < sp.size(); ++i) {
		sp[i].index = i;
	}

	int back_point = 0;
	int front_point = 0;
	int cnt_point = 0;
	
	bool select_a = false;
	bool select_s = false;

	char user_input;
	while (1) {
		print_points(sp);

		cout << "명령어를 입력하시오 : ";
		cin >> user_input;
		cout << '\n';

		switch (user_input)
		{
		case '+':
			if (can_push(sp)) {
				cout << "데이터를 입력하시오 : ";

				back_point = find_back_pointer(sp) + 1;
				cin >> sp[back_point].x >> sp[back_point].y >> sp[back_point].z;
				sp[back_point].exist = true;
			}
			else {
				cout << "추가 할 수 없다." << endl;
			}
			break;
		case '-':	
			if (can_pop(sp)) {
				back_point = find_back_pointer(sp);
				sp[back_point].exist = false;
				sp[back_point].x = sp[back_point].y = sp[back_point].z = 0;
			}
			else {
				cout << "제거 할 수 없다." << endl;
			}
			break;
		case 'e':
			if (can_push(sp)) {
				cout << "데이터를 입력하시오 : ";
				int x, y, z;
				cin >> x >> y >> z;
				
				save_point new_point = { x,y,z,0, true };

				front_point = find_front_pointer(sp);

				// 만약 front point가 0인 경우 밀어야함
				// 아닌 경우 그대로 넣으면 된다.

				if (can_pop(sp) == false) {	// 아무 것도 없는 경우
					sp[front_point] = new_point;
				}
				else {
					if (front_point == 0) {
						sp.insert(sp.begin(), new_point);
						--sp[0].index;
						sp.pop_back();

						// 리스트에 저장된 값이 밀림
						for (auto& k : sp) {
							++k.index;
						}
					}
					else {	// 아무것도 없는 경우는 고려를 안했음
						sp[front_point - 1] = new_point;
						sp[front_point - 1].index = front_point - 1;
					}
				}
				
			}
			else {
				cout << "추가 할 수 없다." << endl;
			}
			break;

		case 'd':
			if (can_pop(sp)) {
				front_point = find_front_pointer(sp);
				sp[front_point].exist = false;
				sp[front_point].x = sp[front_point].y = sp[front_point].z = 0;
			}
			else {
				cout << "제거 할 수 없다." << endl;
			}
			break;

		case 'i':
			for (const auto& k : sp) {
				if (k.exist)
					++cnt_point;
			}
			cout << "리스트에 저장된 점의 개수 : " << cnt_point << '\n';
			break;

		case 'c':
			for (auto& k : sp) {
				k.x = k.y = k.z = 0;
				k.exist = false;
			}
			break;

		case 'm':
			cout << "원점에서 가장 먼 거리에 있는 점의 좌표값을 출력한다\n";
			cout << "x : " << sp[find_max_dis(sp)].x << "	y : " << sp[find_max_dis(sp)].y << "	z : " << sp[find_max_dis(sp)].z << "\n\n";
			break;

		case 'n':
			cout << "원점에서 가장 가까운 거리에 있는 점의 좌표값을 출력한다\n";
			cout << "x : " << sp[find_min_dis(sp)].x << "	y : " << sp[find_min_dis(sp)].y << "	z : " << sp[find_min_dis(sp)].z << "\n\n";
			break;

		case 'a':
			select_a = !select_a;

			if (select_a) {
				initialize_dis(dis, sp);
				sort_dis_a(dis);
				print_dis(dis);
			}
			break;

		case 's':
			select_s = !select_s;

			if (select_s) {
				initialize_dis(dis, sp);
				sort_dis_b(dis);
				print_dis(dis);
			}
			break;

		case 'p':
			exit(0);
			break;
		default:
			break;
		}
	}
}