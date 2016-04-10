#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>
#include <queue>
#include <string>
#include <cmath>
#include <map>
#include <set>
#define MOD 1e9 + 7
#define eps 1e-9
#define pb push_back
#define mp make_pair
#define ft first
#define sd second
#define sz(a) a.size()
#define loop(i, n) for(long long (i) = 0; (i) < (n) ; ++ (i))
#define pii pair<int,int>
#define pll pair<long long,long long>
#define vii vector<int>
#define vll vector<long long>  
typedef long long ll;
typedef long double ld;

using namespace std;

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.ppm", "w", stdout);
    int n,m;
    int maxx;
    cin >> n >> m;
    cin >> maxx;
    vector <int> r(n*m);
    vector <int> g(n*m);
    vector <int> b(n*m);
    loop(i,n*m) {
        cin >> r[i] >> g[i] >> b[i];
    }

    cout << "P3\n";
    cout << n << " " << m << "\n";
    cout << maxx << "\n";
    loop(i,n*m) {
        cout << r[i] << " " << g[i] << " " << b[i] << "\n";
    }
    fclose(stdout);
}