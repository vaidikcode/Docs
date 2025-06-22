#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
using i128  = __int128_t;   


void print_int128(i128 x) {
    if (x == 0) { std::cout << 0; return; }
    if (x < 0) { std::cout << '-'; x = -x; }
    std::string s;
    while (x) { s.push_back(char('0' + x % 10)); x /= 10; }
    std::reverse(s.begin(), s.end());
    std::cout << s;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int Tcs;                           
    cin >> Tcs;

    while (Tcs--) {
        int n; int64 k;
        cin >> n >> k;

        vector<int64> a(n);
        int64 totalCars = 0;
        for (int i = 0; i < n; ++i) { cin >> a[i]; totalCars += a[i]; }

        auto count_leq = [&](int64 val) -> int64 {
            int64 cnt = 0;
            for (int i = 0; i < n; ++i) {
                int64 ai = a[i];

                // sequence‑1: costs j‑k, 1 ≤ j ≤ ai        (cost ≤ val ⇔ j ≤ val + k)
                if (val + k >= 1) cnt += std::min<int64>(ai, val + k);

                // sequence‑2: costs j, j = ai+1 …           (cost ≤ val ⇔ j ≤ val)
                if (val > ai)   cnt += val - ai;
            }
            return cnt;
        };

        // ---- binary search for the threshold cost “lim” ----
        int64 lo = 1 - k;              // smallest possible marginal cost (when j = 1)
        int64 hi = 1;                  // grow until enough tickets are available
        while (count_leq(hi) < totalCars) hi <<= 1;

        while (lo < hi) {
            int64 mid = lo + ((hi - lo) >> 1);
            if (count_leq(mid) >= totalCars) hi = mid;
            else                             lo = mid + 1;
        }
        int64 lim = lo;                // first cost value with ≥ totalCars tickets

        // ---- decide how many cars each lane finally gets ----
        vector<int64> finalCnt(n);     // di for each lane
        vector<int> exactEq(n);        // tickets whose cost == lim in this lane
        int64 assigned = 0;

        for (int i = 0; i < n; ++i) {
            int64 ai   = a[i];
            int64 cnt1 = 0, cnt2 = 0;

            if (lim + k >= 1) cnt1 = std::min<int64>(ai, lim + k);
            if (lim > ai)     cnt2 = lim - ai;

            finalCnt[i] = cnt1 + cnt2;
            assigned   += finalCnt[i];

            int eq = 0;
            int64 j1 = lim + k;            // j with cost lim in sequence‑1?
            if (1 <= j1 && j1 <= ai) ++eq; // yes, exactly one such j
            if (lim > ai) ++eq;            // sequence‑2 also contributes one
            exactEq[i] = eq;
        }

        // remove surplus cars that had cost == lim
        int64 surplus = assigned - totalCars;
        for (int i = 0; surplus && i < n; ++i) {
            int64 take = std::min<int64>(exactEq[i], surplus);
            finalCnt[i] -= take;
            surplus     -= take;
        }

        // ---- compute total angriness ----
        i128 answer = i128(k) * i128(totalCars);   // constant baseline k * Σai

        for (int i = 0; i < n; ++i) {
            int64 ai = a[i];
            int64 di = finalCnt[i];

            if (di <= ai) {
                // cost  = di(di+1)/2 − k·di
                answer += i128(di) * (di + 1) / 2 - i128(k) * di;
            } else {
                // cost  = di(di+1)/2 − k·ai
                answer += i128(di) * (di + 1) / 2 - i128(k) * ai;
            }
        }

        print_int128(answer);
        cout << '\n';
    }
    return 0;
}
