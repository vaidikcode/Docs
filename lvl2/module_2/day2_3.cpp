#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >>n;
        int ans = 0;
        int currPos = 1;
        while (currPos <= n) {
            int nxt = min(n, (currPos * 2)-1);
            ans = max(ans, nxt - currPos + 1);
            currPos *= 2;
        }
        cout << ans << '\n';
    }

    return 0;
}