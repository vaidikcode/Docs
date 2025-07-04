#include <cstdio>
const int N = 100010;
int n, m, a[N];
void solve() {
    if (m < n) {
        puts("No");
    } else {
        if (n & 1) {
            puts("Yes");
            for (int i = 0; i < n - 1; i ++) a[i] = 1;
            a[n - 1] = m - n + 1;
            for (int i = 0; i < n; i ++) printf("%d ", a[i]);
            puts("");
        } else {
            if ((m - n - 2) & 1) {
                puts("No");
            } else {
                puts("Yes");
                for (int i = 0; i < n - 2; i ++) a[i] = 1;
                a[n - 2] = (m - n + 2) / 2, a[n - 1] = (m - n + 2) / 2;
                for (int i = 0; i < n; i ++) printf("%d ", a[i]);
                puts("");
            }
        }
    }
}
int main() {
    int tt;
    scanf("%d", &tt);
    while (tt --) {
        scanf("%d%d", &n, &m);
        solve();
    }
    return 0;
}