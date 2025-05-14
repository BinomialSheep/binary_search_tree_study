#!/usr/bin/env python3
"""
usage:
    # JSON をファイルで渡す
    $ python rb_tree_viz.py tree.json            # DOT を標準出力
    $ python rb_tree_viz.py tree.json -o out.png # PNG を出力

    # 標準入力で渡す
    $ cat tree.json | python rb_tree_viz.py

sample:
[
  { "key": 99, "left": 1, "right": 4, "parent": -1, "color": "BLACK" },
  { "key": 1, "left": -1, "right": -1, "parent": 99, "color": "RED" },
  { "key": 4, "left": -1, "right": -1, "parent": 99, "color": "RED" }
]

"""
import argparse
import json
import sys
from pathlib import Path

try:
    from graphviz import Digraph
except ImportError:
    sys.exit("graphviz パッケージが入っていません。`pip install graphviz` をどうぞ。")

def load_nodes(src):
    """
    一般的な 2 形式を許容:
      1. 配列: [{"key":..}, ...]
      2. 連想: { "0": {...}, "1": {...} }
    """
    data = json.load(src)
    if isinstance(data, dict):
        data = list(data.values())
    return data

def build_graph(nodes):
    g = Digraph(format="png")
    g.attr("node", shape="circle", style="filled", fontname="Helvetica")

    # root 判定（parent == -1）もして太枠に
    for n in nodes:
        key   = n["key"]
        color = "red" if n["color"].upper() == "RED" else "black"
        is_root = (n["parent"] == -1)

        g.node(str(key),
               label=str(key),
               fillcolor=color,
               fontcolor="white",
               penwidth="2" if is_root else "1")

    # edge を貼る
    for n in nodes:
        src = str(n["key"])
        for child_key in ("left", "right"):
            tgt = n[child_key]
            if tgt != -1:
                g.edge(src, str(tgt))
    return g

def main():
    p = argparse.ArgumentParser()
    p.add_argument("json", nargs="?", help="入力 JSON ファイル（省略時は stdin）")
    p.add_argument("-o", "--output", help="PNG など画像を直接保存したいときのパス")
    args = p.parse_args()

    if args.json:
        with open(args.json, encoding="utf-8") as f:
            nodes = load_nodes(f)
    else:
        nodes = load_nodes(sys.stdin)

    g = build_graph(nodes)

    if args.output:
        out_path = Path(args.output)
        out_path.write_bytes(g.pipe())  # 自動で format を判定
        print(f"画像を書き出しました → {out_path.resolve()}")
    else:
        # DOT テキストをそのまま出力
        sys.stdout.write(g.source)

if __name__ == "__main__":
    main()
