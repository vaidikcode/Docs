#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, k;
    cin >> n >> k;

    // Step 1: Decompose n into powers of 2 using binary representation
    priority_queue<int> pq;
    int sum = 0;
    for (int i = 0; (1 << i) <= n; ++i) {
        if (n & (1 << i)) {
            pq.push(1 << i); // add the power of 2 to the heap
        }
    }

    // Step 2: Check if it is even possible
    if (k < pq.size() || k > n) {
        cout << "NO\n";
        return 0;
    }

    // Step 3: Split largest values until we have exactly k parts
    while (pq.size() < k) {
        int top = pq.top();
        pq.pop();
        // Split top into two halves
        pq.push(top / 2);
        pq.push(top / 2);
    }

    // Step 4: Output the result
    cout << "YES\n";
    vector<int> result;
    while (!pq.empty()) {
        result.push_back(pq.top());
        pq.pop();
    }

    for (int x : result) cout << x << " ";
    cout << endl;

    return 0;
}
