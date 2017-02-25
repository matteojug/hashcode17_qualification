#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

typedef long long ll;

typedef pair<int,int> ii;
typedef vector<int> vi;
typedef vector<ii> vii;

#define ff first
#define ss second
#define mp make_pair
#define pb push_back

#define MAXN 1000000
#define MAXC 1001
#define MAXV 10001
#define MAXE 1001

int V,E,R,C,X;
vi video_size;
vi cache_size;
vector<vector<ii> > cc; // for client i, <cache id, latency>
vi client_latency;
vector<pair<ii, int> > req;

vector<set<int> > output;
priority_queue<pair<double, ii> > pq[MAXC];

vector<map<int,int> > rxc;

set<int> changed;

#define EXP (1./2) // FF=0|2
//~ #define EXP (1./1)

int pq_cnt = 0;


void update_scores(){
    pq_cnt = 0;
    for (int i = 0; i < C; i++)
        while (!pq[i].empty()) pq[i].pop();
    
    // R: <<video,endpoint>,req#>
    for (int i = 0; i < (int)rxc.size(); i++){
        auto c = rxc[i];
        for (auto v : c){
            if (changed.count(v.ff))
                rxc[i][v.ff] = 0;
        }
    }
    
    for (auto r : req){
        if (!changed.count(r.ff.ff)) continue;
            
        int lat = client_latency[r.ff.ss];
        for (auto c : cc[r.ff.ss])
            if (output[c.ff].count(r.ff.ff))
                lat = min(lat, c.ss);
        for (auto c : cc[r.ff.ss]){
            //~ cout<<c.ff<<","<<r.ff.ff<<endl;
            rxc[c.ff][r.ff.ff] += (lat-c.ss)*r.second;
        }
    }
    
    changed.clear();
    
    for (int i = 0; i < (int)rxc.size(); i++){
        auto c = rxc[i];
        for (auto v : c){
            if (video_size[v.ff] > cache_size[i]) continue;
            pq[i].push(mp( v.ss / pow(video_size[v.ff],EXP), mp(i,v.ff)));
            pq_cnt++;
        }
    }
}
bool video_taken[MAXV];
map<ii, set<int> > mappa_del_coso;
map<ii,int> nfsijfskjlg[MAXE];

int shared(int c1, int c2, int v){
    int r = 0;
    for (auto e : mappa_del_coso[mp(c1,v)])
        r += nfsijfskjlg[e][mp(c2,v)];
    return r;
}

int main(int argc, char *argv[]){
    int FUCKING_FILE = 0;
    if (argc == 1){
        if (FUCKING_FILE == 0)
            freopen("me_at_the_zoo.in","r",stdin);
        else if (FUCKING_FILE == 1)
            freopen("trending_today.in","r",stdin);
        else if (FUCKING_FILE == 2)
            freopen("videos_worth_spreading.in","r",stdin);
        else if (FUCKING_FILE == 3)
            freopen("kittens.in","r",stdin);
    } else
        freopen(argv[1],"r",stdin);
    scanf("%d%d%d%d%d", &V, &E, &R, &C, &X);
    cache_size.resize(C);
    for (int i = 0; i < C; i++) cache_size[i] = X;
    video_size.resize(V);
    for (int i = 0; i < V; i++)
        scanf("%d", &video_size[i]);
    cc.resize(E);
    client_latency.resize(E);
    for (int i = 0; i < E; i++){
        int a;
        scanf("%d%d", &client_latency[i], &a);
        cc[i].resize(a);
        for (int j = 0; j < a; j++){
            scanf("%d%d", &cc[i][j].ff, &cc[i][j].ss);
        }
    }
    req.resize(R);
    // R: <<video,endpoint>,req#>
    for (int i = 0; i < R; i++){
        scanf("%d%d%d", &req[i].ff.ff, &req[i].ff.ss, &req[i].ss);
        for (auto c : cc[req[i].ff.ss]){
            mappa_del_coso[mp(c.ff, req[i].ff.ff)].insert(req[i].ff.ss);
            nfsijfskjlg[req[i].ff.ss][mp(c.ff, req[i].ff.ff)] += req[i].ss;
        }
    }
    output.resize(C);
    rxc.resize(C);
    for (int i = 0; i < V; i++) changed.insert(i);
    
    int cnt = 1;
    while (cnt){
        update_scores();
        cnt = 0;
        priority_queue<pair<int,ii> > pqq;
        for (int i = 0; i < C; i++){
            if (pq[i].empty()) continue;
            cnt++;
            auto t = pq[i].top(); pq[i].pop();
            pqq.push(mp(t.ff, t.ss));
        }
        memset(video_taken,0,sizeof video_taken);
        
        while (!pqq.empty()){
            auto t = pqq.top(); pqq.pop();
            if (video_taken[t.ss.ss]){
                int sh = shared(video_taken[t.ss.ss], t.ss.ff, t.ss.ss);
                cout<<(sh / max(0.00001f,(float)t.ff))<<endl;
                if ((sh / max(0.00001f,(float)t.ff)) < 0.01)
                    video_taken[t.ss.ss] = 0;
                else
                if (!pq[t.ss.ff].empty()){
                    auto tt = pq[t.ss.ff].top();
                    //~ cout<<"@"<<t.ff/tt.ff<<endl;
                    if (t.ff/tt.ff > 2){
                        video_taken[t.ss.ss] = 0;
                    } else {
                        pq[t.ss.ff].pop();
                        pqq.push(mp(tt.ff, tt.ss));
                    }
                }
            }
            if (!video_taken[t.ss.ss]){
                cache_size[t.ss.ff] -= video_size[t.ss.ss];
                output[t.ss.ff].insert(t.ss.ss);
                changed.insert(t.ss.ss);
                video_taken[t.ss.ss] = t.ss.ff+1;
            }
        }
        
        cout<<cnt<<endl;
    }
    
    ll score = 0, tot = 0;
    for (auto r : req){
        int lat = client_latency[r.ff.ss];
        for (auto c : cc[r.ff.ss])
            if (output[c.ff].count(r.ff.ff))
                lat = min(lat, c.ss);
        score += (client_latency[r.ff.ss]-lat)*r.ss;
        tot += r.ss;
    }
    for (int i = 0; i < C; i++)
        if (cache_size[i] < 0) cout<<"Cache size "<<i<<" is negative pd"<<endl;
    cout<<(score*1000.0)/tot<<endl;
    
    if (FUCKING_FILE == 0)
        freopen("me_at_the_zoo.out","w",stdout);
    else if (FUCKING_FILE == 1)
        freopen("trending_today.out","w",stdout);
    else if (FUCKING_FILE == 2)
        freopen("videos_worth_spreading.out","w",stdout);
    else if (FUCKING_FILE == 3)
        freopen("kittens.out","w",stdout);
        
    printf("%d\n", C);
    for (int i = 0; i < (int)output.size(); i++){
        printf("%d", i);
        for (auto o : output[i])
            printf(" %d", o);
        puts("");
    }
    return 0;
}
