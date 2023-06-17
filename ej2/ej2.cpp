#include<stdio.h>
#include<vector>

using namespace std;
using Node = unsigned int;
using Grafo = vector<vector<Node>>; //Adj. List

unsigned int N, M; //2 ≤ n ≤ 300
vector<bool> Prim;
Grafo G;

int main(){
    while (true)
    {
        scanf("%u %u", &N, &M);
        if (N == 0 && M == 0) break;

        Prim = vector(N, false);
        for (unsigned int i = 0; i < N; i++) {
            unsigned int p; scanf("%u", &p);
            Prim[i] = (bool) p;
        }

        G = Grafo(N, vector<Node>());
        while (M--) {
            unsigned int i, j;
            scanf("%u %u", &i, &j);
            G[i-1].push_back(j-1);
            G[j-1].push_back(i-1);
        }
    }
    return 0;
}