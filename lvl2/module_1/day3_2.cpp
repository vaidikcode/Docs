#include <bits/stdc++.h>
using namespace std;

int main() {
    // Taking inputs
    int n;
    cin >> n;
    vector<long long> a(n);
    for (auto &v : a) cin >> v;

    map<long long, long long> prefix_count_mod;
    long long sum = 0, ans = 0;
    // Create map of modula
    prefix_count_mod[0] = 1;
    for (int i = 0; i < n; ++i) {
        sum += a[i];
        prefix_count_mod[(sum%n+n)%n]++;
    }
    // Now iterate over modul and check if any frequncy is greater than 2 and equal to 2
    for (auto &p : prefix_count_mod) {
        if (p.second > 2) {
            ans += (p.second * (p.second - 1)) / 2;
        } else if (p.second == 2) {
            ans++;
        }
    }
    cout << ans;
    return 0;
}