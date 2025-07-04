#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int x;
    cin >> x;

    int count = 0;
    while (x) {
        count += x & 1;  // Check if the last bit is 1
        x >>= 1;         // Right shift x to check next bit
    }

    cout << count << "\n";
    return 0;
}
