#include <bits/stdc++.h>
using namespace std;

int main () {
    int tt;
    cin >> tt;
    while (tt--) {
        int n;
        cin >> n;

        if (n == 3) {
            cout << "1 2 3\n";
            continue;
        }

        vector<long long> a(n);

        if (n % 2 == 0) {
            int half = n / 2;

            if (half % 2 == 0) {
                int even = 2;
                for (int i = 0; i < n; i++) {
                    if (i % 2 == 0) {
                        a[i] = even;
                        even += 2;
                    } else {
                        a[i] = a[i-1] + 1;
                    }
                }
            } else {
                int even = 2;
                for (int i = 0; i < n; i++) {
                    if (i % 2 == 0) {
                        a[i] = even;
                        even += 2;
                    } else {
                        a[i] = a[i-1] + 1;
                    }
                }
                a[n - 1] ^= 1;
                a[n - 1] |= (1 << 17);
                a[n - 2] |= (1 << 17);
            }

        } else {
            int half = n / 2;

            if (half % 2 == 0) {
                int even = 2;
                for (int i = 0; i < n - 1; i++) {
                    if (i % 2 == 0) {
                        a[i] = even;
                        even += 2;
                    } else {
                        a[i] = a[i-1] + 1;
                    }
                }
                a[n - 1] = 0;
            } else {
                int even = 2;
                for (int i = 0; i < n - 1; i++) {
                    if (i % 2 == 0) {
                        a[i] = even;
                        even += 2;
                    } else {
                        a[i] = a[i-1] + 1;
                    }
                }
                a[n - 2] ^= 1;
                a[n - 2] |= (1 << 20);
                a[n - 3] |= (1 << 20);
                a[n - 1] = 0;
            }
        }

        for (int i = 0; i < n; i++) {
            cout << a[i] << " ";
        }
        cout << "\n";
    }

    return 0;
}
