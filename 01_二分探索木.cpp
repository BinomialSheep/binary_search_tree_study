/**verifyed
 * ALDS1_8_A Binary Search Tree I
 * https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=10476672#1
 *
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
};

class BinarySearchTree {
 private:
  Node *root = nullptr;

  void insert(Node *&node, const ll v) {
    if (!node) {
      node = new Node();
      node->value = v;
    } else if (v < node->value) {
      insert(node->left, v);
    } else {
      insert(node->right, v);
    }
  }

  void destroy(Node *node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
  }

 public:
  BinarySearchTree() {}

  ~BinarySearchTree() { destroy(root); }

  void insert(const ll v) { insert(root, v); }

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

int main() {
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
    } else if (command == "print") {
      tree.print();
    } else {
      assert(false);
    }
  }
}