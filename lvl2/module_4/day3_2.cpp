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
        vector<long long> a(n);
        long long prefix_sum = 0, result = 0;

        for (int i = 0; i < n; ++i)
            cin >> a[i];

        for (int i = 0; i < n; ++i) {
            result += a[i] * i - prefix_sum;
            prefix_sum += a[i];
        }

        cout << result << '\n';
    }

    return 0;
}
