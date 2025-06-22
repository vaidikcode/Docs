#include <bits/stdc++.h>
using namespace std;

int main() {
    // Taking inputs
    long long n, x;
    cin >> n >> x;
    vector<long long> a(n);
    for (auto &v : a) cin >> v;

    map<long long, long long> prefix_count;
    prefix_count[0] = 1;
    long long sum = 0, ans = 0;
    for (int i = 0; i < n; ++i) {
        sum += a[i];
        ans += prefix_count[sum - x];
        prefix_count[sum]++;
    }
    cout << ans << '\n';
    return 0;
}