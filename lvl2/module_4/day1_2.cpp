#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tt;
    cin >> tt;
    while (tt--) {
    //
    // 1 2 3
    // 2 3
    // 2

    // 1 2 3 4
    // 2 3 4
    // 2 4

    // 1 2 3 4 5 6
    // 2 3 4 5 6
    // 2 4 5 6
    // 2 4 6

    // 1 2 3 4 5 6 7 
    // 2 3 4 5 6 7
    // 2 4 5 6 7
    // 2 4 6 7
    // 2 4 6

    // 100
    // 20
    // 10
    // 2
    // 100
    
    // 1 2 3 4 5 6 7 8 
    // 2 3 4 5 6 7 8
    // 2 4 5 6 7 8
    // 2 4 6 7 8
    // 2 4 6 8

    // 1000
    int n, x;
    cin >> n >> x;
    int ans = 2*(x);
    cout << ans << "\n";
    }
    return 0;
}