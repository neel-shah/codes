//
//  Suffix_Arrays.cpp
//  Coding Practice
//
//  Created by Neel Shah on 11/27/14.
//  Copyright (c) 2014 Neel Shah. All rights reserved.
//

#include "Suffix_Arrays.h"

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

int P[20][MAXN];

struct entry
{
    int nr[2];
    int p;
} L[MAXN];

int cmp(const struct entry a, const struct entry b)
{
    return a.nr[0]==b.nr[0] ?(a.nr[1]<b.nr[1] ?1: 0): (a.nr[0]<b.nr[0] ?1: 0);
}

int suffix(char A[MAXN])
{
    ll len = strlen(A);
    for(int i = 0; i < len; i++)
        P[0][i] = A[i] - 'a';
    
    int stp, cnt;
    for(stp=1, cnt = 1; cnt < len; stp++, cnt *= 2)
    {
        for(int i=0; i < len; i++)
        {
            L[i].nr[0]=P[stp- 1][i];
            L[i].nr[1]=i+cnt<len ? P[stp -1][i+ cnt]:-1;
            L[i].p= i;
        }
        
        sort(L, L+len, cmp);
        for(int i=0; i < len; i++)
            P[stp][L[i].p] = (i > 0 && L[i].nr[0]==L[i-1].nr[0] && L[i].nr[1] == L[i- 1].nr[1]) ? P[stp][L[i-1].p] : i;
    }
    
    return stp;
}

int findLCP(int x, int y, int len, char a[MAXN])
{
    int same = 0, ans;
    int add=1, step=1;
    
    while(x+add-1<len && y+add-1<len)
    {
        if(P[step][x] == P[step][y])
        {
            same = add;
            step++;
            add *= 2;
            continue;
        }
        
        ans = same;
        for(int i=same; i+x<len && i+y<len; ++i)
        {
            if(a[i+x] != a[i+y])
                return ans;
            ans++;
        }
        return ans;
    }
    
    ans = same;
    for(int i=same; i+x<len && i+y<len; ++i)
    {
        if(a[i+x] != a[i+y])
            return ans;
        ans++;
    }
    return ans;
}

struct suff{
    int ind, suffInd;
}s[MAXN];

int cmpSuff(const struct suff a, const struct suff b)
{
    return a.suffInd < b.suffInd;
}

int main()
{
    char a[MAXN];
    scanf("%s", a);
    int len = strlen(a);
    int steps = suffix(a);
    
    for(int i=0; i<len; ++i)
    {
        s[i].ind = i;
        s[i].suffInd = P[steps-1][i];
    }
    sort(s, s+len, cmpSuff);
    
    int ans = 0;
    ans += len - s[0].ind;
    for(int i=1; i<len; ++i)
    {
        int suffLen = len - s[i].ind;
        int subtract = findLCP(s[i-1].ind, s[i].ind, len, a);
        ans += suffLen - subtract;
    }
    
    printf("%d", ans);
    return 0;
}