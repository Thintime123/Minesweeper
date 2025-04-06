#include<iostream>
#include<cstdio>
#include<ctime>
#include<graphics.h>
#include<conio.h>

/*
	1����ʼ��map������������ͼ
	2���������������ͼ��������Ҽ���������ը�����
	3���ж��䣨����㵽���ף���Ӯ���������������и��Ӷ����ˣ�
	4����Ϸ�������ٴ���ս�����˳���Ϸ
	5����������

*/

const int ROW = 10; // ��
const int COL = 10; // ��
const int Mine_Num = 10; // �׵�����
const int img_w = 50;
const int img_h = 50;

int map[ROW][COL]; // ��ͼ����ʼ��Ϊ 0 ��
IMAGE imgs[13];

void init(int map[ROW][COL]);
void ShowUpdate(int map[ROW][COL]);
void draw(int map[][COL]);
void Mouseopration(int map[ROW][COL], ExMessage& msg);
void ZeroBoom(int map[][COL], int row, int col);
int judge(int map[][COL]);


#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	initgraph(COL * img_w, ROW * img_h);
	init(map);
	//ShowUpdate(map);

	HWND hwnd = GetHWnd();
	SetWindowText(hwnd, "ɨ��С��Ϸ");
	int is_ok = MessageBox(hwnd, "��ʼ��Ϸ", "ɨ��", MB_OKCANCEL);

	if (is_ok == IDOK) {
		ExMessage msg;
		while (true) {
			if (peekmessage(&msg, EM_MOUSE)) {
				bool f = false; // ��Ϸδ����

				switch (msg.message)
				{
				case WM_LBUTTONDOWN:
				case WM_RBUTTONDOWN:
					Mouseopration(map, msg);
					//system("cls");
					//ShowUpdate(map);
					draw(map);
					break;
				default:
					break;
				}
				int res = judge(map);
				switch (res)
				{
				case 1:
					//std::cout << "��ϲ��Ӯ��" << std::endl;
					SetWindowText(hwnd, "ɨ��С��Ϸ");
					is_ok = MessageBox(hwnd, "ʲô���ȻӮ�ˣ�������һ����", "ɨ��", MB_OKCANCEL);
					if (is_ok == IDOK) {
						init(map);
						//ShowUpdate(map);
						draw(map);
					}
					else exit(0);
					break;
				case -1:
					//system("cls");
					//std::cout << "������" << std::endl;
					f = true; // ��Ϸ����
					break;
				default:
					break;
				}
				if (f) {
					SetWindowText(hwnd, "ɨ��С��Ϸ");
					is_ok = MessageBox(hwnd, "����ô���ˣ�������һ����", "ɨ��", MB_OKCANCEL);
					if (is_ok == IDOK) {
						init(map);
						//ShowUpdate(map);
						draw(map);
					}
					else {
						closegraph();
						exit(0);
					}
				}
			}
		}
		_getch();
	}
	else {
		closegraph();
		exit(0);
	}
	return 0;
}


//int main() {
//	initgraph(COL * img_w, ROW * img_h);
//	init(map);
//	//ShowUpdate(map);
//
//	HWND hwnd = GetHWnd();
//	SetWindowText(hwnd, "ɨ��С��Ϸ");
//	int is_ok = MessageBox(hwnd, "��ʼ��Ϸ", "ɨ��", MB_OKCANCEL);
//
//	if (is_ok == IDOK) {
//		ExMessage msg;
//		while (true) {
//			if (peekmessage(&msg, EM_MOUSE)) {
//				bool f = false; // ��Ϸδ����
//
//				switch (msg.message)
//				{
//				case WM_LBUTTONDOWN:
//				case WM_RBUTTONDOWN:
//					Mouseopration(map, msg);
//					system("cls");
//					//ShowUpdate(map);
//					draw(map);
//					break;
//				default:
//					break;
//				}
//				int res = judge(map);
//				switch (res)
//				{
//				case 1:
//					//std::cout << "��ϲ��Ӯ��" << std::endl;
//					SetWindowText(hwnd, "ɨ��С��Ϸ");
//					is_ok = MessageBox(hwnd, "ʲô���ȻӮ�ˣ�������һ����", "ɨ��", MB_OKCANCEL);
//					if (is_ok == IDOK) {
//						init(map);
//						//ShowUpdate(map);
//						draw(map);
//					}
//					else exit(0);
//					break;
//				case -1:
//					system("cls");
//					//std::cout << "������" << std::endl;
//					f = true; // ��Ϸ����
//					break;
//				default:
//					break;
//				}
//				if (f) {
//					SetWindowText(hwnd, "ɨ��С��Ϸ");
//					is_ok = MessageBox(hwnd, "����ô���ˣ�������һ����", "ɨ��", MB_OKCANCEL);
//					if (is_ok == IDOK) {
//						init(map);
//						//ShowUpdate(map);
//						draw(map);
//					}
//					else {
//						closegraph();
//						exit(0);
//					}
//				}
//			}
//		}
//		_getch();
//	}
//	else {
//		closegraph();
//		exit(0);
//	}
//	return 0;
//}



// ��ʼ�� map
void init(int map[ROW][COL]) {
	// �����׵�λ��(-1)
	srand((unsigned)time(NULL));

	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			map[i][j] = 0;
		}
	}

	for (int i = 0; i < Mine_Num; i++) {
		int r = rand() % ROW;
		int c = rand() % COL;
		if (map[r][c] != -1)
			map[r][c] = -1;
		else i--;
	}

	// ����������ӵ����ݣ�ÿ���׵ľŹ�������+1��
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (map[i][j] == -1) {
				for (int r = i - 1; r <= i + 1; r++) {
					if (r < 0 || r >= ROW) continue;
					for (int c = j - 1; c <= j + 1; c++) {
						if (c < 0 || c >= COL) continue;
						if (map[r][c] != -1) map[r][c]++;
					}
				}
			}
		}
	}

	// ��������ͼƬ	
	for (int i = 0; i <= 12; i++) {
		char path[30] = { 0 };
		sprintf_s(path, "./res/%d.png", i);
		loadimage(&imgs[i], path, 50, 50, false);
	}

	//��ʼ����ͼ
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			putimage(j * img_w, i * img_h, &imgs[12]);
			map[i][j] += 20; // -1 - 8 --> 19 - 28
		}
	}
}

// ��ӡ��ͼ
void ShowUpdate(int map[ROW][COL]) {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			printf("%3d", map[i][j]);
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;

}

// ������ͼ
void draw(int map[][COL]) {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (map[i][j] >= 0 && map[i][j] <= 8) {  //  -1 - 8 --> 19 - 28
				putimage(j * img_w, i * img_h, &imgs[map[i][j]]);
				if (map[i][j] == 0) {
					putimage(j * img_w, i * img_h, &imgs[0]);
					ZeroBoom(map, i, j);
				}
			}
			else if (map[i][j] == -1) {
				putimage(j * img_w, i * img_h, &imgs[10]);
			}
			else if (map[i][j] >= 19 && map[i][j] <= 28) {
				putimage(j * img_w, i * img_h, &imgs[12]);
			}
			else if (map[i][j] >= 39 && map[i][j] <= 48) {
				putimage(j * img_w, i * img_h, &imgs[9]);
			}
			/*else if (map[i][j] == 0) {
			}*/
		}
	}
}

// ������
void Mouseopration(int map[ROW][COL], ExMessage& msg) {
	// �������
	int r = msg.y / img_h, c = msg.x / img_w;
	if (msg.message == WM_LBUTTONDOWN) {
		if (map[r][c] >= -1 && map[r][c] <= 8) return; // �����Ѿ�������
		map[r][c] -= 20;
	}
	else if (msg.message == WM_RBUTTONDOWN) {
		if (map[r][c] >= 19 && map[r][c] <= 28)	// ��û�д򿪵ĸ��Ӳ���
			map[r][c] += 20;
		else if (map[r][c] >= 39 && map[r][c] <= 48)
			map[r][c] -= 20;
	}
}

// ������ը
void ZeroBoom(int map[][COL], int row, int col) {
	for (int i = row - 1; i <= row + 1; i++) {
		if (i < 0 || i >= ROW) continue;
		for (int j = col - 1; j <= col + 1; j++) {
			if (j < 0 || j >= COL) continue;
			if (map[i][j] >= 19 && map[i][j] <= 28) {
				map[i][j] -= 20;
				//system("cls");
				//ShowUpdate(map);
				draw(map);
				if (map[i][j] == 0)
					ZeroBoom(map, i, j);
			}
		}
	}
}

// �ж���Ϸ�Ƿ����
int judge(int map[][COL]) {
	int cnt = 0; // �򿪸�����
	for (int r = 0; r < ROW; r++) {
		for (int c = 0; c < COL; c++) {
			if (map[r][c] >= 0 && map[r][c] <= 8) cnt++;
			else if (map[r][c] == -1) { // ��Ϸʧ��
				for (int i = 0; i < ROW; i++) {
					for (int j = 0; j < COL; j++) {
						if (map[i][j] >= 19 && map[i][j] <= 28) {
							map[i][j] -= 20;
							if (map[i][j] != -1) {
								putimage(j * img_w, i * img_h, &imgs[map[i][j]]);
							}
							else putimage(j * img_w, i * img_h, &imgs[11]);
						}
						else if (map[i][j] > 28) {
							map[i][j] -= 40;
							if (map[i][j] != -1) {
								putimage(j * img_w, i * img_h, &imgs[map[i][j]]);
							}
							else putimage(j * img_w, i * img_h, &imgs[11]);
						}
					}
				}
				return -1;
			}
		}
	}
	if (cnt == ROW * COL - Mine_Num) // ��Ϸʤ��
		return 1;
	return 0;// ��Ϸδ����
}