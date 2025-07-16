#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int tt;
    cin >> tt;
    while (tt--) {
        int a, b;
        cin >> a >> b;
        // Compute XOR of numbers from 0 to a-1 in O(1)
        int sum;
        int mod = a % 4;
        if (mod == 0) sum = 0;
        else if (mod == 1) sum = a - 1;
        else if (mod == 2) sum = 1;
        else sum = a; // mod == 3
        
        int x = sum ^ b; // Element needed to make XOR = b
        if (sum == b) {
            // Array [0, 1, ..., a-1] works
            cout << a << endl;
        } else if (x == a) {
            // Adding x = a increases MEX, so use a+2
            cout << a + 2 << endl;
        } else {
            // If x < a or x > a, array [0, 1, ..., a-1, x] works
            cout << a + 1 << endl;
        }
    }
    return 0;
}