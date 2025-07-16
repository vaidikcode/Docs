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
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        
        map<long long, int> freq;
        for (int i = 0; i < n; i++) {
            freq[a[i]]++;
        }

        long long mex = 0;
        while (freq[mex] > 0) {
            mex++;
        }

        bool mexPlusOneExists = freq[mex + 1] > 0;

        if (mexPlusOneExists) {
            int l = -1, r = -1;
            for (int i = 0; i < n; i++) {
                if (a[i] == mex + 1) {
                    if (l == -1) l = i;
                    r = i;
                }
            }
            for (int i = l; i <= r; i++) {
                a[i] = mex;
            }
        } else {
            bool found = false;
            for (int i = 0; i < n; i++) {
                if (freq[a[i]] > 1 && a[i] < mex) {
                    a[i] = mex;
                    found = true;
                    break;
                }
            }
            if (!found) {
                for (int i = 0; i < n; i++) {
                    if (a[i] > mex + 1) {
                        a[i] = mex;
                        found = true;
                        break;
                    }
                }
            }
        }

        freq.clear();
        for (int i = 0; i < n; i++) {
            freq[a[i]]++;
        }

        long long newMex = 0;
        while (freq[newMex] > 0) {
            newMex++;
        }

        if (newMex == mex + 1) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }

    return 0;
}
