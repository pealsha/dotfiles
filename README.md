# 競技プログラミング環境の統一

Ubuntu 24.04 / WSLのC++環境をchezmoiで管理する。テンプレート、デバッグ用ヘッダー、ビルドコマンド、acc設定をこのリポジトリから適用できる。

GCC 13でgnu++23を使い、atcoder-cli 2.2.0、online-judge-tools 11.5.1、online-judge-api-client 10.10.1、ACLのリビジョンを固定する。GCCのパッチバージョンと各ツールの推移的依存はOSやパッケージ管理に依存するため、実行環境全体の完全な固定ではない。

## 別端末でのセットアップ

chezmoiをインストールしたUbuntu 24.04 / WSLで実行する。

```bash
sudo apt update
sudo apt install g++-13 clangd nodejs npm pipx git
chezmoi init https://github.com/pealsha/dotfiles.git
chezmoi diff
chezmoi apply
export PATH="$HOME/.local/bin:$PATH"
cp-setup
```

この変更をリモートへpushしてから別端末で使う。既にdotfilesを導入している端末では、ソースを更新した後に`chezmoi diff`、`chezmoi apply`、`cp-setup`を実行する。

`cp-setup`は必要に応じてnpm・pipxで指定バージョンを導入し、ACLがなければ指定リビジョンを取得する。既存ACLのリビジョンが違う場合は停止する。AtCoderの認証は端末ごとに行う。Cookie、session.json、認証用スクリプトはこのリポジトリに含めない。

## 問題を解くとき

```bash
cd ~/atcoder
acc new abc123
cd abc123/a
accdebug       # main.cppをDEBUG付きでコンパイルしてmainを作る
./main         # 標準入力で動作確認
acctest        # 最適化付きでコンパイルしてtestsのサンプルを実行
acccheck       # ASan・UBSan・STLの検査付きでサンプルを実行
acc submit     # 提出
```

各ビルドコマンドにはソースとテストディレクトリも指定できる。例: `acctest solution.cpp samples`。シェルのaliasに依存せず、Bash以外のシェルからも呼び出せる。`acccheck`ではASan用にスタック上限を8 MiBにする。

## 設定の変更

共通設定とツールのバージョンは`private_dot_config/competitive-programming/env.sh`、テンプレートは`private_dot_config/atcoder-cli-nodejs/cpp/`で編集する。適用先のcppディレクトリに以前のGit管理が残っている場合も、今後の共有元はこのdotfilesとする。

コンパイラの規格やinclude先を変更する場合は、`atcoder/dot_clangd.tmpl`も合わせて変更する。ホームディレクトリはchezmoiが端末ごとに展開する。

端末固有のビルド設定は`~/.config/competitive-programming/local.sh`で上書きできる。これは管理対象に含まれず、clangd設定には自動反映されない。

## 参考

[chezmoiの適用コマンド](https://www.chezmoi.io/reference/commands/apply/)と[atcoder-cliのテンプレート設定](https://github.com/Tatamo/atcoder-cli#provisioning-templates)を参照。
