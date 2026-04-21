#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Point {
    int x, y;
    int y_compressed;
};

int N;
vector<Point> pts;
vector<int> bit;

void update(int idx, int val) {
    for (; idx <= N; idx += idx & -idx)
        bit[idx] += val;
}

int query(int idx) {
    int sum = 0;
    for (; idx > 0; idx -= idx & -idx)
        sum += bit[idx];
    return sum;
}

int queryRange(int l, int r) {
    if (l > r) return 0;
    return query(r) - query(l - 1);
}

int find_first_after(int y) {
    int cur_s = query(y);
    int target = cur_s + 1;
    int pos = 0;
    int sum = 0;
    for (int i = 1 << 17; i > 0; i >>= 1) {
        if (pos + i <= N && sum + bit[pos + i] < target) {
            pos += i;
            sum += bit[pos];
        }
    }
    return pos + 1;
}

int find_last_before(int y) {
    int target = query(y - 1);
    if (target == 0) return 0;
    int pos = 0;
    int sum = 0;
    for (int i = 1 << 17; i > 0; i >>= 1) {
        if (pos + i <= N && sum + bit[pos + i] < target) {
            pos += i;
            sum += bit[pos];
        }
    }
    return pos + 1;
}

long long total_rects = 0;

struct J_Point {
    int y_j;
    int y_max_r;
};

struct Event {
    int y_val;
    int y_min_l;
};

void solve(int l, int r) {
    if (l >= r) return;
    int mid = l + (r - l) / 2;
    solve(l, mid);
    solve(mid + 1, r);

    int sizeL = mid - l + 1;
    int sizeR = r - mid;
    vector<int> Y_min_L(sizeL);
    vector<int> Y_max_R(sizeR);

    for (int i = mid; i >= l; --i) {
        Y_min_L[i - l] = find_first_after(pts[i].y_compressed);
        update(pts[i].y_compressed, 1);
    }
    for (int i = l; i <= mid; ++i) update(pts[i].y_compressed, -1);

    for (int j = mid + 1; j <= r; ++j) {
        Y_max_R[j - (mid + 1)] = find_last_before(pts[j].y_compressed);
        update(pts[j].y_compressed, 1);
    }
    for (int j = mid + 1; j <= r; ++j) update(pts[j].y_compressed, -1);

    vector<Event> events(sizeL);
    for (int i = l; i <= mid; ++i) {
        events[i - l] = {pts[i].y_compressed, Y_min_L[i - l]};
    }
    sort(events.begin(), events.end(), [](const Event& a, const Event& b) {
        return a.y_val < b.y_val;
    });

    vector<J_Point> j_pts(sizeR);
    for (int j = mid + 1; j <= r; ++j) {
        j_pts[j - (mid + 1)] = {pts[j].y_compressed, Y_max_R[j - (mid + 1)]};
    }
    sort(j_pts.begin(), j_pts.end(), [](const J_Point& a, const J_Point& b) {
        return a.y_max_r < b.y_max_r;
    });

    int j_ptr = 0;
    for (const auto& ev : events) {
        while (j_ptr < sizeR && j_pts[j_ptr].y_max_r < ev.y_val) {
            update(j_pts[j_ptr].y_j, 1);
            j_ptr++;
        }
        if (ev.y_val + 1 <= ev.y_min_l - 1) {
            total_rects += queryRange(ev.y_val + 1, ev.y_min_l - 1);
        }
    }

    for (int k = 0; k < j_ptr; ++k) {
        update(j_pts[k].y_j, -1);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> N)) return 0;
    pts.resize(N);
    vector<int> ys(N);
    for (int i = 0; i < N; ++i) {
        cin >> pts[i].x >> pts[i].y;
        ys[i] = pts[i].y;
    }

    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    for (int i = 0; i < N; ++i) {
        pts[i].y_compressed = lower_bound(ys.begin(), ys.end(), pts[i].y) - ys.begin() + 1;
    }

    sort(pts.begin(), pts.end(), [](const Point& a, const Point& b) {
        return a.x < b.x;
    });

    bit.assign(N + 1, 0);

    solve(0, N - 1);

    cout << total_rects << endl;

    return 0;
}
