#include <iostream>
#include <vector>
#include <tuple>
#include <math.h>
#include <queue>

//using ll = long long;
using namespace std;
using weight_t = uint;
using node_t = uint;

uint N, M;       //n = cant. vertices, m = cant. calles unidireccionales
uint K;          //k = cant. calles bidireccionales
vector<vector<pair<node_t, weight_t>>> adj; //Lista de adyacencias, pair = (vert., costo)
vector<tuple<node_t, node_t, weight_t>> candidatos;

vector<bool> S; //S = conjunto de vertices ya "evaluados"
vector<int> dist; //vector de distancias mínimas de s a todos

bool comp(pair<int, int> a, pair<int, int> b){
    return a.second > b.second;
}

void dijkstra(node_t s){
    priority_queue<pair<int, int>, vector<pair<int, int>>, comp<pair<int, int>>> min_heap;

    for (int i = 0; i < N; i++)
        dist[i] = INFINITY;
    S[s] = true;
    dist[s] = 0;
    for (auto [v, l] : adj[s]) {
        dist[v] = l; //l = c(s,v)
    }

    for (int count = 0; count < N; count++){
        v = min_heap();
        for (auto [x, l] : adj[v]) {
            if(dist[s,x] > dist[s,v] + w(v->x))
                dist[s,x] = dist[s,v] + w(v->x)
        }
    }
}

int main() {
    uint c; cin >> c;
    while(c--){
        uint s, t;
        //(n ≤ 10^4 ), (m ≤ 10^5), (k < 300), (1 ≤ s ≤ n), (1 ≤ t ≤ n)
        cin >> N >> M >> K >> s >> t;
        //Indexamos a 0
        s--;
        t--;
        //Init adjacency list
        adj = vector<vector<pair<node_t, weight_t>>>(N, vector<pair<node_t, weight_t>>());
        while (M--) {
            int d, c, l; //(0 < l ≤ 1000)
            cin >> d >> c >> l; // (d, c) = arista; costo = l
            adj[d-1].push_back(make_pair(c-1, l));
        }

        while (K--) {
            int u, v, q; //(q ≤ 1000)
            cin >> u >> v >> q;
            candidatos.push_back(make_tuple(u-1, v-1, q));
        }
        
    }

    return 0;
}