#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int tt; 
    cin >> tt;
    while (tt--) {
        int n;
        cin >> n;
        vector<int> a(n);
        vector<int> b(n); 
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        for (int i = 0; i < n; i++) {
            cin >> b[i];
        }
        int l, r;
        for (l = 0; l < n; l++) {
            if (b[l] != a[l]) {
                break;
            }
        }
        for (r = n - 1; r >= 0; r--) {
            if (b[r] != a[r]) {
                break;
            }
        }
        while (b[r+1] >= b[r] && r < n - 1) {
            r++;
        }
        while(b[l-1] <= b[l] && l > 0) {
            l--;
        }
        cout << l + 1 << " " << r + 1 << "\n";
    }
    return 0;
}