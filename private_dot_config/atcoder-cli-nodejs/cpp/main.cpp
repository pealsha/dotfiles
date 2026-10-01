#include <bits/stdc++.h>
using namespace std;

// type
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pll = pair<ll, ll>;

template <class T>
using Vec = vector<T>;

template <class T>
using VVec = Vec<Vec<T>>;

template <class T>
using pqMin = priority_queue<T, vector<T>, greater<T>>;

template <class T>
using pqMax = priority_queue<T>;

// rep(i, n): [0, n), rep(i, l, r): [l, r)
#define REP2(i, n) for (ll i = 0; i < (ll)(n); ++i)
#define REP3(i, l, r) for (ll i = (ll)(l); i < (ll)(r); ++i)
#define RREP2(i, n) for (ll i = (ll)(n) - 1; i >= 0; --i)
#define RREP3(i, l, r) for (ll i = (ll)(r) - 1; i >= (ll)(l); --i)
#define REP_SELECT(_1, _2, _3, NAME, ...) NAME
#define rep(...) REP_SELECT(__VA_ARGS__, REP3, REP2)(__VA_ARGS__)
#define rrep(...) REP_SELECT(__VA_ARGS__, RREP3, RREP2)(__VA_ARGS__)

// all
#define all(a) (a).begin(),(a).end()
#define rall(a) (a).rbegin(),(a).rend()

// constant
const ll LINF = 1LL << 60;
const int dx4[4] = {1,0,-1,0};
const int dy4[4] = {0,1,0,-1};
const int dx8[8] = {1,1,0,-1,-1,-1,0,1};
const int dy8[8] = {0,1,1,1,0,-1,-1,-1};
const int INF = 1 << 30;

// input forward declaration
template <class T>
istream& operator>>(istream& is, vector<T>& v);

template <class T, class U>
istream& operator>>(istream& is, pair<T, U>& p);

// input declaration
template <class... Ts>
void input(Ts&... xs) {
    (cin >> ... >> xs);
}

template <class T>
istream& operator>>(istream& is, vector<T>& v) {
    for (auto& x : v) is >> x;
    return is;
}

template <class T, class U>
istream& operator>>(istream& is, pair<T, U>& p) {
    is >> p.first >> p.second;
    return is;
}

template <class... Vs>
void input_cols(Vs&... vs) {
    ll n = min({(ll)vs.size()...});
    rep(i, n) {
        input(vs[i]...);
    }
}

#ifdef DEBUG
#include <debug.hpp>
#else
#define dbg(...) ((void)0)
#endif

// chmin,chmax
template <class T>
bool chmin(T& a, const T& b) {
    if (a > b) {
        a = b;
        return true;
    }
    return false;
}

template <class T>
bool chmax(T& a, const T& b) {
    if (a < b) {
        a = b;
        return true;
    }
    return false;
}

// functions
void yn(bool ok) {
    cout << (ok ? "Yes" : "No") << '\n';
}

bool is_in_grid(ll i, ll j, ll h, ll w) {
    return 0 <= i && i < h && 0 <= j && j < w;
}

ll ceil_div(ll a, ll b) {
    assert(b > 0);
    return a / b + (a % b > 0);
}

ll floor_div(ll a, ll b) {
    assert(b > 0);
    return a / b - (a % b < 0);
}

// constexpr ll MOD = 998244353;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

}
