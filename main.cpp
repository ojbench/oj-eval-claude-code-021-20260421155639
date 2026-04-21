#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 200005;

struct Point {
    int x, y;
};

Point p[MAXN];
int N;
long long ans = 0;

void solve(int l, int r) {
    if (l >= r) return;
    int mid = (l + r) / 2;
    solve(l, mid);
    solve(mid + 1, r);

    // Points in [l, r] are sorted by x.
    // For each i in [l, mid]:
    //   Find j in [mid+1, r] s.t. p[i], p[j] form an empty rectangle.

    // Optimized O(N^2) merge:
    for (int i = mid; i >= l; --i) {
        int y_max = 2000000001;
        int y_min = -1;
        // Blocker from (i, mid]
        for (int k = i + 1; k <= mid; ++k) {
            if (p[k].y > p[i].y) {
                if (p[k].y < y_max) y_max = p[k].y;
            } else {
                if (p[k].y > y_min) y_min = p[k].y;
            }
        }

        // Potential top-right corners in [mid+1, r]
        for (int j = mid + 1; j <= r; ++j) {
            if (p[j].y > p[i].y) {
                if (p[j].y < y_max) {
                    ans++;
                    y_max = p[j].y;
                }
            } else {
                if (p[j].y > y_min) {
                    y_min = p[j].y;
                }
            }
            if (y_min > y_max) break;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if (!(cin >> N)) return 0;
    for (int i = 1; i <= N; ++i) cin >> p[i].x >> p[i].y;
    sort(p + 1, p + N + 1, [](const Point& a, const Point& b) {
        return a.x < b.x;
    });

    solve(1, N);
    cout << ans << endl;
    return 0;
}
