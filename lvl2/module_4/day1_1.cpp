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
        pair<int, int> a[n];
        for (int i = 0; i < n; i++) {
            cin >> a[i].first;
        }
        for (int i = 0; i < n; i++) {
            cin >> a[i].second;
        }

        sort(a, a + n);
        for (int i = 0; i < n; i++) {
            cout << a[i].first << " ";
        }
        cout << endl;
        for (int i = 0; i < n; i++) {
            cout << a[i].second << " ";
        }
        cout << endl;

    }
    return 0;
}