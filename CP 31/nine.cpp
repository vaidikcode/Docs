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
        vector<int> a(n-1);
        for (int &x : a) cin >> x;
        int ans = -1 * (accumulate(a.begin(), a.end(), 0));
        cout << ans << endl;
    }
    return 0;
}