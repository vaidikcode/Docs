#include <iostream>
#include <cmath>
using namespace std;

bool canBobWin(int a, int x, int y) {
    int d1 = abs(a - x);
    int d2 = abs(a - y);

    for (int b = 1; b <= 100; b++) {
        if (b == a) continue;
        int dbx = abs(b - x);
        int dby = abs(b - y);
        if (dbx < d1 && dby < d2) {
            return true;
        }
    }
    return false;
}

int main() {
    int t;
    cin >> t;
    while (t--) {
        int a, x, y;
        cin >> a >> x >> y;
        if (canBobWin(a, x, y)) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }
    return 0;
}
