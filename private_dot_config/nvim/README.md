# 💤 LazyVim

A starter template for [LazyVim](https://github.com/LazyVim/LazyVim).
Refer to the [documentation](https://lazyvim.github.io/installation) to get started.

## WSL / SSHのクリップボード

WSLまたはSSH接続先では、OSC 52で接続元のクリップボードへコピーする。
`y`や`yy`でコピーし、`p`はこのNeovim内で最後にコピー・削除した内容を使う。
接続元のOSから貼り付けるときは挿入モードで端末の貼り付け操作を使う。
端末への読み取り要求を送らないため、OSC 52の読み取り非対応による待ち時間は発生しない。

`NVIM_OSC52=0 nvim`でこの設定を無効にし、`NVIM_OSC52=1 nvim`で明示的に有効にできる。
SSH環境変数を持たない既存のtmuxペインでも後者を使える。
miniPCのUbuntu VMとtmuxでの確認手順は[導入・運用手順](../../docs/miniPC.md#クリップボードの確認)を参照する。

## LaTeX (Ubuntu / WSL)

LazyVimのTeX拡張でVimTeXとtexlabを使用する。PDFビューアはZathura。
VimTeXはNeovim 0.12.3でも使用できるv2.17に固定している。

OS側の依存ツールをインストールしてから、chezmoiの設定を適用する。

```bash
sudo apt install latexmk zathura texlive-latex-base texlive-lang-japanese texlive-luatex
chezmoi apply ~/.config/nvim
```

Neovimを起動してプラグインのインストールを待ち、次のコマンドを実行する。

```vim
:MasonInstall texlab tree-sitter-cli
```

インストール後にNeovimを再起動し、`:TSInstall latex bibtex`でパーサーを導入する。
tree-sitter-cliは0.26.1以上が必要。NeovimではMasonが導入した実行ファイルを使用する。
WSLでPDFを表示するにはWSLgなどのGUI環境が必要。
SSH接続先のUbuntu VMにGUIがなければZathuraで表示できない。
その場合はVMで生成したPDFを`scp`で接続元へ取得して表示する。

日本語文書の例:

```tex
%! TeX program = lualatex
\documentclass{ltjsarticle}
\begin{document}
日本語の文書です。
\[ E = mc^2 \]
\end{document}
```

`.tex`を開いて保存し、ノーマルモードで以下のキーを順番に押す。

| キー | 操作 |
|---|---|
| `\ll` | 自動コンパイル開始／停止。開始後は保存するたびにPDFを更新する |
| `\lv` | PDFをZathuraの別ウィンドウで表示・現在位置へ移動 |
| `\le` | コンパイルエラー一覧 |
| `\li` | プロジェクト情報 |
