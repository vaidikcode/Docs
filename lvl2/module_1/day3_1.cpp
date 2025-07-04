#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<vector<int>> a(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            char c;
            cin >> c;
            a[i][j] = (c == '*') ? 1 : 0;
        }
    }

    // Create prefix sum of 2d array
    vector<vector<int>> pre(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            pre[i][j] = a[i][j]
                + (i > 0 ? pre[i - 1][j] : 0)
                + (j > 0 ? pre[i][j - 1] : 0)
                - (i > 0 && j > 0 ? pre[i - 1][j - 1] : 0);
        }
    }

    while (q--) {
        int l1, r1, l2, r2;
        cin >> l1 >> r1 >> l2 >> r2;
        // Convert to 0-based indices
        --l1; --r1; --l2; --r2;
        int ans = pre[l2][r2];
        if (l1 > 0) ans -= pre[l1 - 1][r2];
        if (r1 > 0) ans -= pre[l2][r1 - 1];
        if (l1 > 0 && r1 > 0) ans += pre[l1 - 1][r1 - 1];
        cout << ans << '\n';
    }
    return 0;
}