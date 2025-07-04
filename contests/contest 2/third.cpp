#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<long long> a(n);
        for (auto &v : a) cin >> v;

        long long mx = a.back();         // because the array is sorted non-decreasing
        long long answer = 0;

        // Two-loop + binary-search counting (O(n² log n))
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                long long sum2 = a[i] + a[j];

                // Find first k > j such that sum2 + a[k] > 2 * a[k]
                long long lo = j + 1, hi = n - 1, pos = n;
                while (lo <= hi) {
                    long long mid = (lo + hi) / 2;
                    if (sum2 + a[mid] > 2 * a[mid]) {
                        pos = mid;
                        hi = mid - 1;
                    } else {
                        lo = mid + 1;
                    }
                }
                answer += n - pos;
            }
        }
        cout << answer << '\n';
    }
    return 0;
}
