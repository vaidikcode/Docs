#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, k;               
        cin >> n >> k;
        bool found = false;
        // If ke exists then its always yes
        for (int i = 0, x; i < n; ++i) {
            cin >> x;
            if (x == k) found = true;
        }
        cout << (found ? "YES\n" : "NO\n");
    }
    return 0;
}
