//
//  MinCostFlow.cpp
//  Coding Practice
//
//  Created by Neel Shah on 12/5/14.
//  Copyright (c) 2014 Neel Shah. All rights reserved.
//

#include "MinCostFlow.h"
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <string.h>
#include <string>
#include <map>
#include<stack>
#include<map>
#include<queue>
#include <math.h>
#include<set>
#include<stdint.h>
#include <utility>
#define MAXN 100005
using namespace std;
typedef long long int ll;
typedef pair<int, int> mp;
template<class T> void chmin(T &t, const T &f) { if (t > f) t = f; }
template<class T> void chmax(T &t, const T &f) { if (t < f) t = f; }

namespace MCF {
#define MAXM 1000010
#define wint int
#define cint int
    const wint wEPS = 0;
    const wint wINF = 1001001001;
    const cint cEPS = 0;
    const cint cINF = 1001001001;
    int n, m, ptr[MAXN], next[MAXM], zu[MAXM];
    wint capa[MAXM], tof;
    cint cost[MAXM], toc, d[MAXN], pot[MAXN];
    int vis[MAXN], pree[MAXN];
    void init(int _n) {
        n = _n; m = 0; memset(ptr, ~0, n << 2);
    }
    void ae(int u, int v, wint w, cint c) {
        next[m] = ptr[u]; ptr[u] = m; zu[m] = v; capa[m] = w; cost[m] = +c; ++m;
        next[m] = ptr[v]; ptr[v] = m; zu[m] = u; capa[m] = 0; cost[m] = -c; ++m;
    }
    bool solve(int src, int ink, wint flo = wINF) {
        wint f;
        cint c, cc;
        int i, u, v;
        memset(pot, 0, n * sizeof(cint));
        //*
        for (bool cont = 1; cont; ) {
            cont = 0;
            for (u = 0; u < n; ++u) for (i = ptr[u]; ~i; i = next[i]) if (capa[i] > wEPS) {
                if (pot[zu[i]] > pot[u] + cost[i] + cEPS) {
                    pot[zu[i]] = pot[u] + cost[i]; cont = 1;
                }
            }
        }
        //*/
        for (toc = 0, tof = 0; tof + wEPS < flo; ) {
            typedef pair<cint,int> node;
            priority_queue< node,vector<node>,greater<node> > q;
            for (u = 0; u < n; ++u) { d[u] = cINF; vis[u] = 0; }
            for (q.push(mp(d[src] = 0, src)); !q.empty(); ) {
                c = q.top().first; u = q.top().second; q.pop();
                if (vis[u]++) continue;
                for (i = ptr[u]; ~i; i = next[i]) if (capa[i] > wEPS) {
                    cc = c + cost[i] + pot[u] - pot[v = zu[i]];
                    if (d[v] > cc) { q.push(mp(d[v] = cc, v)); pree[v] = i; }
                }
            }
            if (!vis[ink]) return 0;
            f = flo - tof;
            for (v = ink; v != src; v = zu[i ^ 1]) { i = pree[v]; chmin(f, capa[i]); }
            for (v = ink; v != src; v = zu[i ^ 1]) { i = pree[v]; capa[i] -= f; capa[i ^ 1] += f; }
            tof += f;
            toc += f * (d[ink] - pot[src] + pot[ink]);
            for (u = 0; u < n; ++u) pot[u] += d[u];
        }
        return 1;
    }
}
