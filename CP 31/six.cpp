#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Taking inputs
    int T;
    cin >> T;
    while (T--) {
        int n;                 cin >> n;
        vector<int> a(n);
        for (int &x : a) cin >> x;
        // Main ideas is to calculate frequency of every odd place and even place common element
        unordered_map<int,int> freq;
        for (int x : a) ++freq[x];
        if (freq.size() == 1) {   
            cout << "YES\n";
            continue;
        }
        if (freq.size() > 2) {             
            cout << "NO\n";
            continue;
        }
        auto it = freq.begin();
        int c1 = it->second;   ++it;
        int c2 = it->second;
        if (n % 2 == 0) {
            cout << (c1 == n/2 && c2 == n/2 ? "YES\n" : "NO\n");
        } else {
            int bigger = max(c1, c2), smaller = min(c1, c2);
            cout << (bigger == n/2 + 1 && smaller == n/2 ? "YES\n" : "NO\n");
        }
    }
    return 0;
}
