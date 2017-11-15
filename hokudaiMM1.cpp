#include "iostream"
#include "vector"
#include "algorithm"
#include "ctime"

using namespace std;

const int MOD = 1003;

int N, M, P, Q;
vector<vector<int>>edge;
vector<pair<int, int>>ans;
vector<int>flavor;

class XorShift {
	unsigned int x, y, z, w, t;
public:
	XorShift() {
		x = 133553533;
		y = 314867339;
		z = 664298413;
		w = 999999937;
		t = 0;
	}
	unsigned int rand() {
		t = x ^ (x << 11);
		x = y;
		y = z;
		z = w;
		w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
		return w & 0x7fffffff;
	}
};

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
	Candidate(int n, int cy, int cx,bool ex) {
		num = n;
		up = 0;
		y = cy;
		x = cx;
		extend = ex;
	}
	bool operator <(const Candidate &c) const{
		return (up < c.up || (up == c.up&&extend && !c.extend));
	}
	bool operator >(const Candidate &c) const{
		return (up > c.up || (up == c.up && !extend && c.extend));
	}
};

int dir_y[] = { 1,1,1,0,-1,-1,-1,0 };
int dir_x[] = { -1,0,1,1,1,0,-1,-1 };

int main() {
	XorShift xs;
	/**/FILE *ofile;
	/**/FILE *ifile;
	/**/ofile = fopen("problem1_toolkit_JP/scripts/result.out", "w");
	/**/ifile = fopen("problem1_toolkit_JP/scripts/testcase.in", "r");
	///**/cin >> N >> M;
	/**/fscanf(ifile, "%d %d", &N, &M);
	edge.resize(N, vector<int>(N));
	flavor.resize(N, 0);
	for (int i = 0; i < M; i++) {
		int a, b, c;
		///**/cin >> a >> b >> c;
		/**/fscanf(ifile, "%d %d %d", &a, &b, &c);
		a--;
		b--;
		edge[a][b] = c;
		edge[b][a] = c;
		flavor[a] += c;
		flavor[b] += c;
	}
	///**/cin >> P >> Q;
	/**/fscanf(ifile, "%d %d", &P, &Q);
	for (int i = 2; i <= 60; i++) {
		if (i*i == P) {
			P = i;
			break;
		}
	}
	for (int i = 0; i < Q; i++) {
		int a, b;
		///**/cin >> a >> b;
		/**/fscanf(ifile, "%d %d", &a, &b);
	}
	for (int i = 0; i < N; i++) {
		flavor[i] = flavor[i] * MOD + i;
	}
	sort(flavor.begin(), flavor.end());
	ans.resize(N, { -1,-1 });
	int score = 0;
	int lft, up;

	int loop_num = 0;

	for(auto cn:flavor){
		loop_num++;
		int c_score = 0;
		vector<vector<int>>c_field(200, vector<int>(200, -1));
		vector<pair<int, int>>place(N, { -1,-1 });
		int c_lft = 100, c_rht = 100, c_up = 100, c_bottom = 100;
		vector<int>used;
		place[cn%MOD] = { 100,100 };
		c_field[100][100] = cn%MOD;
		used.push_back(cn%MOD);
		for (int loop = 1; loop < N; loop++) {
			Candidate c;
			for (int i = 0; i < N; i++) {
				if (place[i].first != -1) {
					continue;
				}
				for (auto j : used) {
					int cy = place[j].first;
					int cx = place[j].second;
					for (int k = 0; k < 8; k++) {
						int ny = cy + dir_y[k];
						int nx = cx + dir_x[k];
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
			lft = c_lft;
			score = c_score;
		}
		/**/if (clock() > 1800) {
		/**/	break;
		/**/}
		///**/if (clock() > 1800000) {
		///**/	break;
		///**/}
		
	}
	cout << N << " " << loop_num << endl;
	for (int i = 0; i < N; i++) {
		///**/ cout << i + 1 << " " << (place[i].first - up)*P + place[i].second - lft + 1 << endl;
		/**/fprintf(ofile, "%d %d\n", i + 1, (ans[i].first - up)*P + ans[i].second - lft + 1);
	}
	/**/fclose(ofile);
	return 0;
}