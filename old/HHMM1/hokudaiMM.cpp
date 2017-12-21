#include "iostream"
#include "vector"
#include "algorithm"
#include "ctime"

using namespace std;

const int MOD = 1003;

int N, M, P, Q;
vector<vector<int>>edge;
vector<pair<int, int>>ans;
vector<vector<int>>field;
vector<int>flavor;

int ay, ax, by, bx, cy, cx, dy, dx;
int lft, rht, up, bottom;
int dif;
int dir_y[] = { 1,1,1,0,-1,-1,-1,0 };
int dir_x[] = { -1,0,1,1,1,0,-1,-1 };

class Candidate {
public:
	int num;
	int up;
	int y;
	int x;
	bool extend;
	Candidate() {
		num = -1;
		up = -1;
		y = -1;
		x = -1;
		extend = false;
	}
	Candidate(int n, int cy, int cx, bool ex) {
		num = n;
		up = 0;
		y = cy;
		x = cx;
		extend = ex;
	}
	bool operator <(const Candidate &c) const {
		if (up < c.up) {
			return true;
		}
		if (up > c.up) {
			return false;
		}
		if (extend && !c.extend) {
			return true;
		}
		else {
			return false;
		}
	}
	bool operator >(const Candidate &c) const {
		if (up > c.up) {
			return true;
		}
		if (up < c.up) {
			return false;
		}
		if (!extend && c.extend) {
			return true;
		}
		else {
			return false;
		}
	}
	bool operator !=(const Candidate &c)const {
		if (x != c.x || y != c.y) {
			return true;
		}
		else {
			return false;
		}
	}
};

inline void one_move(int a) {
	ay = ans[a].first;
	ax = ans[a].second;
	int gain = 0;
	for (int i = 0; i < 8; i++) {
		if (field[ay + dir_y[i]][ax + dir_x[i]] == -1) {
			continue;
		}
		gain += edge[a][field[ay + dir_y[i]][ax + dir_x[i]]];
	}
	Candidate c;
	for (int i = up; i <= bottom; i++) {
		for (int j = lft; j <= rht; j++) {
			if (field[i][j] != -1) {
				continue;
			}
			Candidate c_box(a, i, j, false);
			for (int k = 0; k < 8; k++) {
				if (field[i + dir_y[k]][j + dir_x[k]] == -1) {
					continue;
				}
				c_box.up += edge[a][field[i + dir_y[k]][j + dir_x[k]]];
			}
			c = max(c, c_box);
		}
	}
	if (c.up > gain) {
		swap(field[ay][ax], field[c.y][c.x]);
		ans[a] = { c.y,c.x };
	}
	return;
}

inline void two_swap(int a, int b, int aa, int bb) {
	field[ay][ax] = aa;
	field[by][bx] = bb;
	int gain = 0;
	for (int j = 0; j < 8; j++) {
		if (field[ay + dir_y[j]][ax + dir_x[j]] != -1) {
			gain += edge[field[ay][ax]][field[ay + dir_y[j]][ax + dir_x[j]]];
		}
		if (field[by + dir_y[j]][bx + dir_x[j]] != -1) {
			gain += edge[field[by][bx]][field[by + dir_y[j]][bx + dir_x[j]]];
		}
	}
	if (gain > dif) {
		ans[aa] = { ay,ax };
		ans[bb] = { by,bx };
		return;
	}
	field[ay][ax] = a;
	field[by][bx] = b;
	return;
}

inline int three_swap(int a, int b, int c, int aa, int bb, int cc) {
	field[ay][ax] = aa;
	field[by][bx] = bb;
	field[cy][cx] = cc;
	int gain = 0;
	for (int j = 0; j < 8; j++) {
		if (field[ay + dir_y[j]][ax + dir_x[j]] != -1) {
			gain += edge[aa][field[ay + dir_y[j]][ax + dir_x[j]]];
		}
		if (field[by + dir_y[j]][bx + dir_x[j]] != -1) {
			gain += edge[bb][field[by + dir_y[j]][bx + dir_x[j]]];
		}
		if (field[cy + dir_y[j]][cx + dir_x[j]] != -1) {
			gain += edge[cc][field[cy + dir_y[j]][cx + dir_x[j]]];
		}
	}
	if (max(abs(ax - bx), abs(ay - by)) <= 1) {
		gain -= edge[aa][bb];
	}
	if (max(abs(ax - cx), abs(ay - cy)) <= 1) {
		gain -= edge[aa][cc];
	}
	if (max(abs(cx - bx), abs(cy - by)) <= 1) {
		gain -= edge[cc][bb];
	}
	if (gain > dif) {
		ans[aa] = { ay,ax };
		ans[bb] = { by,bx };
		ans[cc] = { cy,cx };
		return gain - dif;
	}
	field[ay][ax] = a;
	field[by][bx] = b;
	field[cy][cx] = c;
	return 0;
}

int main() {
	scanf("%d %d", &N, &M);
	edge.resize(N, vector<int>(N));
	flavor.resize(N);
	for (int i = 0; i < M; i++) {
		int a, b, c;
		c = 1;
		scanf("%d %d", &a, &b);
		a--;
		b--;
		edge[a][b] = c;
		edge[b][a] = c;
		flavor[a] += c;
		flavor[b] += c;
	}
	scanf("%d %d", &P, &Q);
	for (int i = 2; i <= 60; i++) {
		if (i*i == P) {
			P = i;
			break;
		}
	}
	for (int i = 0; i < Q; i++) {
		int a, b;
		scanf("%d %d", &a, &b);
	}
	for (int i = 0; i < N; i++) {
		flavor[i] = flavor[i] * MOD + i;
	}
	sort(flavor.begin(), flavor.end());
	reverse(flavor.begin(), flavor.end());
	ans.resize(N, { -1,-1 });
	int score = 0;
	int max_loop = 0;
	int bef = clock();
	vector<vector<int>>c_field(P * 2 + 3, vector<int>(P * 2 + 3, -1));
	for (auto ccn : flavor) {
		int cn = ccn%MOD;
		int c_score = 0;
		vector<pair<int, int>>place(N, { -1,-1 });
		int c_lft = P + 1, c_rht = P + 1, c_up = P + 1, c_bottom = P + 1;
		vector<int>used;
		place[cn] = { P + 1,P + 1 };
		c_field[P + 1][P + 1] = cn;
		used.push_back(cn);
		Candidate dummy;
		for (int loop = 1; loop < (N - 1) / 2 * 2; loop += 2) {
			vector<vector<Candidate>>cand(N, vector<Candidate>(2, dummy));
			for (int i = 0; i < N; i++) {
				if (place[i].first != -1) {
					continue;
				}
				for (auto j : used) {
					if (!edge[i][j] && (cand[i][1].up > 0 || (cand[i][1].up == 0 && !cand[i][1].extend))) {
						continue;
					}
					int ccy = place[j].first;
					int ccx = place[j].second;
					for (int k = 0; k < 8; k++) {
						int ny = ccy + dir_y[k];
						int nx = ccx + dir_x[k];
						if (abs(ny - c_up) >= P || abs(ny - c_bottom) >= P || abs(nx - c_lft) >= P || abs(nx - c_rht) >= P) {
							continue;
						}
						if (c_field[ny][nx] != -1) {
							continue;
						}
						Candidate c_box(i, ny, nx, c_up > ny || c_bottom<ny || c_lft > nx || c_rht < nx);
						for (int l = 0; l < 8; l++) {
							if (c_field[ny + dir_y[l]][nx + dir_x[l]] != -1) {
								c_box.up += edge[i][c_field[ny + dir_y[l]][nx + dir_x[l]]];
							}
						}
						if (cand[i][0] < c_box) {
							swap(cand[i][1], cand[i][0]);
							cand[i][0] = c_box;
						}
						else if (cand[i][1] < c_box&&c_box != cand[i][0]) {
							cand[i][1] = c_box;
						}
					}
				}
			}
			Candidate ca;
			Candidate cb;
			int plus = 0;
			for (int i = 0; i < N; i++) {
				if (place[i].first != -1) {
					continue;
				}
				int n_up = min(c_up, cand[i][0].y);
				int n_bottom = max(c_bottom, cand[i][0].y);
				int n_lft = min(c_lft, cand[i][0].x);
				int n_rht = max(c_rht, cand[i][0].x);
				for (int j = 0; j < N; j++) {
					if (i == j) {
						continue;
					}
					if (place[j].first != -1) {
						continue;
					}
					bool exit = false;
					if (!(cand[i][0].y == cand[j][0].y&&cand[i][0].x == cand[j][0].x)) {
						if (abs(n_up - cand[j][0].y) >= P || abs(n_bottom - cand[j][0].y) >= P || abs(n_lft - cand[j][0].x) >= P || abs(n_rht - cand[j][0].x) >= P) {
							continue;
						}
						if (max(abs(cand[i][0].y - cand[j][0].y), abs(cand[i][0].x - cand[j][0].x)) <= 1) {
							if (ca.up + cb.up + plus < cand[i][0].up + cand[j][0].up + edge[i][j]) {
								ca = cand[i][0];
								cb = cand[j][0];
								plus = edge[i][j];
							}
							else if (ca.up + cb.up + plus == cand[i][0].up + cand[j][0].up + edge[i][j] && ca.extend&cb.extend && !cand[i][0].extend && !cand[j][0].extend) {
								ca = cand[i][0];
								cb = cand[j][0];
								plus = edge[i][j];
							}
							exit = true;
						}
						else {
							if (ca.up + cb.up + plus < cand[i][0].up + cand[j][0].up) {
								ca = cand[i][0];
								cb = cand[j][0];
								plus = 0;
							}
							else if (ca.up + cb.up + plus == cand[i][0].up + cand[j][0].up&& ca.extend&cb.extend && !cand[i][0].extend && !cand[j][0].extend) {
								ca = cand[i][0];
								cb = cand[j][0];
								plus = 0;
							}
						}
					}
					if (!exit && !(cand[i][0].y == cand[j][1].y&&cand[i][0].x == cand[j][1].x)) {
						if (abs(n_up - cand[j][1].y) >= P || abs(n_bottom - cand[j][1].y) >= P || abs(n_lft - cand[j][1].x) >= P || abs(n_rht - cand[j][1].x) >= P) {
							continue;
						}
						if (max(abs(cand[i][0].y - cand[j][1].y), abs(cand[i][0].x - cand[j][1].x)) <= 1) {
							if (ca.up + cb.up + plus < cand[i][0].up + cand[j][1].up + edge[i][j]) {
								ca = cand[i][0];
								cb = cand[j][1];
								plus = edge[i][j];
							}
							else if (ca.up + cb.up + plus == cand[i][0].up + cand[j][1].up + edge[i][j] && ca.extend&cb.extend && !cand[i][0].extend && !cand[j][1].extend) {
								ca = cand[i][0];
								cb = cand[j][1];
								plus = edge[i][j];
							}
							exit = true;
						}
						else {
							if (ca.up + cb.up + plus < cand[i][0].up + cand[j][1].up) {
								ca = cand[i][0];
								cb = cand[j][1];
								plus = 0;
							}
							else if (ca.up + cb.up + plus == cand[i][0].up + cand[j][1].up&& ca.extend&cb.extend && !cand[i][0].extend && !cand[j][1].extend) {
								ca = cand[i][0];
								cb = cand[j][1];
								plus = 0;
							}
						}
					}
					if (!exit) {
						for (int k = 0; k < 8; k++) {
							int ny = cand[i][0].y + dir_y[k];
							int nx = cand[i][0].x + dir_x[k];
							if (abs(ny - n_up) >= P || abs(ny - n_bottom) >= P || abs(nx - n_lft) >= P || abs(nx - n_rht) >= P) {
								continue;
							}
							if (c_field[ny][nx] != -1) {
								continue;
							}
							Candidate c_box(j, ny, nx, n_up > ny || n_bottom<ny || n_lft > nx || n_rht < nx);
							for (int l = 0; l < 8; l++) {
								if (c_field[ny + dir_y[l]][nx + dir_x[l]] != -1) {
									c_box.up += edge[j][c_field[ny + dir_y[l]][nx + dir_x[l]]];
								}
							}
							if (ca.up + cb.up + plus < cand[i][0].up + c_box.up + edge[i][j]) {
								ca = cand[i][0];
								cb = c_box;
								plus = edge[i][j];
							}
							else if (ca.up + cb.up + plus == cand[i][0].up + c_box.up + edge[i][j] && ca.extend&cb.extend && !cand[i][0].extend && !c_box.extend) {
								ca = cand[i][0];
								cb = c_box;
								plus = edge[i][j];
							}
						}
					}
				}
			}
			c_field[ca.y][ca.x] = ca.num;
			c_field[cb.y][cb.x] = cb.num;
			place[ca.num] = { ca.y,ca.x };
			place[cb.num] = { cb.y,cb.x };
			used.push_back(ca.num);
			used.push_back(cb.num);
			c_lft = min({ c_lft, ca.x,cb.x });
			c_rht = max({ c_rht, ca.x,cb.x });
			c_up = min({ c_up, ca.y,cb.y });
			c_bottom = max({ c_bottom, ca.y,cb.y });
			c_score += ca.up + cb.up + plus;
		}
		for (int loop = (N - 1) / 2 * 2 + 1; loop < N; loop++) {
			Candidate c;
			for (int i = 0; i < N; i++) {
				if (place[i].first != -1) {
					continue;
				}
				for (auto j : used) {
					if (!edge[i][j] && (c.up > 0 || (c.up == 0 && !c.extend))) {
						continue;
					}
					int ccy = place[j].first;
					int ccx = place[j].second;
					for (int k = 0; k < 8; k++) {
						int ny = ccy + dir_y[k];
						int nx = ccx + dir_x[k];
						if (abs(ny - c_up) >= P || abs(ny - c_bottom) >= P || abs(nx - c_lft) >= P || abs(nx - c_rht) >= P) {
							continue;
						}
						if (c_field[ny][nx] != -1) {
							continue;
						}
						Candidate c_box(i, ny, nx, c_up > ny || c_bottom<ny || c_lft > nx || c_rht < nx);
						for (int l = 0; l < 8; l++) {
							if (c_field[ny + dir_y[l]][nx + dir_x[l]] != -1) {
								c_box.up += edge[i][c_field[ny + dir_y[l]][nx + dir_x[l]]];
							}
						}
						c = max(c, c_box);
					}
				}
			}
			c_field[c.y][c.x] = c.num;
			place[c.num] = { c.y,c.x };
			used.push_back(c.num);
			c_lft = min(c_lft, c.x);
			c_rht = max(c_rht, c.x);
			c_up = min(c_up, c.y);
			c_bottom = max(c_bottom, c.y);
			c_score += c.up;
		}
		if (score < c_score) {
			ans = place;
			up = c_up;
			rht = c_rht;
			bottom = c_bottom;
			lft = c_lft;
			score = c_score;
		}
		for (int i = 0; i < N; i++) {
			c_field[place[i].first][place[i].second] = -1;
		}
		int c_time = clock();
		max_loop = max(max_loop, c_time - bef);
		bef = c_time;
		if (bef + max_loop > 29800000 - N*N*N*1000  / 42500) {
			break;
		}
	}
	field.resize(P * 2 + 3, vector<int>(P * 2 + 3, -1));
	for (int i = 0; i < N; i++) {
		field[ans[i].first][ans[i].second] = i;
	}
	for (int loop = 0; loop < 3; loop++) {
		for (int a = 0; a < N; a++) {
			one_move(a);
		}
	}
	for (int a = 0; a < N - 1; a++) {
		for (int b = a + 1; b < N; b++) {
			ay = ans[a].first;
			ax = ans[a].second;
			by = ans[b].first;
			bx = ans[b].second;
			dif = 0;
			for (int j = 0; j < 8; j++) {
				if (field[ay + dir_y[j]][ax + dir_x[j]] != -1) {
					dif += edge[field[ay][ax]][field[ay + dir_y[j]][ax + dir_x[j]]];
				}
				if (field[by + dir_y[j]][bx + dir_x[j]] != -1) {
					dif += edge[field[by][bx]][field[by + dir_y[j]][bx + dir_x[j]]];
				}
			}
			two_swap(a, b, b, a);
		}
	}
	for (int a = 0; a < N; a++) {
		one_move(a);
	}
	for (int a = 0; a < N - 2; a++) {
		if (clock() > 29800000) {
			break;
		}
		for (int b = a + 1; b < N - 1; b++) {
			for (int c = b + 1; c < N; c++) {
				ay = ans[a].first;
				ax = ans[a].second;
				by = ans[b].first;
				bx = ans[b].second;
				cy = ans[c].first;
				cx = ans[c].second;
				dif = 0;
				for (int j = 0; j < 8; j++) {
					if (field[ay + dir_y[j]][ax + dir_x[j]] != -1) {
						dif += edge[a][field[ay + dir_y[j]][ax + dir_x[j]]];
					}
					if (field[by + dir_y[j]][bx + dir_x[j]] != -1) {
						dif += edge[b][field[by + dir_y[j]][bx + dir_x[j]]];
					}
					if (field[cy + dir_y[j]][cx + dir_x[j]] != -1) {
						dif += edge[c][field[cy + dir_y[j]][cx + dir_x[j]]];
					}
				}
				if (max(abs(ax - bx), abs(ay - by)) <= 1) {
					dif -= edge[a][b];
				}
				if (max(abs(ax - cx), abs(ay - cy)) <= 1) {
					dif -= edge[a][c];
				}
				if (max(abs(cx - bx), abs(cy - by)) <= 1) {
					dif -= edge[c][b];
				}
				dif += max(0, three_swap(a, b, c, c, a, b));
				dif += max(0, three_swap(a, b, c, b, c, a));
			}
		}
	}
	for (int a = 0; a < N; a++) {
		one_move(a);
	}
	for (int a = 0; a < N - 1; a++) {
		for (int b = a + 1; b < N; b++) {
			ay = ans[a].first;
			ax = ans[a].second;
			by = ans[b].first;
			bx = ans[b].second;
			dif = 0;
			for (int j = 0; j < 8; j++) {
				if (field[ay + dir_y[j]][ax + dir_x[j]] != -1) {
					dif += edge[field[ay][ax]][field[ay + dir_y[j]][ax + dir_x[j]]];
				}
				if (field[by + dir_y[j]][bx + dir_x[j]] != -1) {
					dif += edge[field[by][bx]][field[by + dir_y[j]][bx + dir_x[j]]];
				}
			}
			two_swap(a, b, b, a);
		}
	}
	for (int a = 0; a < N; a++) {
		one_move(a);
	}
	for (int i = 0; i < N; i++) {
		printf("1 %d\n", (ans[i].first - up)*P + ans[i].second - lft + 1);
	}
	return 0;
}