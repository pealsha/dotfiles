#pragma once

#include <bits/stdc++.h>

namespace debug_internal {
    using namespace std;

    template <class T>
    struct is_structured : false_type {};

    template <class T, class Alloc>
    struct is_structured<vector<T, Alloc>> : true_type {};

    template <class T, class Alloc>
    struct is_structured<deque<T, Alloc>> : true_type {};

    template <class T, size_t N>
    struct is_structured<array<T, N>> : true_type {};

    template <class T, class Compare, class Alloc>
    struct is_structured<set<T, Compare, Alloc>> : true_type {};

    template <class T, class Compare, class Alloc>
    struct is_structured<multiset<T, Compare, Alloc>> : true_type {};

    template <class T, class Hash, class Equal, class Alloc>
    struct is_structured<unordered_set<T, Hash, Equal, Alloc>> : true_type {};

    template <class K, class V, class Compare, class Alloc>
    struct is_structured<map<K, V, Compare, Alloc>> : true_type {};

    template <class K, class V, class Hash, class Equal, class Alloc>
    struct is_structured<unordered_map<K, V, Hash, Equal, Alloc>> : true_type {};

    template <class T, class Container>
    struct is_structured<queue<T, Container>> : true_type {};

    template <class T, class Container>
    struct is_structured<stack<T, Container>> : true_type {};

    template <class T, class Container, class Compare>
    struct is_structured<priority_queue<T, Container, Compare>> : true_type {};

    template <class T, class U>
    struct is_structured<pair<T, U>>
        : bool_constant<is_structured<T>::value || is_structured<U>::value> {};

    template <class... Ts>
    struct is_structured<tuple<Ts...>>
        : bool_constant<(is_structured<Ts>::value || ...)> {};

    template <class T>
    inline constexpr bool is_structured_v = is_structured<remove_cvref_t<T>>::value;

    void print_debug(bool x, size_t depth);

    template <class T>
    void print_debug(const T& x, size_t depth);

    template <class T, class U>
    void print_debug(const pair<T, U>& p, size_t depth);

    template <class... Ts>
    void print_debug(const tuple<Ts...>& t, size_t depth);

    template <class T, size_t N>
    void print_debug(const array<T, N>& a, size_t depth);

    template <class T, class Alloc>
    void print_debug(const vector<T, Alloc>& v, size_t depth);

    template <class T, class Alloc>
    void print_debug(const deque<T, Alloc>& d, size_t depth);

    template <class T, class Compare, class Alloc>
    void print_debug(const set<T, Compare, Alloc>& s, size_t depth);

    template <class T, class Compare, class Alloc>
    void print_debug(const multiset<T, Compare, Alloc>& s, size_t depth);

    template <class T, class Hash, class Equal, class Alloc>
    void print_debug(const unordered_set<T, Hash, Equal, Alloc>& s, size_t depth);

    template <class K, class V, class Compare, class Alloc>
    void print_debug(const map<K, V, Compare, Alloc>& mp, size_t depth);

    template <class K, class V, class Hash, class Equal, class Alloc>
    void print_debug(const unordered_map<K, V, Hash, Equal, Alloc>& mp, size_t depth);

    template <class T, class Container>
    void print_debug(queue<T, Container> q, size_t depth);

    template <class T, class Container>
    void print_debug(stack<T, Container> st, size_t depth);

    template <class T, class Container, class Compare>
    void print_debug(priority_queue<T, Container, Compare> pq, size_t depth);

    inline void indent(size_t depth) {
        for (size_t i = 0; i < depth; ++i) cerr << "  ";
    }

    inline void print_debug(bool x, size_t) {
        cerr << (x ? "true" : "false");
    }

    template <class T>
    void print_debug(const T& x, size_t) {
        cerr << x;
    }

    template <class Range>
    void print_range(const Range& range, size_t depth) {
        if (range.empty()) {
            cerr << "[]";
            return;
        }

        using Element = remove_cvref_t<decltype(*begin(range))>;
        constexpr bool multiline = is_structured_v<Element>;

        cerr << "[";
        if constexpr (multiline) cerr << '\n';

        size_t index = 0;
        for (const auto& value : range) {
            if constexpr (multiline) {
                indent(depth + 1);
            } else if (index) {
                cerr << ", ";
            }

            print_debug(value, depth + 1);
            if constexpr (multiline) {
                if (++index < range.size()) cerr << ",";
                cerr << '\n';
            } else {
                ++index;
            }
        }

        if constexpr (multiline) indent(depth);
        cerr << "]";
    }

    template <class T, class U>
    void print_debug(const pair<T, U>& p, size_t depth) {
        constexpr bool multiline = is_structured_v<T> || is_structured_v<U>;
        if constexpr (multiline) {
            cerr << "(\n";
            indent(depth + 1);
            print_debug(p.first, depth + 1);
            cerr << ",\n";
            indent(depth + 1);
            print_debug(p.second, depth + 1);
            cerr << '\n';
            indent(depth);
            cerr << ")";
        } else {
            cerr << "(";
            print_debug(p.first, depth);
            cerr << ", ";
            print_debug(p.second, depth);
            cerr << ")";
        }
    }

    template <class... Ts>
    void print_debug(const tuple<Ts...>& t, size_t depth) {
        constexpr bool multiline = (is_structured_v<Ts> || ...);
        cerr << "(";
        if constexpr (multiline) cerr << '\n';

        size_t index = 0;
        apply([&](const auto&... values) {
            (([&] {
                if constexpr (multiline) {
                    indent(depth + 1);
                } else if (index) {
                    cerr << ", ";
                }
                print_debug(values, depth + 1);
                if constexpr (multiline) {
                    if (++index < sizeof...(Ts)) cerr << ",";
                    cerr << '\n';
                } else {
                    ++index;
                }
            }()), ...);
        }, t);

        if constexpr (multiline) indent(depth);
        cerr << ")";
    }

    template <class T, size_t N>
    void print_debug(const array<T, N>& a, size_t depth) {
        print_range(a, depth);
    }

    template <class T, class Alloc>
    void print_debug(const vector<T, Alloc>& v, size_t depth) {
        print_range(v, depth);
    }

    template <class T, class Alloc>
    void print_debug(const deque<T, Alloc>& d, size_t depth) {
        print_range(d, depth);
    }

    template <class T, class Compare, class Alloc>
    void print_debug(const set<T, Compare, Alloc>& s, size_t depth) {
        print_range(s, depth);
    }

    template <class T, class Compare, class Alloc>
    void print_debug(const multiset<T, Compare, Alloc>& s, size_t depth) {
        print_range(s, depth);
    }

    template <class T, class Hash, class Equal, class Alloc>
    void print_debug(const unordered_set<T, Hash, Equal, Alloc>& s, size_t depth) {
        print_range(s, depth);
    }

    template <class Map>
    void print_map(const Map& mp, size_t depth) {
        if (mp.empty()) {
            cerr << "{}";
            return;
        }

        using Key = typename Map::key_type;
        using Value = typename Map::mapped_type;
        constexpr bool multiline = is_structured_v<Key> || is_structured_v<Value>;

        cerr << "{";
        if constexpr (multiline) cerr << '\n';

        size_t index = 0;
        for (const auto& [key, value] : mp) {
            if constexpr (multiline) {
                indent(depth + 1);
            } else if (index) {
                cerr << ", ";
            }

            print_debug(key, depth + 1);
            cerr << ": ";
            print_debug(value, depth + 1);
            if constexpr (multiline) {
                if (++index < mp.size()) cerr << ",";
                cerr << '\n';
            } else {
                ++index;
            }
        }

        if constexpr (multiline) indent(depth);
        cerr << "}";
    }

    template <class K, class V, class Compare, class Alloc>
    void print_debug(const map<K, V, Compare, Alloc>& mp, size_t depth) {
        print_map(mp, depth);
    }

    template <class K, class V, class Hash, class Equal, class Alloc>
    void print_debug(const unordered_map<K, V, Hash, Equal, Alloc>& mp, size_t depth) {
        print_map(mp, depth);
    }

    template <class T, class Container>
    void print_debug(queue<T, Container> q, size_t depth) {
        vector<T> values;
        while (!q.empty()) {
            values.push_back(q.front());
            q.pop();
        }
        print_debug(values, depth);
    }

    template <class T, class Container>
    void print_debug(stack<T, Container> st, size_t depth) {
        vector<T> values;
        while (!st.empty()) {
            values.push_back(st.top());
            st.pop();
        }
        print_debug(values, depth);
    }

    template <class T, class Container, class Compare>
    void print_debug(priority_queue<T, Container, Compare> pq, size_t depth) {
        vector<T> values;
        while (!pq.empty()) {
            values.push_back(pq.top());
            pq.pop();
        }
        print_debug(values, depth);
    }

    inline string_view trim(string_view text) {
        while (!text.empty() && isspace(static_cast<unsigned char>(text.front()))) {
            text.remove_prefix(1);
        }
        while (!text.empty() && isspace(static_cast<unsigned char>(text.back()))) {
            text.remove_suffix(1);
        }
        return text;
    }

    inline vector<string_view> split_names(string_view names) {
        vector<string_view> result;
        size_t start = 0;
        int round = 0, square = 0, curly = 0;
        char quote = 0;
        bool escaped = false;

        for (size_t i = 0; i < names.size(); ++i) {
            const char c = names[i];
            if (quote) {
                if (escaped) escaped = false;
                else if (c == '\\') escaped = true;
                else if (c == quote) quote = 0;
                continue;
            }
            if (c == '\'' || c == '"') quote = c;
            else if (c == '(') ++round;
            else if (c == ')') --round;
            else if (c == '[') ++square;
            else if (c == ']') --square;
            else if (c == '{') ++curly;
            else if (c == '}') --curly;
            else if (c == ',' && round == 0 && square == 0 && curly == 0) {
                result.push_back(trim(names.substr(start, i - start)));
                start = i + 1;
            }
        }
        if (!names.empty()) result.push_back(trim(names.substr(start)));
        return result;
    }

    inline void debug_out(string_view) {}

    template <class... Ts>
    void debug_out(string_view names, const Ts&... values) {
        const auto labels = split_names(names);
        size_t index = 0;
        auto print_named = [&](const auto& value) {
            cerr << (index < labels.size() ? labels[index] : "?") << " = ";
            print_debug(value, 0);
            cerr << '\n';
            ++index;
        };
        (print_named(values), ...);
    }
}

#define dbg(...) do { \
    std::cerr << "[debug]\n"; \
    debug_internal::debug_out(#__VA_ARGS__ __VA_OPT__(,) __VA_ARGS__); \
} while (0)
