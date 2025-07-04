#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;

        int c0 = 0, c1 = 0, c2 = 0, c3 = 0;
        for (int i = 0; i < n; i++) {
            int r = i % 4;
            if (r == 0) c0++;
            else if (r == 1) c1++;
            else if (r == 2) c2++;
            else c3++;
        }

        int pairs = min(c0, c3) + min(c1, c2);

        if (pairs % 2 == 1) cout << "Alice\n";
        else cout << "Bob\n";
    }

    return 0;
}
