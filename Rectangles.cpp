#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <queue>
#include <deque>
#include <array>
#include <list>
#include <stack>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <string>
#include <cstring>
#include <random>
#include <bitset>
#include <functional>

using namespace std;

class seg_tree {
public:
    void build(int size) {
        sum.assign(2 * size, 0);
        add.assign(2 * size, 0);
    }

    int upd_sum(int l, int r, int y1, int y2, int v, int x) {
        if (l > y2 || r < y1) {
            return 0;
        }

        if (l >= y1 && r <= y2) {
            sum[v] += x * max(0, (min(r, y2) - max(l, y1) + 1));
            return x * max(0, (min(r, y2) - max(l, y1) + 1));
        }

        int m = (l + r) / 2;
        int k = upd_sum(l, m, y1, y2, v * 2, x) + upd_sum(m + 1, r, y1, y2, v * 2 + 1, x);
        sum[v] += k;

        return k;
    }

    void upd_sum(int y1, int y2, int value) {
        upd_sum(0, sum.size() / 2 - 1, y1, y2, 1, value);
    }

    int upd_add(int l, int r, int y1, int y2, int v, int x) {
        if (l > y2 || r < y1) {
            return 0;
        }

        if (l >= y1 && r <= y2) {
            add[v] += x;
            return 0;
        }

        int m = (l + r) / 2;
        int k = upd_add(l, m, y1, y2, v * 2, x) + upd_add(m + 1, r, y1, y2, v * 2 + 1, x);
        sum[v] += k;

        return k;
    }

    void upd_add(int y1, int y2, int value) {
        upd_add(0, sum.size() / 2 - 1, y1, y2, 1, value);
    }

    int ask_sum(int l, int r, int y1, int y2, int v) {
        if (l > y2 || r < y1) {
            return 0;
        }

        if (l >= y1 && r <= y2) {
            return sum[v];
        }

        int m = (l + r) / 2;
        if (min(r, y2) - max(l, y1) + 1 > 0) {
            return ask_sum(l, m, y1, y2, v * 2) +
                ask_sum(m + 1, r, y1, y2, v * 2 + 1) +
                add[v] * max(0, (min(r, y2) - max(l, y1) + 1));
        }
        else {
            return 0;
        }
    }

    int ask_sum(int y1, int y2) {
        return ask_sum(0, sum.size() / 2 - 1, y1, y2, 1);
    }

    int ask_add(int l, int r, int y1, int y2, int v) {
        if (l > y2 || r < y1) {
            return 0;
        }

        if (l >= y1 && r <= y2) {
            return add[v] * max(0, (min(r, y2) - max(l, y1) + 1));
        }

        int m = (l + r) / 2;
        return ask_add(l, m, y1, y2, v * 2) +
            ask_add(m + 1, r, y1, y2, v * 2 + 1) +
            add[v] * max(0, (min(r, y2) - max(l, y1) + 1));
    }

    int ask_add(int y1, int y2) {
        return ask_add(0, sum.size() / 2 - 1, y1, y2, 1);
    }
private:
    vector <int> sum;
    vector <int> add;
};

class SegmentTree2D {
public:
    SegmentTree2D(int n, int m) {
        n_ = toX(n);
        m_ = toX(m);

        TREE_.resize(2 * n_);
        for (int i = 0; i < 2 * n_; ++i) {
            seg_tree node;
            node.build(m_);
            TREE_[i] = node;
        }
    };

    void upd(int x1, int x2, int y1, int y2, int Dop) {
        upd(0, n_ - 1, x1, x2, y1, y2, 1, Dop);
    }

    int ask(int x1, int x2, int y1, int y2) {
        return ask(0, n_ - 1, x1, x2, y1, y2, 1);
    }
private:
    int n_, m_;
    vector <seg_tree> TREE_;

    int toX(int n) {
        int x = 1;
        while (x * 2 < n) {
            x *= 2;
        }
        x *= 2;
        return x;
    }

    void upd(int l, int r, int x1, int x2, int y1, int y2, int v, int X) {
        if (r < x1 || l > x2) {
            return;
        }
        else {
            TREE_[v].upd_sum(y1, y2, X * max(0, (min(r, x2) - max(l, x1) + 1)));
            if (x1 <= l && x2 >= r) {
                TREE_[v].upd_add(y1, y2, X);
                return;
            }

            int med = (l + r) / 2;
            upd(l, med, x1, x2, y1, y2, 2 * v, X);
            upd(med + 1, r, x1, x2, y1, y2, 2 * v + 1, X);
        }
    }

    int ask(int l, int r, int x1, int x2, int y1, int y2, int v) {
        if (x1 > r || x2 < l) {
            return 0;
        }
        else if (x1 <= l && x2 >= r) {
            return TREE_[v].ask_sum(y1, y2);
        }
        else {
            int med = (l + r) / 2;
            return ask(l, med, x1, x2, y1, y2, 2 * v) +
                ask(med + 1, r, x1, x2, y1, y2, 2 * v + 1) +
                TREE_[v].ask_add(y1, y2) * max(0, (min(r, x2) - max(l, x1) + 1));
        }
    }

    seg_tree& operator[](int index) {
        return TREE_[index];
    }
};

int main() {
    int n, m, q; cin >> n >> m >> q;

    SegmentTree2D ST(n, m);

    int x1, y1, x2, y2, Dop;
    for (int i = 0; i < q; ++i) {
        string s; cin >> s;
        if (s == "!") {
            cin >> x1 >> y1 >> x2 >> y2 >> Dop;
            if (x1 < 0 || x2 >= n || y1 < 0 || y2 >= m) {
                cout << "WRONG QUERY" << "\n";
            }
            else {
                ST.upd(x1, x2, y1, y2, Dop);
            }
        }
        else if (s == "?") {
            cin >> x1 >> y1 >> x2 >> y2;
            if (x1 < 0 || x2 >= n || y1 < 0 || y2 >= m) {
                cout << "WRONG QUERY" << "\n";
            }
            else {
                int ans = ST.ask(x1, x2, y1, y2);
                cout << ans << "\n";
            }
        }
    }
    return 0;
}
