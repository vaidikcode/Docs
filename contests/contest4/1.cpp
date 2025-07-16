#include <iostream>
#include <queue>
#include <vector>
#include <climits>
using namespace std;

int main() {
    int t;
    cin >> t;
    while (t--) {
        int a, b;
        long long x, y;
        cin >> a >> b >> x >> y;

        if (a == b) {
            cout << 0 << '\n';
            continue;
        }

        // BFS for shortest cost path since a, b <= 100
        vector<long long> cost(200, LLONG_MAX);
        cost[a] = 0;

        queue<int> q;
        q.push(a);

        while (!q.empty()) {
            int cur = q.front(); q.pop();

            // Try a + 1
            if (cur + 1 < 200 && cost[cur + 1] > cost[cur] + x) {
                cost[cur + 1] = cost[cur] + x;
                q.push(cur + 1);
            }

            // Try a ^ 1
            int xor_val = cur ^ 1;
            if (xor_val < 200 && cost[xor_val] > cost[cur] + y) {
                cost[xor_val] = cost[cur] + y;
                q.push(xor_val);
            }
        }

        if (cost[b] == LLONG_MAX)
            cout << -1 << '\n';
        else
            cout << cost[b] << '\n';
    }

    return 0;
}
