#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(nullptr);

    int q;
    cin >> q;
    while (q--) {
        int n, j, k;
        cin >> n >> j >> k;

        int value = 0;
        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
            if (j - 1 == i) {
                value = a[i];
            }
        }

        set<int, greater<int>> s(a.begin(), a.end());
        int rem = 0;
        for (auto it = s.begin(); it != s.end(); ++it) {
            rem++;
            if (*it == value) {
                break;
            }
        }

        if (rem == 1) {
            cout << "YES\n";
            break;
        } 
        else if (rem == 2) {
            cout << "YES\n";
        }
        else if (rem <= k) {
            cout << "YES\n";
        } 
        else {
            cout << "NO\n";
        }
    }

    return 0;
}
