#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, q;
    cin >> n >> k >> q;

    vector<int> a(200001, 0);
    while (n--) {
        int l, r;
        cin >> l >> r;
        a[l]++;
        if (r + 1 < 200001) a[r + 1]--;
    }
    
    // make prefix of diff array
    vector<int> diff_prefix(200001, 0);
    diff_prefix[0] = a[0];
    for (int i = 1; i < 200001; i++) {
        diff_prefix[i] = diff_prefix[i - 1] + a[i];
        if (diff_prefix[i] >= k) {
            diff_prefix[i] = 1; // Cap the value at k
        }
        else {
            diff_prefix[i] = 0; // Otherwise, set to 0
        }
    }

    // Now make prefix sum of diff_prefix
    vector<long long> prefix(200001, 0);
    prefix[0] = diff_prefix[0];
    for (int i = 1; i < 200001; i++) {
        prefix[i] = prefix[i - 1] + diff_prefix[i];
    }

    while (q--) {
        int l, r;
        cin >> l >> r;
        l;
        r;
        cout << prefix[r] - (l > 0 ? prefix[l - 1] : 0) << '\n';
    }

    return 0;
}