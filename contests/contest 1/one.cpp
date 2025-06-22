#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;  cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;
        vector<int> p(n);
        for (int i = 0; i < n; ++i) {
            cin >> p[i];
        }
        
        //main fnc
        int i = 0;
        while (i < n) {
            for (int j = i + 1; j < n; ++j) {
                int initial_angriness = p[i];
                int new_angriness = p[j] + 1;
                if (new_angriness < initial_angriness) {}
                else {i++;}
            }
        }
    }
    return 0;
}
