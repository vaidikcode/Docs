#include <bits/stdc++.h>
using namespace std;

int main () {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tt;
    cin >> tt;
    while (tt--) {
        int n;
        cin >> n;
        string s;
        cin >> s;

        int no_0 = 0, no_1 = 0;
        for (char c : s) {
            if (c == '0') no_0++;
            else no_1++;
        }

        if (no_0 != no_1) {
            cout << "-1\n";
            continue;
        }

        vector<int> ans;
        deque<char> dq;
        for (int i = 0; i < s.length(); i++) {
            dq.push_back(s[i]);
        }

        int d = 0;
        while (!dq.empty()) {
            if (dq.front() == dq.back()) {
                if (dq.front() == '0') {
                    dq.push_back('0');
                    dq.push_back('1');
                    ans.push_back(n-d);
                } else {
                    dq.push_front('1');
                    dq.push_front('0');
                    ans.push_back(0+d);
                }
                n+=2;
            }
            while (!dq.empty() && dq.front() != dq.back()) {
                dq.pop_front();
                dq.pop_back();
                ++d;
            }
        }

        cout << ans.size() << endl;
        if (ans.size() == 0) {
            cout << endl;
            continue;
        }
        for (auto x : ans) {
            cout << x << " ";
        }
        cout << endl;
    }
    return 0;
}