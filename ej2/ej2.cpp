#include<stdio.h>
#include<vector>
#include<queue>
#include<limits.h>

//Juez en SPOJ lo acepta solo si se elige como compilador 'C++14 (gcc 8.3)'

const int INF = INT_MAX;

using namespace std;

int N, M; //2 ≤ n ≤ 300
vector<bool> Prim;

//E&E Implementation from
//https://cp-algorithms.com/graph/edmonds_karp.html#implementation

vector<vector<int>> capacity; //The matrix stores the capacity for every pair of vertices. The matrix will actually store the residual capacity of the network.
vector<vector<int>> adj; //adjacency list of the undirected graph, since we have also to use
                         //the reversed of directed edges when we are looking for augmenting paths

int bfs(int s, int t, vector<int>& parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<pair<int, int>> q;
    q.push({s, INF});

    while (!q.empty()) {
        int cur = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int next : adj[cur]) {
            if (parent[next] == -1 && capacity[cur][next]) {
                parent[next] = cur;
                int new_flow = min(flow, capacity[cur][next]);
                if (next == t)
                    return new_flow;
                q.push({next, new_flow});
            }
        }
    }

    return 0;
}

int maxflow(int n, int s, int t) {
    int flow = 0;
    vector<int> parent(n);
    int new_flow = bfs(s, t, parent);

    while (new_flow) {
        flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            capacity[prev][cur] -= new_flow; //No fue necesario tener en cuenta las direcciones de las relaciones
            capacity[cur][prev] += new_flow; //"It just works"
            cur = prev;
        }
        new_flow = bfs(s, t, parent);
    }

    return flow;
}

int main(){
    while (true) {
        scanf("%u %u", &N, &M); //scanf en lugar de cin porque supuestamente es mas rapido?
        if (N == 0 && M == 0) break;

        adj = vector<vector<int>>(N+2, vector<int>()); //fuente s=0, sumidero t=N+1
        capacity = vector<vector<int>>(N+2, vector<int>(N+2, 0));
        Prim = vector<bool>(N, false); //Esto no hace falta pero queda mas bonito (creo)
        for (int i = 0; i < N; i++) {
            unsigned int p; scanf("%u", &p);
            Prim[i] = (bool) p;
        }
        for (int i = 0; i < N; i++) {
            if (Prim[i]) { //Conectamos s con los que creen que Prim es el mejor
                adj[0].push_back(i+1);
                adj[i+1].push_back(0);
                capacity[0][i+1] = 1;
            } else { //Conectamos t con los que creen que Kruskal es el mejor
                adj[N+1].push_back(i+1);
                adj[i+1].push_back(N+1);
                capacity[i+1][N+1] = 1;
            }
        }
        //Agregamos las aristas "relaciones de amistad"
        while (M--) {
            int i, j;
            scanf("%d %d", &i, &j);
            adj[i].push_back(j);
            adj[j].push_back(i);
            capacity[i][j] = 1; //Agregamos ambas direcciones porque las relaciones
            capacity[j][i] = 1; //son bidireccionales
        }

        //Buscamos el corte mínimo en la red de flujo dado por adj y capacity ya que:
        //   Si particionames la red en dos conjuntos S y T, con s ∈ S y t ∈ T, donde
        //       · u ∈ S si el estudiante u votó a Prim como mejor algoritmo
        //       · v ∈ T si el estudiante v votó a Kruskal como mejor algoritmo
        //   Tenemos que para cada estudiante que voto a Kruskal siendo que preferia a Prim,
        // habrá una arista s → T y, analogamente para cada estudiante que voto a Prim siendo
        // que preferia a Kruskal, habrá una arista S → t. 
        //   Por otro lado tenemos que dados dos estudiantes u y v (u,v ∉ {s,t}) con una relación
        // de amistad entre ellos, su relación estará tensa sii u ∈ S y v ∈ T (o viceversa), con lo
        // cual, habrá una arista que conecta S y T por cada relación tensa.
        //   Luego, como las capacidades tienen valor 1, encontrar la minima cantidad de aristas que
        // puede haber tal que conecte un vertice de S con otro de T es lo mismo que encontrar el corte mínimo.
        
        int res = maxflow(N+2, 0, N+1);
        printf("%d\n", res);
    }
    
    return 0;
}