#include <iostream>
#include <vector>
#include <tuple>
#include <math.h>
#include <queue>
#include <set>

#include <bits/stdc++.h>
#include <sys/stat.h>
#include <chrono>

using namespace std;
string dir_path;

const int INF = 1e9;
using Grafo = vector<vector<pair<int, int>>>; //Adj. List

int N, M, K;       //n = cant. vertices, m = cant. calles unidireccionales, k = cant. calles bidireccionales
int s, t;        
Grafo G; //Lista de adyacencias, pair = (vert., costo)
Grafo G_T; //Grafo con direcciones invertidas
vector<tuple<int, int, int>> candidatos;
//-----------------------------------------------------------------
//Implementation from https://cp-algorithms.com/graph/dijkstra.html#implementation
void dijkstra_dense(Grafo& g, vector<int> & d) {
    d.assign(N, INF);
    vector<bool> S(N, false);

    d[s] = 0;
    for (int i = 0; i < N; i++) {
        int v = -1;
        for (int j = 0; j < N; j++) {
            if (!S[j] && (v == -1 || d[j] < d[v]))
                v = j;
        }

        if (d[v] == INF)
            break;

        S[v] = true;
        for (auto edge : g[v]) {
            int to = edge.first;
            int len = edge.second;

            if (d[v] + len < d[to]) {
                d[to] = d[v] + len;
            }
        }
    }
}

//Implementation from https://cp-algorithms.com/graph/dijkstra_sparse.html#implementation
void dijkstra_set(Grafo& g, vector<int> & d) {
    d.assign(N, INF);
    d[s] = 0;
    set<pair<int, int>> q;
    q.insert({0, s});

    while (!q.empty()) {
        int v = q.begin()->second;
        q.erase(q.begin());

        for (auto edge : g[v]) {
            int to = edge.first;
            int len = edge.second;

            if (d[v] + len < d[to]) {
                q.erase({d[to], to});
                d[to] = d[v] + len;
                q.insert({d[to], to});
            }
        }
    }
}

void dijkstra_pq(Grafo& g, vector<int>& dist){
    vector<bool> S(N, false); //S = conjunto de vertices ya "evaluados"
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> min_heap;

    dist.assign(N, INF);
    dist[s] = 0;
    min_heap.push(make_pair(0, s));

    while (!min_heap.empty()) {
        pair<int, int> p = min_heap.top(); min_heap.pop();
        int dist_cand = p.first;
        int u = p.second;
        S[u] = true; //Marcamos como visitado
        //Chequamos si no encontramos ya un mejor camino de s a u
        if (dist[u] < dist_cand) continue;

        for (auto [v, cost] : g[u]) { //g es lista de ady.
            if (S[v]) continue; //Si el nodo fue visitado, ignorarlo
            //Relajamos los adyacentes a u no visitados
            int new_dist = dist[u] + cost; 
            if (new_dist < dist[v]) {
                dist[v] = new_dist;
                min_heap.push(make_pair(new_dist, v));
            }
        }
    }
}
//-----------------------------------------------------------------
void mejorarTrafico_pq(){
    int res;
    vector<int> dist_desdeS(N);
    vector<int> dist_desdeT(N);
    dijkstra_pq(G, dist_desdeS);
    dijkstra_pq(G_T, dist_desdeT);
    res = dist_desdeS[t];
    
    for (auto [u, v, q] : candidatos) {
        int new_dist1 = dist_desdeS[u] + ((int) q) + dist_desdeT[v];
        int new_dist2 = dist_desdeS[v] + ((int) q) + dist_desdeT[u];
        if (res > min(new_dist1, new_dist2))
            res = min(new_dist1, new_dist2);
    }

    //return res;
}

void mejorarTrafico_set(){
    int res;
    vector<int> dist_desdeS(N);
    vector<int> dist_desdeT(N);
    dijkstra_set(G, dist_desdeS);
    dijkstra_set(G_T, dist_desdeT);
    res = dist_desdeS[t];
    
    for (auto [u, v, q] : candidatos) {
        int new_dist1 = dist_desdeS[u] + ((int) q) + dist_desdeT[v];
        int new_dist2 = dist_desdeS[v] + ((int) q) + dist_desdeT[u];
        if (res > min(new_dist1, new_dist2))
            res = min(new_dist1, new_dist2);
    }

    //return res;
}

void mejorarTrafico_dense(){
    int res;
    vector<int> dist_desdeS(N);
    vector<int> dist_desdeT(N);
    dijkstra_dense(G, dist_desdeS);
    dijkstra_dense(G_T, dist_desdeT);
    res = dist_desdeS[t];
    
    for (auto [u, v, q] : candidatos) {
        int new_dist1 = dist_desdeS[u] + ((int) q) + dist_desdeT[v];
        int new_dist2 = dist_desdeS[v] + ((int) q) + dist_desdeT[u];
        if (res > min(new_dist1, new_dist2))
            res = min(new_dist1, new_dist2);
    }

    //return res;
}
//-----------------------------------------------------------------
void read_input(int size) {
    //Buscamos el input correspondiente
    string file_name = dir_path + "inputs/input_" + to_string(size);
    cout << "input_" << size << endl;
    ifstream read_file(file_name); /*abre para leer*/
    
    //(n ≤ 10^4 ), (m ≤ 10^5), (k < 300), (1 ≤ s ≤ n), (1 ≤ t ≤ n)
    read_file >> N >> M >> K >> s >> t;
    
    //Indexamos a 0
    s--;
    t--;
    //Init adjacency list
    G = Grafo(N, vector<pair<int, int>>());
    G_T = Grafo(N, vector<pair<int, int>>());
    while (M--) {
        int d, c, l; //(0 < l ≤ 1000)
        read_file >> d >> c >> l; // (d, c) = arista; costo = l
        G[d-1].push_back(make_pair(c-1, l));
        G_T[c-1].push_back(make_pair(d-1, l));
    }
    //Init candidatos
    candidatos.clear();
    while (K--) {
        int u, v, q; //(q ≤ 1000)
        read_file >> u >> v >> q;
        candidatos.push_back(make_tuple(u-1, v-1, q));
    }

    read_file.close();
}

int main() {
    //Buscamos el directorio del codigo
    string code_path = __FILE__;
    dir_path = code_path.substr(0, code_path.rfind("/")) + "/";
    string out = dir_path + "output";
    if(mkdir(out.c_str(), 0777) == 0)
        cout << "Carpeta 'output/' creada" << endl;

    //Abrimos los archivos
    ofstream of_pq, of_set, of_dense;
    of_pq.open(dir_path + "output/runtime_pq.dat");
    of_set.open(dir_path + "output/runtime_set.dat");
    of_dense.open(dir_path + "output/runtime_dense.dat");

    of_pq << "n time\n";
    of_set << "n time\n";
    of_dense << "n time\n";

    int repeat = 10;
    //int limit = pow(10,4);
    for (int n = 25; n <= 500; n += 25) {    
    //for (int power = 1; power < 5; power++) {
    //    for (int i=1; i < 10; i++) {
            //int n = i * pow(10,power);
            //if(n > limit) break;

            read_input(n);

            double counter_pq, counter_set, counter_dense = 0;
            for (int ignore=0; ignore<repeat; ignore++){
                auto start = chrono::high_resolution_clock::now();
                mejorarTrafico_pq();
                auto stop = chrono::high_resolution_clock::now();
                chrono::duration<double> diff = stop - start;
                counter_pq += diff.count();
            }
            for (int ignore=0; ignore<repeat; ignore++){
                auto start = chrono::high_resolution_clock::now();
                mejorarTrafico_set();
                auto stop = chrono::high_resolution_clock::now();
                chrono::duration<double> diff = stop - start;
                counter_set += diff.count();
            }
            for (int ignore=0; ignore<repeat; ignore++){
                auto start = chrono::high_resolution_clock::now();
                mejorarTrafico_dense();
                auto stop = chrono::high_resolution_clock::now();
                chrono::duration<double> diff = stop - start;
                counter_dense += diff.count();
            }
        
            of_pq << n << " " << counter_pq / repeat << endl;
            of_set << n << " " << counter_set / repeat << endl;
            of_dense << n << " " << counter_dense / repeat << endl;
    //    }
    }

    of_pq.close();
    of_set.close();
    of_dense.close();

    return 0;
}