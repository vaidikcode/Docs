#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Taking inputs
    int T;
    cin >> T;
    while (T--) {
        int n, m;
        cin >> n >> m;
        string x, s;
        cin >> x >> s;

        // Say the max on m is to be 25, 
        // and for this case n is 1
        // Means we have to do atleast 5 oprations to make s substring of 32
        // And if s does not exist in x(32) means it will not exist in x(64)
        // Means we can check at 5 operations of concatioantion
        string x2 = x + x;
        string x4 = x2 + x2;
        string x8 = x4 + x4;
        string x16 = x8 + x8;
        string x32 = x16 + x16;
        int ans = -1;
        // Here we exert if the find fnc returns not found then we not enter the case
        if (x.find(s) != string::npos) {
            ans = 0;
        } else if (x2.find(s) != string::npos) {
            ans = 1;
        } else if (x4.find(s) != string::npos) {
            ans = 2;
        } else if (x8.find(s) != string::npos) {
            ans = 3;
        } else if (x16.find(s) != string::npos) {
            ans = 4;
        } else if (x32.find(s) != string::npos) {
            ans = 5;
        }
        cout << ans << '\n';
    }
    return 0;
}