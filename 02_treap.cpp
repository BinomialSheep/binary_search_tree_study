/**
 * ALDS1_8_D Treap
 * https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=10481491#1
 * distinct にせよ
 * https://algo-method.com/tasks/9580pym
 */

#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define all(x) (x).begin(), (x).end()
using ll = long long;

template <typename T>
inline bool chmax(T &a, T b) {
  return ((a < b) ? (a = b, true) : (false));
}
template <typename T>
inline bool chmin(T &a, T b) {
  return ((a > b) ? (a = b, true) : (false));
}

static uint32_t randXor() {
  static uint32_t x = 123456789;
  static uint32_t y = 362436069;
  static uint32_t z = 521288629;
  static uint32_t w = 88675123;
  uint32_t t;
  t = x ^ (x << 11);
  x = y;
  y = z;
  z = w;
  return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

struct Node {
  ll value;
  ll priority;
  Node *left = nullptr;
  Node *right = nullptr;
  Node *parent = nullptr;
};

class Treap {
 private:
  Node *root = nullptr;

  void destroy(Node *node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
  }

 public:
  Treap() {}

  ~Treap() { destroy(root); }

  // nodeの左子をnodeの親にする回転
  Node *right_rotate(Node *node) {
    Node *node2 = node->left;
    node2->parent = node->parent;
    if (node2->parent && node2->parent->left == node) {
      node2->parent->left = node2;
    }
    if (node2->parent && node2->parent->right == node) {
      node2->parent->right = node2;
    }
    node->left = node2->right;
    if (node2->right) node2->right->parent = node;
    node2->right = node;
    node->parent = node2;
    if (!node2->parent) {
      root = node2;
    }
    return node2;
  }
  // nodeの右子をnodeの親にする回転
  Node *left_rotate(Node *node) {
    Node *node2 = node->right;
    node2->parent = node->parent;
    if (node2->parent && node2->parent->left == node) {
      node2->parent->left = node2;
    }
    if (node2->parent && node2->parent->right == node) {
      node2->parent->right = node2;
    }
    node->right = node2->left;
    if (node2->left) node2->left->parent = node;
    node2->left = node;
    node->parent = node2;
    if (!node2->parent) {
      root = node2;
    }
    return node2;
  }

  void insert(const ll value, ll priority = -1) {
    auto f = [&](auto rec, Node *&node, Node *p) -> Node * {
      if (!node) {
        node = new Node();
        node->value = value;
        node->priority = priority;
        node->parent = p;
        return node;
      } else if (value < node->value) {
        node->left = rec(rec, node->left, node);
        node->left->parent = node;
        // 子の方が優先度が高い場合、回転して上に上げる
        if (node->priority < node->left->priority) {
          node = right_rotate(node);
        }
        return node;
      } else {
        node->right = rec(rec, node->right, node);
        node->right->parent = node;
        if (node->priority < node->right->priority) {
          node = left_rotate(node);
        }
        return node;
      }
    };
    // 優先度は乱択で決める
    if (priority == -1) priority = randXor();

    f(f, root, nullptr);
  }

  Node *find(const ll value, Node *subtree_root = nullptr) {
    auto f = [](auto rec, Node *&node, ll v) -> Node * {
      if (!node) return nullptr;
      if (node->value == v) return node;
      if (node->value > v) {
        return rec(rec, node->left, v);
      } else {
        return rec(rec, node->right, v);
      }
    };
    return f(f, subtree_root ? subtree_root : root, value);
  }

  void erase(const ll value) {
    Node *delete_node = find(value);
    // 木にvalueが存在しなかった
    if (!delete_node) return;

    // 葉まで移動させてから削除する
    auto f = [&](auto rec, Node *node) -> void {
      // debug_node(node);
      // 両方の子がない場合
      if (!node->left && !node->right) {
        // これを削除する
        if (node->parent && node == node->parent->left)
          node->parent->left = nullptr;
        if (node->parent && node == node->parent->right)
          node->parent->right = nullptr;
      } else if (!node->left) {
        // 右の子を上に持ち上げる（=左回転）
        left_rotate(node);
        rec(rec, node);
      } else if (!node->right) {
        right_rotate(node);
        rec(rec, node);
      } else {
        // 両方の子を持つ場合、優先度が高い方を持ち上げる
        if (node->left->priority > node->right->priority) {
          right_rotate(node);
          rec(rec, node);
        } else {
          left_rotate(node);
          rec(rec, node);
        }
      }
    };
    f(f, delete_node);
    delete delete_node;
  }

  // 指定したnodeを根とする部分木の最も小さい値を返す。デフォルトは木全体
  Node *tree_minimum(Node *root_node = nullptr) {
    auto f = [&](auto rec, Node *&node) -> Node * {
      return node->left ? rec(rec, node->left) : node;
    };
    return f(f, root_node ? root_node : root);
  }
  Node *tree_maximum(Node *root_node = nullptr) {
    auto f = [&](auto rec, Node *&node) -> Node * {
      return node->right ? rec(rec, node->right) : node;
    };
    return f(f, root_node ? root_node : root);
  }

  Node *tree_successor(Node *node) {
    assert(node);
    // 右の子がいるならその中で一番小さい値
    if (node->right) return tree_minimum(node->right);
    // 祖先のうち右上方向に登る最初のもの
    Node *ret = node;
    while (ret->parent) {
      if (ret->parent->left == ret) {
        // 初めて右上に登ったらそれがsuccessor
        ret = ret->parent;
        break;
      } else {
        // 左上に登っている間は登り続ける
        ret = ret->parent;
      }
    }
    return ret;
  }
  Node *tree_predecessor(Node *node) {
    assert(node);
    // 左の子がいるならその中で一番大きい値
    if (node->left) return tree_maximum(node->left);
    // 祖先のうち左上方向に登る最初のもの
    Node *ret = node;
    while (ret->parent) {
      if (ret->parent->right == ret) {
        // 初めて左上に登ったらそれがsuccessor
        ret = ret->parent;
        break;
      } else {
        // 右上に登っている間は登り続ける
        ret = ret->parent;
      }
    }
    return ret;
  }

  vector<ll> inorder_tree_walk() {
    vector<ll> ret;
    auto dfs = [&](auto rec, Node *&node) -> void {
      if (!node) return;
      rec(rec, node->left);
      ret.push_back(node->value);
      rec(rec, node->right);
    };
    dfs(dfs, root);
    return ret;
  }

  vector<ll> preorder_tree_walk() {
    vector<ll> ret;
    auto dfs = [&](auto rec, Node *&node) -> void {
      if (!node) return;
      ret.push_back(node->value);
      rec(rec, node->left);
      rec(rec, node->right);
    };
    dfs(dfs, root);
    return ret;
  }

  void print() {
    auto res = inorder_tree_walk();
    for (auto v : res) cout << " " << v;
    cout << endl;
    res = preorder_tree_walk();
    for (auto v : res) cout << " " << v;
    cout << endl;
  }

  // 可視化用（https://binomialsheep.github.io/sheep-visualize-graph-beta/）
  void print_edges() {
    cerr << "print_edge" << endl;
    vector<pair<int, int>> edges;
    auto dfs = [&](auto rec, Node *&node) -> void {
      if (!node) return;
      debug_node(node);
      if (node->parent) edges.emplace_back(node->parent->value, node->value);
      rec(rec, node->left);
      rec(rec, node->right);
    };
    dfs(dfs, root);
    int n = 0;
    for (auto [v, u] : edges) {
      n = max(n, v);
      n = max(n, u);
    }
    int m = (int)edges.size();
    // ラベルは圧縮しない
    cout << n << " " << m << endl;
    for (auto [v, u] : edges) {
      cout << v << " " << u << endl;
    }
  }
  // デバッグ用

  void debug_node(Node *node) {
    ll value = node->value;
    ll left = node->left ? node->left->value : -1;
    ll right = node->right ? node->right->value : -1;
    ll parent = node->parent ? node->parent->value : -1;
    cerr << "v,l,r.p: " << value << ", " << left << ", " << right << ", "
         << parent << endl;
  }

  void assert_valid_tree() {
    auto dfs = [&](auto rec, Node *&node) -> void {
      if (!node) return;
      if (node->left) {
        assert(node == node->left->parent);
        assert(node->value >= node->left->value);
      }
      if (node->right) {
        assert(node == node->right->parent);
        assert(node->value <= node->right->value);
      }
      rec(rec, node->left);
      rec(rec, node->right);
    };
    dfs(dfs, root);
  }
};

void test() {
  {
    // minimum, maximu, successor, predecessorのテスト
    Treap tree;
    tree.insert(2);
    tree.insert(1);
    tree.insert(5);
    tree.insert(4);
    tree.insert(3);
    tree.insert(6);

    Node *node = tree.tree_minimum();
    for (int i = 1; i <= 6; i++) {
      if (node->value != i) {
        cerr << i << " " << node->value << endl;
        assert(false);
      }
      node = tree.tree_successor(node);
    }
    node = tree.tree_maximum();
    for (int i = 6; i >= 1; i--) {
      if (node->value != i) {
        cerr << i << " " << node->value << endl;
        assert(false);
      }
      node = tree.tree_predecessor(node);
    }
  }
}

void solve_ALDS1_8_D() {
  Treap tree;
  int N;
  cin >> N;
  rep(i, N) {
    string command;
    cin >> command;
    if (command == "insert") {
      int v, p;
      cin >> v >> p;
      tree.insert(v, p);
    } else if (command == "find") {
      int v;
      cin >> v;
      cout << (tree.find(v) ? "yes" : "no") << endl;
    } else if (command == "delete") {
      int v;
      cin >> v;
      tree.erase(v);
    } else if (command == "print") {
      tree.print();
      // tree.print_edges();
    } else if (command == "minmax") {
      Node *min = tree.tree_minimum();
      Node *max = tree.tree_maximum();
      cout << min->value << " " << max->value << endl;
    } else {
      assert(false);
    }
  }
}

void solve_9580pym() {
  Treap tree;
  int ans = 0;
  int N;
  cin >> N;
  rep(i, N) {
    int v;
    cin >> v;
    Node *node = tree.find(v);
    if (node) {
      ans++;
    } else {
      tree.insert(v);
    }
  }
  cout << ans << endl;
}

int main() {
  test();
  // solve_ALDS1_8_D();
  solve_9580pym();
}