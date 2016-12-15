#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <fstream>

using namespace std;

#define SZ(x)       (int)x.size()
#define REP(i, n)   for(i=0; i<n; i++)
#define INF         (1<<28)

#define Min(a, b)   (a<b?a:b)
#define debug(args...) {dbg,args; cerr<<endl;}
struct debugger{template<typename T> debugger& operator , (const T& v){cerr<<v<<"\t"; return *this; }}dbg;

double pScore[5][5];   // parsimony score
map<char, int> cm;  // mapping character a, c, g, t, -

void out(string sName, vector<double> v)
{
    int i;
    cout << sName << ": ";
    REP(i, SZ(v))
        cout << v[i] << " ";
    cout << endl;
}

void genScoreMat()
{
    ifstream score_f;
    score_f.open("score_matrix.txt");

    int i, j;
    REP(i, 5)
        REP(j, 5)
            score_f >> pScore[i][j];
//    pScore[0][0] = 0.0;   pScore[0][1] = 2.5;   pScore[0][2] = 1.0;   pScore[0][3] = 2.5;   pScore[0][4] = 50.5;
//    pScore[1][0] = 2.5;   pScore[1][1] = 0.0;   pScore[1][2] = 2.5;   pScore[1][3] = 1.0;   pScore[1][4] = 50.5;
//    pScore[2][0] = 1.0;   pScore[2][1] = 2.5;   pScore[2][2] = 0.0;   pScore[2][3] = 2.5;   pScore[2][4] = 50.5;
//    pScore[3][0] = 2.5;   pScore[3][1] = 1.0;   pScore[3][2] = 2.5;   pScore[3][3] = 0.0;   pScore[3][4] = 50.5;
//    pScore[4][0] = 50.5;   pScore[4][1] = 50.5;   pScore[4][2] = 50.5;   pScore[4][3] = 50.5;   pScore[4][4] = 10.0;

    cm['a'] = 0;
    cm['c'] = 1;
    cm['g'] = 2;
    cm['t'] = 3;
    cm['-'] = 4;

    score_f.close();
}

vector<double> getBestChar(vector<vector<double> > cur)
{
    if(SZ(cur) == 1)
        return cur[0];

    int i, j, k;
    double m1, m2;
    vector<vector<double> > next;

    for(int ind=0; ind<SZ(cur); ind+=2)
    {
        if(ind+1 >= SZ(cur))
            next.push_back(cur[ind]);
        else{
            vector<double> scr = vector<double> (5, INF);
            REP(k, 5){
                m1 = m2 = INF;
                REP(i, 5){
                    REP(j, 5){
                        m1 = pScore[i][k] + cur[ind][i];
                        m2 = pScore[j][k] + cur[ind+1][j];
                        scr[k] = Min(scr[k], m1+m2);
                    }
                }
            }
            next.push_back(scr);
        }
    }
    return getBestChar(next);
}

string indxToChar(vector<double> v)
{
    int i, indx = 0;
    double val = v[0];
    REP(i, SZ(v))
    {
        if(v[i] < val)
        {
            val = v[i];
            indx = i;
        }
    }

    if(indx == 0)   return "a";
    if(indx == 1)   return "c";
    if(indx == 2)   return "g";
    if(indx == 3)   return "t";
    return "-";
}

int main(int argc, char *argv[])
{
    argc--;
    // cout << argc << endl;
    if(argc != 2){
        cout << "Please specify input file (i.e: clustal) and concensus output file" << endl;
        return 0;
    }

    // freopen("7864612_clustal.aln", "r", stdin);
    // freopen("7864612_clustal.concensus", "w", stdout);

    int nCnt = 0;
    string sName, sSeq;
    vector<string> aln = vector<string> (20, "");
    vector<string> sSource;
    map<string, int> mp;

    ifstream clustal_f;
    ofstream consensus_f;
    clustal_f.open(argv[1]);
    consensus_f.open(argv[2]);

    getline(clustal_f, sName);
    while(clustal_f >> sName >> sSeq)
    {
        if(!mp[sName]){
            mp[sName] = ++nCnt;
            sSource.push_back(sName);
        }

        aln[mp[sName]-1] += sSeq;
    }


    for(int i=0; i < (int)sSource.size(); i++){
        // cout << sSource[i] << " >> " << aln[i] << endl;
        // cout << "-" << sSource[i] << "-" << (int)aln[i].size() << endl;
    }

    genScoreMat();

    // Input manipulator
//    sSource.clear(); aln.clear();
//    sSource.push_back("a1");
//    sSource.push_back("a2");
//    sSource.push_back("a3");
//    sSource.push_back("a4");
//    sSource.push_back("a5");
//    aln.push_back("g");
//    aln.push_back("a");
//    aln.push_back("c");
//    aln.push_back("a");
//    aln.push_back("c");

    string sConcensus = "";
    int indx;
    for(int j=0; j < (int)aln[0].size(); j++){
        vector<vector<double> > v;
        vector<double> scr = vector<double> (5, INF);
        for(int i=0; i < (int)sSource.size(); i++){
            if(aln[i][j] == '-')
                continue;
            aln[i][j] = tolower(aln[i][j]);
            indx = cm[aln[i][j]];

            scr[indx] = pScore[indx][indx];
            v.push_back(scr);
            scr[indx] = INF;
        }
        scr = getBestChar(v);
        // out("score", scr);
        sConcensus += indxToChar(scr);
    }
    // debug("concensus: ", SZ(sConcensus));
    consensus_f << ">" << argv[2] << endl;
    consensus_f << sConcensus << endl;

    clustal_f.close();
    consensus_f.close();

    return 0;
}
