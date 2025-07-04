#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (int &x : a) cin >> x;

    // Initialize the difference array
    // n+1 still fails
    vector<int> diff(n+1, 0); // Extra element to handle r+1 case

    // Create difference array of queries
    while (q--) {
        int l, r;
        cin >> l >> r;
        l--; // Convert to 0-based index
        r--; // Convert to 0-based index
        diff[l]++; // Increment start of range
        if (r + 1 < n) diff[r + 1]--; // Decrement just after end of range
    }

    // MAke prefix array of diff sum
    vector<long long> prefix(n);
    prefix[0] = diff[0];
    for (int i = 1; i < n; i++) {
        prefix[i] = prefix[i - 1] + diff[i];
    }

    // Sort in descending order
    sort(a.begin(), a.end(), greater<int>());
    sort(prefix.begin(), prefix.end(), greater<int>());

    long long ans = 0;
    for (int i = 0; i < n; i++) {
        ans += 1LL * a[i] * prefix[i];
    }

    cout << ans << '\n';
    
    return 0;
}