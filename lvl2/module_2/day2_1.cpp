#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    while (q--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
           cin >> a[i];
        }

        // make frequency array of a
        vector<int> freq(1024, 0);
        for (int i = 0; i < n; ++i) {
            freq[a[i]]++;
        }

        // finding k
        // initially set it to -1
        // Also make temp freq array to compare
        int k = -1;
        for (int i = 1; i < 1024; ++i) {
            vector<int> temp_freq(1024, 0);
            for (int j = 0; j < n; ++j) {
                temp_freq[a[j]^i]++;
            }
            if  (temp_freq == freq) {
                k = i;
                break;
            }
        }

        cout << k << '\n';
    }
    return 0;
}