#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    sort(a.begin(), a.end());
    long long median = (n%2 == 0)? ((a[n / 2 - 1] + a[n / 2]) / 2) : a[n / 2];
    long long cost = 0;
    for (int i = 0; i < n; i++) {
        cost += abs(a[i] - median);
    }

    cout << cost;
    return 0;
}