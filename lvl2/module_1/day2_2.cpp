#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<long long> a(n);
    for (auto &v : a) cin >> v;
    int q;
    cin >> q;

    vector<long long> pre(n);
    pre[0] = a[0];
    for (int i = 1; i < n; ++i) {
        pre[i] = pre[i - 1] + a[i];
    }

    vector<long long> sorted_a = a;
    sort(sorted_a.begin(), sorted_a.end());
    vector<long long> pre_cost(n);
    pre_cost[0] = sorted_a[0];
    for (int i = 1; i < n; ++i) {
        pre_cost[i] = pre_cost[i - 1] + sorted_a[i];
    }

    while (q--) {
        int t, l, r;
        cin >> t >> l >> r;
        long long answer;
        l--; r--; // convert to 0-based
        if (t == 1) {
            answer = pre[r] - (l > 0 ? pre[l - 1] : 0);
        } else {
            answer = pre_cost[r] - (l > 0 ? pre_cost[l - 1] : 0);
        }
        cout << answer << '\n';
    }
    return 0;
}