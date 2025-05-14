# 二分探索木 習作

色んな二分探索木を書く

## 参考

- アルゴリズムイントロダクション第 4 版
- みんなのデータ構造

## verify について

### 専用

#### ALDS1_8_C Binary Search Tree III

https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_8_C&lang=jp

非平衡な二分探索木でも通るようになっている。
挿入、検索、削除、昇順出力の verify ができる。
ただし問題文中の疑似コードに従って実装する必要がある。

#### ALDS1_8_D Treap

https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=10481491#1

Treap の verify 用。
問題文中の疑似コードに従って実装する必要がある。
優先度が入力として与えられる。

### insert, find

[distinct にせよ](https://algo-method.com/tasks/9580pym)

多分ちゃんと平衡になってなくても通るけど。

[Associative Array](https://judge.yosupo.jp/problem/associative_array)

こっちはちゃんと実装しないと落としてくれそう。key,value ストアを実装する必要がある。
