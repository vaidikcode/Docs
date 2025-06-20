#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        string cells;
        cin >> n >> cells;

        int emptyCnt = 0;          // total empty cells ('.')
        int streak   = 0;          // current consecutive empties
        bool done    = false;      // whether we've already printed

        for (int i = 0; i < n; ++i) {
            if (cells[i] == '#') {   // blocked cell
                streak = 0;
            } else {                 // empty cell
                ++streak;
                ++emptyCnt;
                if (streak == 3) {   // three in a row → answer is always 2
                    cout << 2 << '\n';
                    done = true;
                    break;
                }
            }
        }

        if (!done) {
            cout << emptyCnt << '\n';   // no three‑in‑a‑row block found
        }
    }
    return 0;
}
