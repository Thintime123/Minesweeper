#include<iostream>
#include<cstdio>
#include<ctime>
#include<graphics.h>
#include<conio.h>

/*
	1，初始化map，根据数据贴图
	2，鼠标点击操作，贴图，左键和右键，连环爆炸，标记
	3，判定输（左键点到了雷），赢（除了雷以外所有格子都打开了）
	4，游戏结束，再次挑战或者退出游戏
	5，播放音乐

*/

const int ROW = 10; // 行
const int COL = 10; // 列
const int Mine_Num = 10; // 雷的数量
const int img_w = 50;
const int img_h = 50;

int map[ROW][COL]; // 地图，初始化为 0 ；
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
	SetWindowText(hwnd, "扫雷小游戏");
	int is_ok = MessageBox(hwnd, "开始游戏", "扫雷", MB_OKCANCEL);

	if (is_ok == IDOK) {
		ExMessage msg;
		while (true) {
			if (peekmessage(&msg, EM_MOUSE)) {
				bool f = false; // 游戏未结束

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
					//std::cout << "恭喜你赢了" << std::endl;
					SetWindowText(hwnd, "扫雷小游戏");
					is_ok = MessageBox(hwnd, "什么你居然赢了！敢再玩一把吗？", "扫雷", MB_OKCANCEL);
					if (is_ok == IDOK) {
						init(map);
						//ShowUpdate(map);
						draw(map);
					}
					else exit(0);
					break;
				case -1:
					//system("cls");
					//std::cout << "你输了" << std::endl;
					f = true; // 游戏结束
					break;
				default:
					break;
				}
				if (f) {
					SetWindowText(hwnd, "扫雷小游戏");
					is_ok = MessageBox(hwnd, "你怎么输了？敢再来一把吗？", "扫雷", MB_OKCANCEL);
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
//	SetWindowText(hwnd, "扫雷小游戏");
//	int is_ok = MessageBox(hwnd, "开始游戏", "扫雷", MB_OKCANCEL);
//
//	if (is_ok == IDOK) {
//		ExMessage msg;
//		while (true) {
//			if (peekmessage(&msg, EM_MOUSE)) {
//				bool f = false; // 游戏未结束
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
//					//std::cout << "恭喜你赢了" << std::endl;
//					SetWindowText(hwnd, "扫雷小游戏");
//					is_ok = MessageBox(hwnd, "什么你居然赢了！敢再玩一把吗？", "扫雷", MB_OKCANCEL);
//					if (is_ok == IDOK) {
//						init(map);
//						//ShowUpdate(map);
//						draw(map);
//					}
//					else exit(0);
//					break;
//				case -1:
//					system("cls");
//					//std::cout << "你输了" << std::endl;
//					f = true; // 游戏结束
//					break;
//				default:
//					break;
//				}
//				if (f) {
//					SetWindowText(hwnd, "扫雷小游戏");
//					is_ok = MessageBox(hwnd, "你怎么输了？敢再来一把吗？", "扫雷", MB_OKCANCEL);
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



// 初始化 map
void init(int map[ROW][COL]) {
	// 设置雷的位置(-1)
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

	// 填充其他格子的数据（每个雷的九宫格数据+1）
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

	// 加载所有图片	
	for (int i = 0; i <= 12; i++) {
		char path[30] = { 0 };
		sprintf_s(path, "./res/%d.png", i);
		loadimage(&imgs[i], path, 50, 50, false);
	}

	//初始化贴图
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			putimage(j * img_w, i * img_h, &imgs[12]);
			map[i][j] += 20; // -1 - 8 --> 19 - 28
		}
	}
}

// 打印地图
void ShowUpdate(int map[ROW][COL]) {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			printf("%3d", map[i][j]);
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;

}

// 更新贴图
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

// 鼠标操作
void Mouseopration(int map[ROW][COL], ExMessage& msg) {
	// 左键按下
	int r = msg.y / img_h, c = msg.x / img_w;
	if (msg.message == WM_LBUTTONDOWN) {
		if (map[r][c] >= -1 && map[r][c] <= 8) return; // 格子已经打开来了
		map[r][c] -= 20;
	}
	else if (msg.message == WM_RBUTTONDOWN) {
		if (map[r][c] >= 19 && map[r][c] <= 28)	// 对没有打开的格子操作
			map[r][c] += 20;
		else if (map[r][c] >= 39 && map[r][c] <= 48)
			map[r][c] -= 20;
	}
}

// 连环爆炸
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

// 判定游戏是否结束
int judge(int map[][COL]) {
	int cnt = 0; // 打开格子数
	for (int r = 0; r < ROW; r++) {
		for (int c = 0; c < COL; c++) {
			if (map[r][c] >= 0 && map[r][c] <= 8) cnt++;
			else if (map[r][c] == -1) { // 游戏失败
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
	if (cnt == ROW * COL - Mine_Num) // 游戏胜利
		return 1;
	return 0;// 游戏未结束
}