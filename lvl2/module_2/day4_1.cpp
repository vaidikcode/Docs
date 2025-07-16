#include<bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Taking inputs
    int tt;
    cin >> tt;

    while (tt--) {
        int n, k;
        cin >> n >> k;
        vector<long long> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }

        // Get max value
        long long max_value = *max_element(a.begin(), a.end());
        long long max_power_k = 1;
        while (max_power_k < max_value) {
            max_power_k *= k;
        }

        bool found = false;
        while (max_power_k > 0) {
            int count = 0;
            for (int i = 0; i < n; i++) {
                if (a[i] >= max_power_k) {
                    a[i] -= max_power_k;
                    count++;
                }
            }
            if (count > 1) {
                found = true;
                break;
            }
            max_power_k /= k;
        }

        if (found == false && (*max_element(a.begin(), a.end()) == 0)) {
            cout << "Yes\n";
        } else {
            cout << "No\n";
        }
    }
    return 0;
}