#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int tt;
    cin >> tt;
    while (tt--) {
        // take input a string of lenght n
        int n;
        cin >> n;
        string s;
        cin >> s;
        int pair_0 = 0, pair_1 = 0;
        // count the number of pairs of 0s and 1s
        for (int i = 0; i < n - 1; i++) {
            if (s[i] == '0' && s[i + 1] == '0') {
                pair_0++;
            } else if (s[i] == '1' && s[i + 1] == '1') {
                pair_1++;
            }
        }

        cout << max(pair_0, pair_1) << endl;
    }
    return 0;
}