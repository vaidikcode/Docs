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
        vector<int> arr(n);
        for (int i = 0; i < n; ++i) {
            cin >> arr[i];
        }

        // Make y vector of n size and initilase it as 0
        vector<int> y(n, 0);
        // Iterate through bits
        for (int i = 0; i < 30; ++i) {
            bool found = false;
            for (int j = 0; j < n; ++j) {
                if (arr[j] & (1 << i) && !found) {
                    found = true;
                }
                else if ((arr[j] & (1 << i)) == 0 && found) {
                    y[j] += (1 << i);
                }
            }
        }

        // Print the y vector
        for (int i = 0; i < n; ++i) {
            cout << y[i] << " ";
        }

        cout << '\n';
    }

    return 0;
}