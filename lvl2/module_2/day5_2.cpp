#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    long long k;
    cin >> n >> k;
    vector<int> a(n);
    for (int &x : a) cin >> x;

    int ans = 0;

    // Check each bit from high to low
    for (int bit = 30; bit >= 0; bit--) {
        int cnt = 0;

        for (int i = 0; i < n; i++) {
            if (((a[i] >> bit) & 1) == 0) cnt++; // count how many DON'T have this bit
        }

        if (cnt <= k) {
            k -= cnt;
            ans |= (1 << bit);
        }
    }

    cout << ans << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tt;
    cin >> tt;
    while (tt--) solve();
    return 0;
}
