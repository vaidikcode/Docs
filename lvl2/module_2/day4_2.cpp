#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n), used(n, 0), ans;
    for (int i = 0; i < n; ++i) cin >> a[i];

    int cur_or = 0;

    for (int step = 0; step < n; ++step) {
        int best = -1, best_val = cur_or;
        for (int i = 0; i < n; ++i) {
            if (used[i]) continue;
            int new_or = cur_or | a[i];
            if (new_or > best_val) {
                best_val = new_or;
                best = i;
            }
        }
        if (best == -1) break; // no improvement
        ans.push_back(a[best]);
        used[best] = 1;
        cur_or = best_val;
    }

    // Add remaining unused elements in any order
    for (int i = 0; i < n; ++i)
        if (!used[i]) ans.push_back(a[i]);

    // Print result
    for (int num : ans) cout << num << " ";
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; cin >> t;
    while (t--) solve();

    return 0;
}
