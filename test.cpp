#include<bits/stdc++.h>
using namespace std;
vector<vector<int>> seq;
vector<int> comp;
vector<int> terminalSymbol;
map<int, int> comp2idx;
int adj[666][666];
struct GAMESTATUS{
	int idx;	//Index of status(seq, comp)
	int player;	//Current player 0: player1, 1: player2
};
int Vec2Int(vector<int> & seq){
	int res = 0;
	for(auto i : seq)
		res = res * 10 + i;
	return res;
}
int CompStatus(int p1l, int p1r, int p2l, int p2r){
	return p1l * 1000 + p1r * 100 + p2l * 10 + p2r;
}
void go(int toPick, vector<int> & pick){
	if(toPick == 0){
		seq.push_back(pick);
		return;
	}
	for(int i = 0; i<5; ++i){
		pick.push_back(i);
		go(toPick - 1, pick);
		pick.pop_back();
	}
}
void MakeGraph(){
	//Game starts at the 155
	const int startNumber = comp2idx[1111];
	bool chk[666] = {0};
	queue<GAMESTATUS> q;
	q.push({startNumber, 0});
	chk[startNumber] = 1;
	while(q.size()){
		auto cur = q.front(); q.pop();
		int idx = cur.idx;
		int p = cur.player;
		int p1l = seq[idx][0], p1r = seq[idx][1], p2l = seq[idx][2], p2r = seq[idx][3];
		if(p){
			swap(p1l, p2l);
			swap(p1r, p2r);
		}
		int nx;
		//Touch player2's left hand with left hand
		if(p1l){	//If player 1 has number in left hand
			if(p1l + p2l >= 5){
				if(p)
					nx = CompStatus(0, p2r, p1l, p1r);
				
				else
					nx = CompStatus(p1l, p1r, 0, p2r);
			}
			else{
				if(p)
					nx = CompStatus(p1l + p2l, p2r, p1l, p1r);
				
				else
					nx = CompStatus(p1l, p1r, p1l + p2l, p2r);
			}
			printf("%04d > %04d\n", comp[idx], nx);
			nx = comp2idx[nx];
			adj[idx][nx] = 1;
			if(chk[nx] == 0){
				chk[nx] = 1;
				q.push({nx, p ^ 1});
			}	
		}

		//Touch player2's right hand with left hand
		if(p1l){	//If player 1 has number in left hand
			if(p1l + p2r >= 5){
				if(p)
					nx = CompStatus(p2l, 0, p1l, p1r);
				
				else
					nx = CompStatus(p1l, p1r, p2l, 0);
			}
			else{
				if(p)
					nx = CompStatus(p2l, p1l + p2r, p1l, p1r);
				
				else
					nx = CompStatus(p1l, p1r, p2l, p1l + p2r);
			}
			printf("%04d > %04d\n", comp[idx], nx);
			nx = comp2idx[nx];
			adj[idx][nx] = 1;
			if(chk[nx] == 0){
				chk[nx] = 1;
				q.push({nx, p ^ 1});
			}	
		}

		//Touch player2's left hand with right hand
		if(p1r){	//If player 1 has number in right hand
			if(p1r + p2l >= 5){
				if(p)
					nx = CompStatus(0, p2r, p1l, p1r);
				
				else
					nx = CompStatus(p1l, p1r, 0, p2r);
			}
			else{
				if(p)
					nx = CompStatus(p1r + p2l, p2r, p1l, p1r);
				
				else
					nx = CompStatus(p1l, p1r, p1r + p2l, p2r);
			}
			printf("%04d > %04d\n", comp[idx], nx);
			nx = comp2idx[nx];
			adj[idx][nx] = 1;
			if(chk[nx] == 0){
				chk[nx] = 1;
				q.push({nx, p ^ 1});
			}	
		}

		//Touch player2's right hand with right hand
		if(p1r){	//If player 1 has number in right hand
			if(p1r + p2r >= 5){
				if(p)
					nx = CompStatus(p2l, 0, p1l, p1r);
				
				else
					nx = CompStatus(p1l, p1r, p2l, 0);
			}
			else{
				if(p)
					nx = CompStatus(p2l, p1r + p2r, p1l, p1r);
				
				else
					nx = CompStatus(p1l, p1r, p2l, p1r + p2r);
			}
			printf("%04d > %04d\n", comp[idx], nx);
			nx = comp2idx[nx];
			adj[idx][nx] = 1;
			if(chk[nx] == 0){
				chk[nx] = 1;
				q.push({nx, p ^ 1});
			}	
		}

		//Swap
		if(p1l != p1r){ //Different numbers in two hands
			if(p)
				nx = CompStatus(p2l, p2r, p1r, p1l);
			
			else
				nx = CompStatus(p1r, p1l, p2l, p2r);

			printf("%04d > %04d\n", comp[idx], nx);
			nx = comp2idx[nx];
			adj[idx][nx] = 1;
			if(chk[nx] == 0){
				chk[nx] = 1;
				q.push({nx, p ^ 1});
			}
		}
		
		//Divide two number	
		if(p1l + p1r > 1){	//if p1l + p1r is 1, It works same thing in swap
			for(int i = min(4, p1l + p1r); i >= 0; --i){
				if(i == p1l && p1l + p1r - i == p1r) continue;
				if(p)
					nx = CompStatus(p2l, p2r, i, p1l + p1r - i);
				else
					nx = CompStatus(i, p1l + p1r - i, p2l, p2r);

			printf("%04d > %04d\n", comp[idx], nx);
				nx = comp2idx[nx];
				adj[idx][nx] = 1;
				if(chk[nx] == 0){
					chk[nx] = 1;
					q.push({nx, p ^ 1});
				}
			}
		}
	}	
}
void CalcShortestPath(){
	int sz = comp.size();
	for(int k = 0; k<sz; ++k)
		for(int i = 0; i<sz; ++i)
			for(int j = 0; j<sz; ++j)
				adj[i][j] = min(adj[i][j], adj[i][k] + adj[k][i]);
}	
void PrintAdj(){
	int sz = comp.size();
	for(int i = 1; i<sz; ++i){
		for(int j = 1; j<sz; ++j)
			printf("%d ", adj[i][j] > 10000 ? 0 : adj[i][j]);	
		puts("");
	}
}
void FindTerminalSymbol(){
	for(int i = 0; i<seq.size(); ++i)
		if((seq[i][0] == seq[i][1] && seq[i][0] == 0)
			|| (seq[i][2] == seq[i][3] && seq[i][2] == 0))
			terminalSymbol.push_back(comp[i]);
}
void Preprocess(){
	memset(adj, 0x3f, sizeof(adj));
	vector<int> tmp;

	go(4, tmp);	//Get all status
	sort(seq.begin(), seq.end());	//Sort status increasing order
	for(auto i : seq)	//Make status to integer
		comp.push_back(Vec2Int(i));

	FindTerminalSymbol();

	for(int i = 1; i<comp.size(); ++i)	//Make status to index
		comp2idx[comp[i]] = i;

	freopen("res.txt", "w", stdout);
	MakeGraph();

	int count = 0;
		for(int i = 0; i<comp.size(); ++i)
			for(int j = 0; j<comp.size(); ++j)
				if(adj[i][j] == 1)
					++count;
	printf("%d\n%d", count, comp[0]);
	CalcShortestPath();
	
//	PrintAdj();
}	
int main(){
	int cnt = 0;
	Preprocess();
}