#include <iostream>
#include <vector>
#include <tuple>
#include <math.h>
#include <queue>

using ll = long long;
const ll inf = 1e10;

using namespace std;
using weight_t = uint;
using node_t = uint;
using Grafo = vector<vector<pair<node_t, weight_t>>>; //Adj. List

uint N, M;       //n = cant. vertices, m = cant. calles unidireccionales
uint K;          //k = cant. calles bidireccionales
Grafo G; //Lista de adyacencias, pair = (vert., costo)
Grafo G_T; //Grafo con direcciones invertidas
vector<tuple<node_t, node_t, weight_t>> candidatos;

class compare { //Para comparar y hacer el min-heap
    public:
        int operator() (pair<node_t, ll>& p1,pair<node_t, ll>& p2) {
            return p1.second > p2.second;
        }
};

void dijkstra(Grafo& g, vector<ll>& dist, node_t s){
    vector<bool> S(N, false); //S = conjunto de vertices ya "evaluados"
    priority_queue<pair<node_t, ll>, vector<pair<node_t, ll>>, compare> min_heap;

    for (uint i = 0; i < N; i++)
        dist[i] = inf;
    dist[s] = 0;
    min_heap.push(make_pair(s, 0));

    while (!min_heap.empty()) {
        pair<node_t, ll> p = min_heap.top(); min_heap.pop();
        node_t u = p.first;
        ll dist_cand = p.second;
        S[u] = true; //Marcamos como visitado
        //Chequamos si no encontramos ya un mejor camino de s a u
        if (dist[u] < dist_cand) continue;

        for (auto [v, cost] : g[u]) { //g es lista de ady.
            if (S[v]) continue; //Si el nodo fue visitado, ignorarlo
            //Relajamos los adyacentes a u no visitados
            ll new_dist = dist[u] + ((ll) cost); 
            if (new_dist < dist[v]) {
                dist[v] = new_dist;
                min_heap.push(make_pair(v, new_dist));
            }
        }
    }
}

ll mejorarTrafico(node_t s, node_t t){
    ll res;
    vector<ll> dist_desdeS(N);
    vector<ll> dist_desdeT(N);
    dijkstra(G, dist_desdeS, s);
    dijkstra(G_T, dist_desdeT, t);
    res = dist_desdeS[t];
    
    for (auto [u, v, q] : candidatos) {
        ll new_dist1 = dist_desdeS[u] + ((ll) q) + dist_desdeT[v];
        ll new_dist2 = dist_desdeS[v] + ((ll) q) + dist_desdeT[u];
        if (res > min(new_dist1, new_dist2))
            res = min(new_dist1, new_dist2);
    }

    return res;
}

int main() {
    uint c; cin >> c; //0 < c ≤ 20
    while(c--){
        node_t s, t;
        //(n ≤ 10^4 ), (m ≤ 10^5), (k < 300), (1 ≤ s ≤ n), (1 ≤ t ≤ n)
        cin >> N >> M >> K >> s >> t;
        //Indexamos a 0
        s--;
        t--;
        //Init adjacency list
        G = Grafo(N, vector<pair<node_t, weight_t>>());
        G_T = Grafo(N, vector<pair<node_t, weight_t>>());
        while (M--) {
            int d, c, l; //(0 < l ≤ 1000)
            cin >> d >> c >> l; // (d, c) = arista; costo = l
            G[d-1].push_back(make_pair(c-1, l));
            G_T[c-1].push_back(make_pair(d-1, l));
        }
        //Init candidatos
        candidatos.clear();
        while (K--) {
            int u, v, q; //(q ≤ 1000)
            cin >> u >> v >> q;
            candidatos.push_back(make_tuple(u-1, v-1, q));
        }
        ll res = mejorarTrafico(s, t);
        if (res >= inf)
            cout << -1 << endl;
        else        
            cout << res << endl;
    }

    return 0;
}