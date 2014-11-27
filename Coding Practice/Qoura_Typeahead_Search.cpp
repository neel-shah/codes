//
//  Qoura_Typeahead_Search.cpp
//  Coding Practice
//
//  Created by Neel Shah on 11/27/14.
//  Copyright (c) 2014 Neel Shah. All rights reserved.
//

#include "Qoura_Typeahead_Search.h"

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

struct addStrings {
    string id;
    string topic;
    float score;
    string searchString;
    int whenAdded;
    map<pair<int, char>, int> trie;
};

struct ans{
    string id;
    float score;
    int whenAdded;
};

map<string, int> commandMap;
vector<addStrings* > searchStringVector;
set<string> deletedIds;

vector<string> tokenizeAddStrings(char a[2000])
{
    vector<string> ans;
    ll len = strlen(a);
    for(ll i=0; i<len;)
    {
        string add;
        while (a[i] != ' ' && i<len) {
            add += a[i];
            i++;
        }
        
        ans.push_back(add);
        i++;
    }
    
    return ans;
}

string getIdForDeletedString(char a[2000])
{
    string ans;
    ll len = strlen(a), i=0;
    while (a[i] != ' ') {
        i++;
    }
    i++;
    
    while(i<len)
    {
        ans += a[i];
        i++;
    }
    
    return ans;
}

int getNumberOfResultsToShowForQuery(char a[2000])
{
    string ans;
    ll  i=0;
    while(a[i] != ' ') {
        i++;
    }
    i++;
    
    while (a[i] != ' ') {
        ans += a[i];
        i++;
    }
    
    return stoi(ans);
}

vector<string> getTokenizedSearchQuery(char a[2000])
{
    vector<string> ans;
    ll i=0, len = strlen(a);
    while(a[i] != ' ') {
        i++;
    }
    i++;
    while (a[i] != ' ') {
        i++;
    }
    i++;
    
    for(;i<len;)
    {
        string token;
        while (a[i]!=' ' && i<len) {
            token += a[i];
            i++;
        }
        
        ans.push_back(token);
    }
    
    return ans;
}

void makeObjectForSearchString(vector<string> tokenizedAddString, int whenAdded)
{
    struct addStrings* add = new addStrings;
    add->topic = tokenizedAddString[1];
    add->id = tokenizedAddString[2];
    add->score = stof(tokenizedAddString[3]);
    add->whenAdded = whenAdded;
    
    map<pair<int, char>, int> trie;
    int state = 0, newState=0;
    for(int i=4; i<tokenizedAddString.size(); ++i)
    {
        string token = tokenizedAddString[i];
        ll size = token.size();
        for(ll j=0; j<size; ++j)
        {
            if(trie.count(make_pair(newState, token[j])))
                newState = trie[make_pair(newState, token[j])];
            else
            {
                trie[make_pair(newState, token[j])] = ++state;
                newState = state;
            }
        }
    }
    add->trie = trie;
    
    searchStringVector.push_back(add);
}

int isPrefix(string token, map<pair<int, char>, int> trie)
{
    int state = 0;
    for(int i=0; i<token.size(); ++i)
    {
        if(trie.count(make_pair(state, token[i]))) {
            state = trie[make_pair(state, token[i])];
        } else {
            return 0;
        }
    }
    
    return 1;
}

int findSol(vector<string> tokenizedSearchQuery, map<pair<int, char>, int> trie)
{
    for(int i=0; i<tokenizedSearchQuery.size(); ++i)
    {
        string token = tokenizedSearchQuery[i];
        int prefix = isPrefix(token, trie);
        if(prefix)
            continue;
        else
            return 0;
    }
    
    return 1;
}

int cmp(const struct ans* a, const struct ans* b)
{
    if(a->score == b->score)
        return a->whenAdded > b->whenAdded;
    return a->score > b->score;
}

vector<ans*> getAnswers(vector<string> tokenizedSearchQuery)
{
    vector<ans*> answers;
    for(int i=0; i<searchStringVector.size(); ++i)
    {
        struct addStrings* add = searchStringVector[i];
        if(!deletedIds.count(add->id)) {
            int canBeAns = findSol(tokenizedSearchQuery, add->trie);
            if(canBeAns) {
                struct ans* a = new ans;
                a->id = add->id;
                a->whenAdded = add->whenAdded;
                a->score = add->score;
                answers.push_back(a);
            }
        }
    }
    
    sort(answers.begin(), answers.end(), cmp);
    return answers;
}

void parse(char a[2000], int whenAdded)
{
    int i=0;
    string command;
    while(a[i] != ' ')
    {
        command += a[i];
        i++;
    }
    
    switch(commandMap[command])
    {
        case 0: {
            vector<string> tokenizedString = tokenizeAddStrings(a);
            makeObjectForSearchString(tokenizedString, whenAdded);
        }
            break;
        case 1: {
            string deleteId = getIdForDeletedString(a);
            deletedIds.insert(deleteId);
        }
            break;
        case 2: {
            int numberOfResultsToShow = getNumberOfResultsToShowForQuery(a);
            vector<string> tokenizedSearchQuery = getTokenizedSearchQuery(a);
            vector<ans*> answers = getAnswers(tokenizedSearchQuery);
            for(int i=0; i<numberOfResultsToShow && i<answers.size(); ++i)
                cout << answers[i]->id + " ";
        }
            break;
    }
}

int main()
{
    char a[2000];
    int n, i=0;
    
    commandMap["ADD"] = 0;
    commandMap["DEL"] = 1;
    commandMap["QUERY"] = 2;
    commandMap["WQUERY"] = 3;
    
    scanf("%d", &n);
    cin.getline(a, 100);
    for(i=0; i<n; ++i)
    {
        cin.getline(a,100);
        parse(a, i);
    }
    
    return 0;
}