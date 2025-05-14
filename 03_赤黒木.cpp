
/**
 * WARN: deleteはunverify
 *
 * WARN: 古いコンパイラではformatをコメントアウトする必要がある
 *
 * insert, find
 * distinct にせよ
 * https://algo-method.com/tasks/9580pym/submissions/1730786
 * Associative Array
 * https://judge.yosupo.jp/submission/285426
 */
#include <bits/stdc++.h>

#include <format>
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

const int RED = 0, BLACK = 1;
struct Node {
  ll key;
  ll value;
  int color;
  Node *left = nullptr;
  Node *right = nullptr;
  Node *parent = nullptr;
};

class RedBlackTree {
 private:
  Node *root = nullptr;

  void destroy(Node *node) {
    if (node == NIL) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
  }

 public:
  Node *NIL = nullptr;

  RedBlackTree() {
    NIL = new Node();
    NIL->color = BLACK;
    root = NIL;
  }

  ~RedBlackTree() {
    destroy(root);
    delete NIL;
  }

  // nodeの左子をnodeの親にする回転
  Node *right_rotate(Node *node) {
    Node *node2 = node->left;
    node2->parent = node->parent;
    if (node2->parent != NIL && node2->parent->left == node) {
      node2->parent->left = node2;
    }
    if (node2->parent != NIL && node2->parent->right == node) {
      node2->parent->right = node2;
    }
    node->left = node2->right;
    if (node2->right != NIL) node2->right->parent = node;
    node2->right = node;
    node->parent = node2;
    if (node2->parent == NIL) {
      root = node2;
    }
    return node2;
  }
  // nodeの右子をnodeの親にする回転
  Node *left_rotate(Node *node) {
    Node *node2 = node->right;
    node2->parent = node->parent;
    if (node2->parent != NIL && node2->parent->left == node) {
      node2->parent->left = node2;
    }
    if (node2->parent != NIL && node2->parent->right == node) {
      node2->parent->right = node2;
    }
    node->right = node2->left;
    if (node2->left != NIL) node2->left->parent = node;
    node2->left = node;
    node->parent = node2;
    if (node2->parent == NIL) {
      root = node2;
    }
    return node2;
  }

  // 色の変更と回転をして赤黒木条件を保つ
  void rb_insert_fixup(Node *node) {
    // nodeは赤色なので、親が赤だと条件を満たさない。
    // 逆に、親が黒の場合は条件を満たす状態を保ちながら操作する

    while (node->parent->color == RED) {
      // NODE：rootの色は黒なので、親が赤なら親はrootでない（つまり祖父が存在する）
      Node *grandpa = node->parent->parent;
      // 親が祖父の左子の場合
      if (node->parent == grandpa->left) {
        Node *uncle = grandpa->right;
        if (uncle->color == RED) {
          // ケース１：叔父も赤の場合
          // 親と叔父を黒くして、祖父を赤くする
          node->parent->color = uncle->color = BLACK;
          grandpa->color = RED;
          node = grandpa;
        } else {
          if (node == node->parent->right) {
            // ケース２：赤の右子が赤の場合
            // 左回転
            node = node->parent;
            left_rotate(node);
          }
          // ケース３：赤の左子が赤の場合
          // 親側を黒にして、回転後の右子を赤にして、右回転
          node->parent->color = BLACK;
          grandpa->color = RED;
          right_rotate(grandpa);
        }
      } else {
        // 親が祖父の右子の場合
        Node *uncle = grandpa->left;
        if (uncle->color == RED) {
          // ケース１：叔父も赤の場合
          node->parent->color = uncle->color = BLACK;
          grandpa->color = RED;
          node = grandpa;
        } else {
          if (node == node->parent->left) {
            // ケース２：赤の左子が赤の場合
            node = node->parent;
            right_rotate(node);
          }
          // ケース３：赤の右子が赤の場合
          node->parent->color = BLACK;
          grandpa->color = RED;
          left_rotate(grandpa);
        }
      }
    }
    root->color = BLACK;
  }

  void insert(const ll key, ll value = -1) {
    Node *new_node = new Node();
    new_node->key = key;
    new_node->value = value;
    new_node->color = RED;
    new_node->left = NIL;
    new_node->right = NIL;

    // 現在の比較ノード
    Node *node = root;
    // new_nodeの親になる
    Node *parent = NIL;
    // 番兵に到達するまで降下する
    while (node != NIL) {
      parent = node;
      if (key <= node->key) {
        node = node->left;
      } else {
        node = node->right;
      }
    }
    // 正しい場所に到達したので挿入
    new_node->parent = parent;
    if (parent == NIL) {
      // 最初の1要素だった
      root = new_node;
    } else if (key <= parent->key) {
      parent->left = new_node;
    } else {
      parent->right = new_node;
    }

    // 赤黒木条件を保つ
    rb_insert_fixup(new_node);
  }

  Node *find(const ll key, Node *subtree_root = nullptr) {
    auto f = [&](auto rec, Node *&node, ll v) -> Node * {
      if (node == NIL) return NIL;
      if (node->key == v) return node;
      if (node->key > v) {
        return rec(rec, node->left, v);
      } else {
        return rec(rec, node->right, v);
      }
    };
    return f(f, subtree_root ? subtree_root : root, key);
  }

  // TODO
  void erase(const ll key) {
    Node *delete_node = find(key);
    // 木にkeyが存在しなかった
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
      }
    };
    f(f, delete_node);
    delete delete_node;
  }

  vector<ll> inorder_tree_walk() {
    vector<ll> ret;
    auto dfs = [&](auto rec, Node *&node) -> void {
      if (node == NIL) return;
      rec(rec, node->left);
      ret.push_back(node->key);
      rec(rec, node->right);
    };
    dfs(dfs, root);
    return ret;
  }

  vector<ll> preorder_tree_walk() {
    vector<ll> ret;
    auto dfs = [&](auto rec, Node *&node) -> void {
      if (node == NIL) return;
      ret.push_back(node->key);
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
      if (node == NIL) return;
      debug_node(node);
      if (node->parent) edges.emplace_back(node->parent->key, node->key);
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

  // デバッグ用。json形式で標準エラーに書き出す
  void print_red_black_tree() {
    auto dfs = [&](auto rec, Node *&node) -> void {
      if (node == NIL) return;
      ll key = node->key;
      ll left = node->left == NIL ? -1 : node->left->key;
      ll right = node->right == NIL ? -1 : node->right->key;
      ll parent = node->parent == NIL ? -1 : node->parent->key;
      string color = node->color == BLACK ? "BLACK" : "RED";
      cerr
          << format(
                 R"(    {{"key": {}, "left": {}, "right": {}, "parent": {}, "color": "{}"}}, )",
                 key, left, right, parent, color)
          << endl;
      rec(rec, node->left);
      rec(rec, node->right);
    };

    cerr << "[" << endl;
    dfs(dfs, root);
    cerr << "]" << endl;
  }

  // デバッグ用

  void debug_node(Node *node) {
    ll key = node->key;
    ll left = node->left ? node->left->key : -1;
    ll right = node->right ? node->right->key : -1;
    ll parent = node->parent ? node->parent->key : -1;
    cerr << "v,l,r.p: " << key << ", " << left << ", " << right << ", "
         << parent << endl;
  }

  void assert_valid_tree() {
    auto dfs = [&](auto rec, Node *&node) -> void {
      if (!node) return;
      if (node->left) {
        assert(node == node->left->parent);
        assert(node->key >= node->left->key);
      }
      if (node->right) {
        assert(node == node->right->parent);
        assert(node->key <= node->right->key);
      }
      rec(rec, node->left);
      rec(rec, node->right);
    };
    dfs(dfs, root);
  }
};

void test() {
  RedBlackTree tree;
  tree.insert(3);
  tree.insert(1);
  tree.insert(4);
  tree.insert(5);
  tree.insert(9);
  tree.insert(6);
  tree.print();
  tree.print_red_black_tree();
}

void solve_9580pym() {
  RedBlackTree tree;
  int ans = 0;
  int N;
  cin >> N;
  rep(i, N) {
    int v;
    cin >> v;
    Node *node = tree.find(v);
    if (node != tree.NIL) {
      ans++;
    } else {
      tree.insert(v);
    }
  }
  cout << ans << endl;
}

// 問題：https://judge.yosupo.jp/problem/associative_array
// 提出：https://judge.yosupo.jp/submission/285426
// 1508ms。参考：std::mapは1637ms、unordered_mapは1350ms
void solve_associative_array() {
  RedBlackTree tree;
  int Q;
  cin >> Q;
  rep(qi, Q) {
    int t;
    cin >> t;
    if (t == 0) {
      ll k, v;
      cin >> k >> v;
      Node *node = tree.find(k);
      if (node == tree.NIL) {
        tree.insert(k, v);
      } else {
        node->value = v;
      }
    } else {
      ll k;
      cin >> k;
      Node *node = tree.find(k);
      cout << (node == tree.NIL ? 0 : node->value) << endl;
    }
  }
}

int main() {
  //   test();
  //   solve_9580pym();
  solve_associative_array();
}