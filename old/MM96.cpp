#include "vector"
#include "iostream"
#include "string"
#include "algorithm"
#include "stack"

using namespace std;

class GarlandOfLights {
	int dir_x[12] = { 0,1,-1,0,-1,0,0,1,-1,1,0,0 };
	int dir_y[12] = { 1,0,0,1,0,-1,-1,0,0,0,-1,1 };

	pair<vector<vector<int>>, int> search(int H, int W, int yoko, int amari, int tate_minus, int best, vector<string>& lights, vector<vector<vector<int>>>box, vector<int>num) {
		vector<vector<int>>ret(H, vector<int>(W, -1));
		vector<vector<int>>flag(H, vector<int>(W));
		vector<vector<int>>index(6, vector<int>(4));
		vector<vector<int>>kind(H, vector<int>(W, -1));
		vector<int>used(6);
		used[5] += tate_minus;
		//yoko
		for (int i = 0; i < yoko; i += 2) {
			kind[H - 1 - i][0] = 3;
			kind[H - 1 - i][W - 2] = 1;
			used[3]++;
			used[1]++;
		}
		for (int i = 1; i < yoko; i += 2) {
			kind[H - 1 - i][0] = 0;
			kind[H - 1 - i][W - 2] = 2;
			used[0]++;
			used[2]++;
		}
		for (int i = 1; i < W - 1; i++) {
			kind[H - 1][i] = 4;
			used[4]++;
		}
		used[1]--;
		for (int i = 1; i < yoko; i++) {
			for (int j = 1; j < W - 2; j++) {
				kind[H - 1 - i][j] = 4;
				used[4]++;
			}
		}
		kind[H - 1][W - 1] = 2;
		used[2]++;
		for (int i = 1; i < yoko; i++) {
			kind[H - i - 1][W - 1] = 5;
			used[5]++;
		}
		//uneune
		kind[H - yoko - 1][0] = 0;
		used[0]++;
		kind[H - yoko - 1][W - 1] = 1;
		used[1]++;
		for (int i = 1; i < W - 1; i++) {
			kind[H - yoko - 1][i] = 4;
			used[4]++;
		}
		int nx = 0;
		for (int i = 0; i + 3 < W; i += 4) {
			if (num[0] - used[0] - 2 < amari) {
				break;
			}
			if (num[1] - used[1] - 2 < amari) {
				break;
			}
			if (num[2] - used[2] - 2 < amari) {
				break;
			}
			if (num[3] - used[3] - 2 < amari) {
				break;
			}
			for (int j = H - yoko - 1; j >= 2; j -= 2) {
				if (num[0] - used[0] - 2 < amari) {
					break;
				}
				if (num[1] - used[1] - 2 < amari) {
					break;
				}
				if (num[2] - used[2] - 2 < amari) {
					break;
				}
				if (num[3] - used[3] - 2 < amari) {
					break;
				}
				nx = i + 4;
				used[0] += 2;
				used[1] += 2;
				used[2] += 2;
				used[3] += 2;
				kind[j][i + 1] = 2;
				kind[j][i + 2] = 3;
				kind[j - 1][i] = 3;
				kind[j - 1][i + 1] = 1;
				kind[j - 1][i + 2] = 0;
				kind[j - 1][i + 3] = 2;
				kind[j - 2][i] = 0;
				kind[j - 2][i + 1] = 4;
				kind[j - 2][i + 2] = 4;
				kind[j - 2][i + 3] = 1;
			}
		}
		for (int i = W - 2; i >= nx; i -= 2) {
			if (used[0] == num[0]) {
				break;
			}
			if (used[1] == num[1]) {
				break;
			}
			if (used[2] == num[2]) {
				break;
			}
			if (used[3] == num[3]) {
				break;
			}
			kind[H - yoko - 1][i] = 2;
			kind[H - yoko - 1][i + 1] = 3;
			used[2]++;
			used[3]++;
			used[4] -= 2;
			int y = H - yoko - 2;
			while (y&&num[5] - used[5] > 2) {
				kind[y][i] = 5;
				kind[y][i + 1] = 5;
				used[5] += 2;
				y--;
			}
			kind[y][i] = 0;
			kind[y][i + 1] = 1;
			used[0]++;
			used[1]++;
		}
		if (kind[H - yoko - 1][W - 1] == 3) {
			used[3]--;
			kind[H - yoko - 1][W - 1] = 5;
			used[5]++;
		}
		int score = 0;
		used[5] -= tate_minus;
		for (int i = 0; i < 6; i++) {
			score += used[i];
		}
		if (score <= best) {
			return{ vector<vector<int>>(0), score };
		}
		int y = H - 1;
		int x = 0;
		stack<pair<int, int>>S;
		S.push({ y,x });
		flag[y][x] = 1;
		while (!S.empty()) {
			int cy = S.top().first;
			int cx = S.top().second;
			int ck = kind[cy][cx];
			flag[cy][cx] = 2;
			S.pop();
			vector<bool>can(4, true);
			for (int i = 0; i < 2; i++) {
				int ny = cy + dir_y[ck * 2 + i];
				int nx = cx + dir_x[ck * 2 + i];
				if (flag[ny][nx] == 0) {
					S.push({ ny,nx });
					flag[ny][nx] = 1;
				}
				else if (flag[ny][nx] == 2) {
					can[(lights[ret[ny][nx]][1] - 'a')] = false;
				}
			}
			int nc = -1;
			for (int i = 0; i < 4; i++) {
				if (can[i]) {
					if (nc == -1) {
						if (box[ck][i].size() - index[ck][i]) {
							nc = i;
						}
					}
					else if (box[ck][nc].size() - index[ck][nc]<box[ck][i].size() - index[ck][i] && box[ck][i].size() - index[ck][i]) {
						nc = i;
					}
				}
			}
			if (nc == -1) {
				return{ vector<vector<int>>(0),0 };
			}
			ret[cy][cx] = box[ck][nc][index[ck][nc]];
			index[ck][nc]++;
			if (box[ck][nc].size() < index[ck][nc]) {
				return{ vector<vector<int>>(0),0 };
			}
		}
		stack<int>ex;
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				while (box[i][j].size() > index[i][j]) {
					ex.push(box[i][j][index[i][j]]);
					index[i][j]++;
				}
			}
		}
		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				if (ret[i][j] == -1) {
					ret[i][j] = ex.top();
					ex.pop();
				}
			}
		}
		for (int i = 0; i < 6; i++) {
			cerr << i << " " << used[i] << " ";
		}
		cerr << endl;
		return{ ret,score };
	}
	pair<vector<vector<int>>, int> search3ee(int H, int W, int yoko, int amari, int tate_minus, int best, vector<string>& lights, vector<vector<vector<int>>>box, vector<int>num) {
		vector<vector<int>>ret(H, vector<int>(W, -1));
		vector<vector<int>>flag(H, vector<int>(W));
		vector<vector<int>>index(6, vector<int>(4));
		vector<vector<int>>kind(H, vector<int>(W, -1));
		vector<int>used(6);
		used[5] += tate_minus;
		//yoko
		for (int i = 0; i < yoko; i += 2) {
			kind[H - 1 - i][0] = 3;
			kind[H - 1 - i][W - 2] = 1;
			used[3]++;
			used[1]++;
		}
		for (int i = 1; i < yoko; i += 2) {
			kind[H - 1 - i][0] = 0;
			kind[H - 1 - i][W - 2] = 2;
			used[0]++;
			used[2]++;
		}
		for (int i = 1; i < W - 1; i++) {
			kind[H - 1][i] = 4;
			used[4]++;
		}
		used[1]--;
		for (int i = 1; i < yoko; i++) {
			for (int j = 1; j < W - 2; j++) {
				kind[H - 1 - i][j] = 4;
				used[4]++;
			}
		}
		kind[H - 1][W - 1] = 2;
		used[2]++;
		for (int i = 1; i < yoko; i++) {
			kind[H - i - 1][W - 1] = 5;
			used[5]++;
		}
		//uneune
		kind[H - yoko - 1][0] = 0;
		used[0]++;
		kind[H - yoko - 1][W - 1] = 1;
		used[1]++;
		for (int i = 1; i < W - 1; i++) {
			kind[H - yoko - 1][i] = 4;
			used[4]++;
		}
		int nx = 0;
		for (int i = 0; i + 3 < W; i += 4) {
			if (num[0] - used[0] - 2 < amari) {
				break;
			}
			if (num[1] - used[1] - 2 < amari) {
				break;
			}
			if (num[2] - used[2] - 2 < amari) {
				break;
			}
			if (num[3] - used[3] - 2 < amari) {
				break;
			}
			for (int j = H - yoko - 1; j >= 2; j -= 2) {
				if (num[0] - used[0] - 2 < amari) {
					break;
				}
				if (num[1] - used[1] - 2 < amari) {
					break;
				}
				if (num[2] - used[2] - 2 < amari) {
					break;
				}
				if (num[3] - used[3] - 2 < amari) {
					break;
				}
				nx = i + 4;
				used[0] += 2;
				used[1] += 2;
				used[2] += 2;
				used[3] += 2;
				kind[j][i + 1] = 2;
				kind[j][i + 2] = 3;
				kind[j - 1][i] = 3;
				kind[j - 1][i + 1] = 1;
				kind[j - 1][i + 2] = 0;
				kind[j - 1][i + 3] = 2;
				kind[j - 2][i] = 0;
				kind[j - 2][i + 1] = 4;
				kind[j - 2][i + 2] = 4;
				kind[j - 2][i + 3] = 1;
			}
		}
		for (int i = W-2; i >=nx; i -= 2) {
			if (used[0] == num[0]) {
				break;
			}
			if (used[1] == num[1]) {
				break;
			}
			if (used[2] == num[2]) {
				break;
			}
			if (used[3] == num[3]) {
				break;
			}
			kind[H - yoko - 1][i] = 2;
			kind[H - yoko - 1][i + 1] = 3;
			used[2]++;
			used[3]++;
			used[4] -= 2;
			int y = H - yoko - 2;
			while (y&&num[5] - used[5] > 2) {
				kind[y][i] = 5;
				kind[y][i + 1] = 5;
				used[5] += 2;
				y--;
			}
			kind[y][i] = 0;
			kind[y][i + 1] = 1;
			used[0]++;
			used[1]++;
		}
		if (kind[H - yoko - 1][W - 1] == 3) {
			used[3]--;
			kind[H - yoko - 1][W - 1] = 5;
			used[5]++;
		}
		if (num[5] - used[5] > 0) {
			int box_y = -1;
			for (int i = 0; i < H; i++) {
				if (kind[i][nx - 1] != -1) {
					box_y = i;
					break;
				}
			}
			if (num[0] - used[0] > 0 && num[2] - used[2] > 0 && box_y) {
				kind[box_y][nx - 2] = 2;
				kind[box_y][nx - 1] = 5;
				kind[box_y - 1][nx - 2] = 0;
				kind[box_y - 1][nx - 1] = 1;
				used[0]++;
				used[2]++;
				used[5]++;
				used[4]--;
				for (int i = box_y - 1; i&&num[5] - used[5]>=2; i--) {
					kind[i - 1][nx - 2] = kind[i][nx - 2];
					kind[i - 1][nx - 1] = kind[i][nx - 1];
					kind[i][nx - 2] = 5;
					kind[i][nx - 1] = 5;
					used[5] += 2;
				}
			}
			if (num[1] - used[1] > 0 && num[3] - used[3] > 0 && num[5] - used[5] > 0 && box_y) {
				kind[box_y][nx - 4] = 5;
				kind[box_y][nx - 3] = 3;
				kind[box_y - 1][nx - 4] = 0;
				kind[box_y - 1][nx - 3] = 1;
				used[1]++;
				used[3]++;
				used[5]++;
				used[4]--;
				for (int i = box_y - 1; i&&num[5] - used[5] >= 2; i--) {
					kind[i - 1][nx - 4] = kind[i][nx - 4];
					kind[i - 1][nx - 3] = kind[i][nx - 3];
					kind[i][nx - 4] = 5;
					kind[i][nx - 3] = 5;
					used[5] += 2;
				}
			}
		}
		for (int i = 0; i < H - 1; i+=2) {
			for (int j = nx - 4; j < W&&kind[i + 1][j] == -1 && num[4] - used[4] >= 2; j++) {
				kind[i][j] = kind[i][j - 1];
				kind[i + 1][j] = kind[i + 1][j - 1];
				kind[i][j - 1] = 4;
				kind[i + 1][j - 1] = 4;
				used[4] += 2;
			}
			for (int j = nx; j < W&&kind[i + 1][j] == -1 && kind[i + 1][j - 1] == 2 && kind[i][j - 1] == 1&& num[4] - used[4] >= 2; j++) {
				kind[i][j] = kind[i][j - 1];
				kind[i + 1][j] = kind[i + 1][j - 1];
				kind[i][j - 1] = 4;
				kind[i + 1][j - 1] = 4;
				used[4] += 2;
			}
		}
	
		int score = 0;
		used[5] -= tate_minus;
		for (int i = 0; i < 6; i++) {
			score += used[i];
		}
		if (score <= best) {
			return{ vector<vector<int>>(0), score };
		}
		int y = H - 1;
		int x = 0;
		stack<pair<int, int>>S;
		S.push({ y,x });
		flag[y][x] = 1;
		while (!S.empty()) {
			int cy = S.top().first;
			int cx = S.top().second;
			int ck = kind[cy][cx];
			flag[cy][cx] = 2;
			S.pop();
			vector<bool>can(4, true);
			for (int i = 0; i < 2; i++) {
				int ny = cy + dir_y[ck * 2 + i];
				int nx = cx + dir_x[ck * 2 + i];
				if (flag[ny][nx] == 0) {
					S.push({ ny,nx });
					flag[ny][nx] = 1;
				}
				else if (flag[ny][nx] == 2) {
					can[(lights[ret[ny][nx]][1] - 'a')] = false;
				}
			}
			int nc = -1;
			for (int i = 0; i < 4; i++) {
				if (can[i]) {
					if (nc == -1) {
						if (box[ck][i].size() - index[ck][i]) {
							nc = i;
						}
					}
					else if (box[ck][nc].size() - index[ck][nc]<box[ck][i].size() - index[ck][i] && box[ck][i].size() - index[ck][i]) {
						nc = i;
					}
				}
			}
			if (nc == -1) {
				return{ vector<vector<int>>(0),0 };
			}
			ret[cy][cx] = box[ck][nc][index[ck][nc]];
			index[ck][nc]++;
			if (box[ck][nc].size() < index[ck][nc]) {
				return{ vector<vector<int>>(0),0 };
			}
		}
		stack<int>ex;
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				while (box[i][j].size() > index[i][j]) {
					ex.push(box[i][j][index[i][j]]);
					index[i][j]++;
				}
			}
		}
		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				if (ret[i][j] == -1) {
					ret[i][j] = ex.top();
					ex.pop();
				}
			}
		}
		return{ ret,score };
	}
	pair<vector<vector<int>>, int> search3oe(int H, int W, int yoko, int amari, int tate_minus, int best, vector<string>& lights, vector<vector<vector<int>>>box, vector<int>num) {
		vector<vector<int>>ret(H, vector<int>(W, -1));
		vector<vector<int>>flag(H, vector<int>(W));
		vector<vector<int>>index(6, vector<int>(4));
		vector<vector<int>>kind(H, vector<int>(W, -1));
		vector<int>used(6);
		used[5] += tate_minus;
		//yoko
		for (int i = 0; i < yoko; i += 2) {
			kind[H - 1 - i][0] = 3;
			kind[H - 1 - i][W - 2] = 1;
			used[3]++;
			used[1]++;
		}
		for (int i = 1; i < yoko; i += 2) {
			kind[H - 1 - i][0] = 0;
			kind[H - 1 - i][W - 2] = 2;
			used[0]++;
			used[2]++;
		}
		for (int i = 1; i < W - 1; i++) {
			kind[H - 1][i] = 4;
			used[4]++;
		}
		used[1]--;
		for (int i = 1; i < yoko; i++) {
			for (int j = 1; j < W - 2; j++) {
				kind[H - 1 - i][j] = 4;
				used[4]++;
			}
		}
		kind[H - 1][W - 1] = 2;
		used[2]++;
		for (int i = 1; i < yoko; i++) {
			kind[H - i - 1][W - 1] = 5;
			used[5]++;
		}
		//uneune
		kind[H - yoko - 1][0] = 0;
		used[0]++;
		kind[H - yoko - 1][W - 1] = 1;
		used[1]++;
		for (int i = 1; i < W - 1; i++) {
			kind[H - yoko - 1][i] = 4;
			used[4]++;
		}
		int nx = 0;
		for (int i = 0; i + 3 < W; i += 4) {
			if (num[0] - used[0] - 2 < amari) {
				break;
			}
			if (num[1] - used[1] - 2 < amari) {
				break;
			}
			if (num[2] - used[2] - 2 < amari) {
				break;
			}
			if (num[3] - used[3] - 2 < amari) {
				break;
			}
			for (int j = H - yoko - 1; j >= 2; j -= 2) {
				if (num[0] - used[0] - 2 < amari) {
					break;
				}
				if (num[1] - used[1] - 2 < amari) {
					break;
				}
				if (num[2] - used[2] - 2 < amari) {
					break;
				}
				if (num[3] - used[3] - 2 < amari) {
					break;
				}
				nx = i + 4;
				used[0] += 2;
				used[1] += 2;
				used[2] += 2;
				used[3] += 2;
				kind[j][i + 1] = 2;
				kind[j][i + 2] = 3;
				kind[j - 1][i] = 3;
				kind[j - 1][i + 1] = 1;
				kind[j - 1][i + 2] = 0;
				kind[j - 1][i + 3] = 2;
				kind[j - 2][i] = 0;
				kind[j - 2][i + 1] = 4;
				kind[j - 2][i + 2] = 4;
				kind[j - 2][i + 3] = 1;
			}
			if (kind[1][i] != -1) {
				if (num[0] - used[0] > amari && num[1] - used[1] > amari && num[2] - used[2] > amari && num[3] - used[3] > amari && num[5] - used[5] > 2) {
					kind[0][i] = 0;
					kind[0][i + 1] = 1;
					kind[0][i + 2] = 0;
					kind[0][i + 3] = 1;
					kind[1][i] = 5;
					kind[1][i + 1] = 3;
					kind[1][i + 2] = 2;
					kind[1][i + 3] = 5;
					used[1]++;
					used[2]++;
					used[3]++;
					used[5] += 2;
					used[4] -= 2;
					used[0]++;
				}
			}
		}
		for (int i = W - 2; i >= nx; i -= 2) {
			if (used[0] == num[0]) {
				break;
			}
			if (used[1] == num[1]) {
				break;
			}
			if (used[2] == num[2]) {
				break;
			}
			if (used[3] == num[3]) {
				break;
			}
			kind[H - yoko - 1][i] = 2;
			kind[H - yoko - 1][i + 1] = 3;
			used[2]++;
			used[3]++;
			used[4] -= 2;
			int y = H - yoko - 2;
			while (y&&num[5] - used[5] > 2) {
				kind[y][i] = 5;
				kind[y][i + 1] = 5;
				used[5] += 2;
				y--;
			}
			kind[y][i] = 0;
			kind[y][i + 1] = 1;
			used[0]++;
			used[1]++;
		}
		if (kind[H - yoko - 1][W - 1] == 3) {
			used[3]--;
			kind[H - yoko - 1][W - 1] = 5;
			used[5]++;
		}
		if (num[5] - used[5] > 0) {
			int box_y = -1;
			for (int i = 0; i < H; i++) {
				if (kind[i][nx - 1] != -1) {
					box_y = i;
					break;
				}
			}
			if (num[0] - used[0] > 0 && num[2] - used[2] > 0 && box_y) {
				kind[box_y][nx - 2] = 2;
				kind[box_y][nx - 1] = 5;
				kind[box_y - 1][nx - 2] = 0;
				kind[box_y - 1][nx - 1] = 1;
				used[0]++;
				used[2]++;
				used[5]++;
				used[4]--;
				for (int i = box_y - 1; i&&num[5] - used[5] >= 2; i--) {
					kind[i - 1][nx - 2] = kind[i][nx - 2];
					kind[i - 1][nx - 1] = kind[i][nx - 1];
					kind[i][nx - 2] = 5;
					kind[i][nx - 1] = 5;
					used[5] += 2;
				}
			}
			if (num[1] - used[1] > 0 && num[3] - used[3] > 0 && num[5] - used[5] > 0 && box_y) {
				kind[box_y][nx - 4] = 5;
				kind[box_y][nx - 3] = 3;
				kind[box_y - 1][nx - 4] = 0;
				kind[box_y - 1][nx - 3] = 1;
				used[1]++;
				used[3]++;
				used[5]++;
				used[4]--;
				for (int i = box_y - 1; i&&num[5] - used[5] >= 2; i--) {
					kind[i - 1][nx - 4] = kind[i][nx - 4];
					kind[i - 1][nx - 3] = kind[i][nx - 3];
					kind[i][nx - 4] = 5;
					kind[i][nx - 3] = 5;
					used[5] += 2;
				}
			}
		}
		for (int i = 3; i < H - 1; i += 2) {
			for (int j = nx - 4; j < W&&kind[i + 1][j] == -1 && num[4] - used[4] >= 2; j++) {
				kind[i][j] = kind[i][j - 1];
				kind[i + 1][j] = kind[i + 1][j - 1];
				kind[i][j - 1] = 4;
				kind[i + 1][j - 1] = 4;
				used[4] += 2;
			}
			for (int j = nx; j < W&&kind[i + 1][j] == -1 && kind[i + 1][j - 1] == 2&&kind[i][j-1]==1 && num[4] - used[4] >= 2; j++) {
				kind[i][j] = kind[i][j - 1];
				kind[i + 1][j] = kind[i + 1][j - 1];
				kind[i][j - 1] = 4;
				kind[i + 1][j - 1] = 4;
				used[4] += 2;
			}
		}

		int score = 0;
		used[5] -= tate_minus;
		for (int i = 0; i < 6; i++) {
			score += used[i];
		}
		if (score <= best) {
			return{ vector<vector<int>>(0), score };
		}
		int y = H - 1;
		int x = 0;
		stack<pair<int, int>>S;
		S.push({ y,x });
		flag[y][x] = 1;
		while (!S.empty()) {
			int cy = S.top().first;
			int cx = S.top().second;
			int ck = kind[cy][cx];
			flag[cy][cx] = 2;
			S.pop();
			vector<bool>can(4, true);
			for (int i = 0; i < 2; i++) {
				int ny = cy + dir_y[ck * 2 + i];
				int nx = cx + dir_x[ck * 2 + i];
				if (flag[ny][nx] == 0) {
					S.push({ ny,nx });
					flag[ny][nx] = 1;
				}
				else if (flag[ny][nx] == 2) {
					can[(lights[ret[ny][nx]][1] - 'a')] = false;
				}
			}
			int nc = -1;
			for (int i = 0; i < 4; i++) {
				if (can[i]) {
					if (nc == -1) {
						if (box[ck][i].size() - index[ck][i]) {
							nc = i;
						}
					}
					else if (box[ck][nc].size() - index[ck][nc]<box[ck][i].size() - index[ck][i] && box[ck][i].size() - index[ck][i]) {
						nc = i;
					}
				}
			}
			if (nc == -1) {
				return{ vector<vector<int>>(0),0 };
			}
			ret[cy][cx] = box[ck][nc][index[ck][nc]];
			index[ck][nc]++;
			if (box[ck][nc].size() < index[ck][nc]) {
				return{ vector<vector<int>>(0),0 };
			}
		}
		stack<int>ex;
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				while (box[i][j].size() > index[i][j]) {
					ex.push(box[i][j][index[i][j]]);
					index[i][j]++;
				}
			}
		}
		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				if (ret[i][j] == -1) {
					ret[i][j] = ex.top();
					ex.pop();
				}
			}
		}
		return{ ret,score };
	}
public:
	vector<int> create(int H, int W, vector<string> lights) {
		vector<int>ret(H*W, -1);
		vector<vector<int>>field;
		vector<vector<vector<int>>>box(6, vector<vector<int>>(4));
		vector<int>num(6);
		vector<int>dis;
		int C = 0;
		for (int i = 0; i < H*W; i++) {
			box[lights[i][0] - '0'][lights[i][1] - 'a'].push_back(i);
		}
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				num[i] += (int)box[i][j].size();
				if (!box[i][j].empty()) {
					C = max(C, j);
					if (i < 4) {
						dis.push_back((int)box[i][j].size());
					}
				}
			}
		}
		C++;
		int score = 0;
		if(C>=3){
			for (int i = 1; i <= H / 3; i += 2) {
				if (i * 3 + 1> num[4]) {
					break;
				}
				int l = 0;
				int r = 20;
				for (int j = l; j <= r; j++) {
					int L = 0;
					int R = num[5] + 1;
					while (R - L > 1) {
						int mid = (L + R + 1) / 2;
						pair<vector<vector<int>>, int > bag;
						if (C >= 3 && H % 2 == 0) {
							bag = search3ee(H, W, i, j, mid, score, lights, box, num);
						}
						else if (C >= 3 && H % 2) {
							bag = search3oe(H, W, i, j, mid, score, lights, box, num);
						}
						if (bag.second > score) {
							field = bag.first;
							score = bag.second;
						}
						if (bag.second) {
							R = mid;
						}
						else {
							L = mid;
						}
					}
					pair<vector<vector<int>>, int > bag;
					if (C >= 3 && H % 2 == 0) {
						bag = search3ee(H, W, i, j, 0, score, lights, box, num);
					}
					else if (C >= 3 && H % 2) {
						bag = search3oe(H, W, i, j, 0, score, lights, box, num);
					}
					if (bag.second > score) {
						field = bag.first;
						score = bag.second;
					}
				}
			}
		}
		else {
			sort(dis.begin(), dis.end());
			int dif = dis.back() - dis.front();
			for (int i = 1; i <= H / 3; i += 2) {
				if (i * 3 + 1> num[4]) {
					break;
				}
				int	l = max(0, dif - 60);
				int	r = dif + 20;
				for (int j = l; j <= r; j++) {
					int L = 0;
					int R = num[5] + 1;
					while (R - L > 1) {
						int mid = (L + R + 1) / 2;
						pair<vector<vector<int>>, int > bag;
							bag = search(H, W, i, j, mid, score, lights, box, num);
						if (bag.second > score) {
							field = bag.first;
							score = bag.second;
							cerr << mid << endl;
						}
						if (bag.second) {
							R = mid;
						}
						else {
							L = mid;
						}
					}
				}
			}
		}
		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				ret[i*W + j] = field[i][j];
			}
		}
		return ret;
	}
};

// -------8<------- end of solution submitted to the website -------8<-------

int main() {
	GarlandOfLights gl;
	int H, W;
	cin >> H >> W;
	cerr << H << " " << W << endl;
	vector<string> lights(H*W);
	for (int k = 0; k < H*W; ++k) {
		cin >> lights[k];
	}

	vector<int> ret = gl.create(H, W, lights);
	cout << ret.size() << endl;
	for (int i = 0; i < (int)ret.size(); ++i)
		cout << ret[i] << endl;
	cout.flush();
}