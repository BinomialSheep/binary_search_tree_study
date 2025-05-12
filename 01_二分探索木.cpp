/**
 * insert, find, eraseについてはALDS1_8_Cでverifyed
 * https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=10476812#1
 */

#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define all(x) (x).begin(), (x).end()
using ll = long long;

struct Node {
  ll value;
  Node *left = nullptr;
  Node *right = nullptr;
  Node *parent = nullptr;
};

class BinarySearchTree {
 private:
  Node *root = nullptr;

  void destroy(Node *node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
  }

  // uを根とする部分木を、vを根とする部分木で置き換える（親方向の処理しかやらない点に注意）
  void transparent(Node *&u, Node *&v) {
    if (u->parent == nullptr) {
      root = v;
    } else if (u == u->parent->left) {
      u->parent->left = v;
    } else {
      u->parent->right = v;
    }
    if (v) v->parent = u->parent;
  }

 public:
  BinarySearchTree() {}

  ~BinarySearchTree() { destroy(root); }

  void insert(const ll value) {
    auto f = [&](auto rec, Node *&node, Node *p, const ll v) -> void {
      if (!node) {
        node = new Node();
        node->value = v;
        node->parent = p;
      } else if (v < node->value) {
        rec(rec, node->left, node, v);
      } else {
        rec(rec, node->right, node, v);
      }
    };
    f(f, root, nullptr, value);
  }

  Node *find(const ll value) {
    auto f = [](auto rec, Node *&node, ll v) -> Node * {
      if (!node) return nullptr;
      if (node->value == v) return node;
      if (node->value > v) {
        return rec(rec, node->left, v);
      } else {
        return rec(rec, node->right, v);
      }
    };
    return f(f, root, value);
  }

  void erase(const ll value) {
    Node *delete_node = find(value);
    // 木にvalueが存在しなかった
    if (!delete_node) return;

    if (!delete_node->left) {
      // 両方の子がないか、右側しか子がない
      transparent(delete_node, delete_node->right);
    } else if (!delete_node->right) {
      // 左側しか子がない
      transparent(delete_node, delete_node->left);
    } else {
      // 両方子がある
      // delete_nodeの位置に来るnode
      Node *node = tree_minimum(delete_node->right);
      if (node != delete_node->right) {
        // 孫以下の場合、孫の右の子を置いてくる必要がある
        transparent(node, node->right);
        //
        node->right = delete_node->right;
        node->right->parent = node;
      }
      transparent(delete_node, node);
      node->left = delete_node->left;
      node->left->parent = node;
    }
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
      assert(node->value);
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
      assert(node->value);
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
};

void test() {
  {
    // minimum, maximu, successor, predecessorのテスト
    BinarySearchTree tree;
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

int main() {
  test();
  BinarySearchTree tree;

  int N;
  cin >> N;
  rep(i, N) {
    string command;
    cin >> command;
    if (command == "insert") {
      int v;
      cin >> v;
      tree.insert(v);
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
    } else if (command == "minmax") {
      Node *min = tree.tree_minimum();
      Node *max = tree.tree_maximum();
      cout << min->value << " " << max->value << endl;
    } else {
      assert(false);
    }
  }
}