#include <bits/stdc++.h>
using namespace std;

int main() {
    // Taking inputs
    int n, q;
    cin >> n >> q;
    vector<long long> a(n);
    for (auto &x : a) {cin >> x;}
    // Making prefix sum vector
    // T.S: O(n)
    vector<long long> ps(n);
    ps[0] = a[0];
    for (int i = 1; i < n; i++) {
        ps[i] = ps[i - 1] + a[i];
    }
    // Working on queries
    while (q--) {
        // Taking inputs
        // T.S: O(1) for each query
        // T.T: O(q)
        // Total T.T: O(n + q)
        // Total T.S: O(n)
        long long l, r;
        cin >> l >> r;
        l = l-1;
        r = r-1;
        cout << ps[r] - (l > 0 ? ps[l - 1] : 0) << endl;
    }
    return 0;
}