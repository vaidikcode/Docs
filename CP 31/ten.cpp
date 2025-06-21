#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int total = 0;
        for (int i = 0; i < 10; ++i) {
            string row;
            cin >> row;
        }
        // Now we iterate through the matrix

        // When i == 0 and X we say +1
        // Similarly when j == 0 and X we say +1
        // when j == 9 and X we say +1
        // when i == 9 and X we say +1
        
        // and between cells (2,2) to (2,7) and X We way +2
        // and between cells (2,2) to (7,2) and ...
        cout << total << "\n";
    }
    return 0;
}
