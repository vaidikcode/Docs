#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tt;
    cin >> tt;
    while (tt--) {
        int n, c;
        cin >> n >> c;
        vector<int> totalCost(n);
        for (int i = 0; i < n; i++) {
            int ai;
            cin >> ai;
            totalCost[i] = (i + 1) + ai;
        }

        sort(totalCost.begin(), totalCost.end());

        int cnt = 0;
        for (int i = 0; i < n; i++) {
            if (c >= totalCost[i]) {
                c -= totalCost[i];
                cnt++;
            } else {
                break;
            }
        }

        cout << cnt << '\n';
    }

    return 0;
}
