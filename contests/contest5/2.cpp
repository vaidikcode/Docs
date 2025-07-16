#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Computes the sum of prefix minimums
long long computePrefixMinSum(const vector<int>& a) {
    int n = a.size();
    long long sum = a[0];
    int currentMin = a[0];
    for (int i = 1; i < n; ++i) {
        currentMin = min(currentMin, a[i]);
        sum += currentMin;
    }
    return sum;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n; cin >> n;
        vector<int> a(n);
        for (int &x : a) cin >> x;

        // Compute the sum of prefix minimums
        int min_till=a[0];
        vector<int> prefix_min(n);
        for (int i = 0; i < n; ++i) {
            min_till = min(min_till, a[i]);
            prefix_min[i] = min_till;
        }

        // make prefix sum of prefix_min
        vector<long long> prefix_sum(n);
        prefix_sum[0] = prefix_min[0];
        for (int i = 1; i < n; ++i) {
            prefix_sum[i] = prefix_sum[i - 1] + prefix_min[i];
        }

        bool found = false;
        for (int i = 1; i < n-1; ++i) {
            int new_current;
            int new_sum = 0;
            new_current = a[i] + a[i+1];
            new_sum = prefix_sum[i-1] + ((prefix_min[i] > new_current) ? new_current : prefix_min[i]);
            if (new_sum < prefix_sum[n-1]) {
                found = true;
                cout << new_sum << "\n";
            }
        }
        if (!found) {
            cout << prefix_sum[n-1] << "\n";
        }
    }

    return 0;
}
