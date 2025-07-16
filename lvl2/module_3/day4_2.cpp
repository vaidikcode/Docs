#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        
        // Count number of elements with bit j set for each bit position
        vector<int> cnt(30, 0);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < 30; j++) {
                if (a[i] & (1 << j)) {
                    cnt[j]++;
                }
            }
        }
        
        // If all elements are 0, any k from 1 to n works
        bool all_zero = true;
        for (int j = 0; j < 30; j++) {
            if (cnt[j] > 0) {
                all_zero = false;
                break;
            }
        }
        if (all_zero) {
            for (int k = 1; k <= n; k++) {
                cout << k << " ";
            }
            cout << "\n";
            continue;
        }
        
        // Compute GCD of all non-zero cnt[j]
        int g = 0;
        for (int j = 0; j < 30; j++) {
            if (cnt[j] > 0) {
                g = __gcd(g, cnt[j]);
            }
        }
        
        // Find all divisors of g
        vector<int> ans;
        for (int k = 1; k * k <= g; k++) {
            if (g % k == 0) {
                ans.push_back(k);
                if (k != g / k) {
                    ans.push_back(g / k);
                }
            }
        }
        sort(ans.begin(), ans.end());
        
        // Output the result
        for (int k : ans) {
            cout << k << " ";
        }
        cout << "\n";
    }
    
    return 0;
}