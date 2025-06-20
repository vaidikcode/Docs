#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;                 cin >> n;
        vector<int> p(n);
        for (int &x : p) cin >> x;

        // Main idea here is that there is no way to move first element
        // So if to be sorted the first elemnt should be min in all
        // If already sorted → YES for this case, move to next
        if (is_sorted(p.begin(), p.end())) {
            cout << "YES\n";
            continue;
        }

        int mn = *min_element(p.begin(), p.end());
        if (p[0] == mn)
            cout << "YES\n";
        else
            cout << "NO\n";
    }
    return 0;
}
