# dotfiles

Ubuntu 24.04 / WSLで日常的に使うシェル、エディター、開発用コマンドの設定をchezmoiで管理する。共通の設定をこのリポジトリに保存し、各端末に適用する。

## 管理する設定

| 設定 | 内容と適用先 |
|---|---|
| [Bash](dot_bashrc) | プロンプト、履歴、補完、PATHなどを `~/.bashrc` に設定する |
| [Neovim](private_dot_config/nvim/) | LazyVimを基にしたエディター設定を `~/.config/nvim/` に配置する |
| [AtCoder用の設定](private_dot_config/atcoder-cli-nodejs/) | atcoder-cliの設定とC++の解答用テンプレートを `~/.config/atcoder-cli-nodejs/` に配置する |
| [開発用コマンド](dot_local/bin/) | C++環境のセットアップ・ビルド・テスト用スクリプトを `~/.local/bin/` に配置する |
| [C++のビルド設定](private_dot_config/competitive-programming/env.sh) | コンパイラ、規格、ツールのバージョンを `~/.config/competitive-programming/env.sh` に設定する |
| [clangd](atcoder/dot_clangd.tmpl) | AtCoder用のコンパイラ、規格、ヘッダーの検索先を `~/atcoder/.clangd` に設定する |

## 導入と更新

Ubuntu 24.04 / WSLに[chezmoiをインストール](https://www.chezmoi.io/install/)してから、次のコマンドを実行する。`chezmoi diff`で適用する変更を確認し、`chezmoi apply`で端末に反映する。

```bash
chezmoi init https://github.com/pealsha/dotfiles.git
chezmoi diff
chezmoi apply
```

設定を変更した場合は、その変更をGitHubへpushしてから別端末で使う。既にdotfilesを導入している端末では、chezmoiのソースをGitHubの内容に更新した後に、`chezmoi diff`、`chezmoi apply`を順に実行する。競技プログラミング用のツールを利用する場合は、続けて`cp-setup`を実行する。

## Bash

`~/.local/bin`と`~/.local/share/bob/nvim-bin`をPATHに追加する。Rustの環境設定、Codexの環境設定、nvmの補完は、それぞれのファイルがあれば読み込む。

端末固有の設定は`~/.bashrc.local`に記述できる。このファイルがあれば、`.bashrc`の最後で読み込む。`~/.bashrc.local`は端末ごとに管理する。

## Neovim

LazyVimのC/C++（clangd）、CMake、.NET、Python、LaTeXとデバッグ用の拡張を有効にしている。

インデントはスペース4個で、C/C++とPythonでは保存時の自動整形を無効にしている。プレーンテキストではスペルチェックを無効にし、C/C++ではインレイヒントを表示しない。配色はTokyo Nightで、背景を透過する設定にしている。

WSLでは、対応する端末でOSC 52を使ってコピーした内容をOSのクリップボードに送る。OSのクリップボードから貼り付ける際は、端末の貼り付け操作を使う。

Neovimの導入は[LazyVimの導入手順](https://www.lazyvim.org/installation)を参照する。LaTeXではVimTeX v2.17、texlab、Zathuraを使う。依存ツールの導入とコンパイル・PDF表示の操作は[NeovimのREADME](private_dot_config/nvim/README.md)に記載している。

## 競技プログラミング

### C++環境のセットアップ

GCC 13でgnu++23を使う。atcoder-cliは2.2.0、online-judge-toolsは11.5.1、online-judge-api-clientは10.10.1に固定し、AtCoder Library（ACL）もリビジョンを指定する。GCCのパッチバージョンや各ツールの依存パッケージのバージョンは、OSとパッケージ管理に左右される。

chezmoiで設定を適用した後に、次のコマンドを実行する。

```bash
sudo apt update
sudo apt install g++-13 clangd nodejs npm pipx git
export PATH="$HOME/.local/bin:$PATH"
cp-setup
```

`cp-setup`はツールが未導入、またはバージョンが指定と異なる場合に、npm・pipxで指定バージョンをインストールする。ACLがなければ取得して指定リビジョンに切り替え、既存のACLのリビジョンが指定と異なる場合は停止する。

AtCoderの認証は端末ごとに行う。Cookie、`session.json`、認証用スクリプトはこのリポジトリに含めない。

### 問題を解くとき

atcoder-cliは`acc`コマンドで使用する。

```bash
cd ~/atcoder
acc new abc123
cd abc123/a
accdebug       # main.cppをDEBUG付きでコンパイルしてmainを作る
./main         # 標準入力で動作確認
acctest        # 最適化付きでコンパイルしてmainを作り、testsのサンプルを実行
acccheck       # ASan・UBSan・STLの検査付きでmain.sanを作り、サンプルを実行
acc submit     # 提出
```

各ビルドコマンドでは、ソースファイルを指定できる。`acctest`と`acccheck`では、テストディレクトリも指定できる。例えば`acctest solution.cpp samples`は、`solution.cpp`をコンパイルして`samples`内のテストを実行する。省略時は`main.cpp`と`tests`を使う。

ビルドコマンドは`~/.local/bin`に配置されるBashスクリプトで、Bash以外のシェルからも呼び出せる。`acccheck`はASanを使用するため、スタック上限を8 MiBに設定する。

### 設定を変更するとき

共通設定とツールのバージョンは`private_dot_config/competitive-programming/env.sh`、解答用テンプレートは`private_dot_config/atcoder-cli-nodejs/cpp/`で編集する。以前のテンプレート用Gitリポジトリが適用先のcppディレクトリに残っている場合も、共有する変更はこのdotfilesに記録する。

コンパイラやC++の規格、ヘッダーの検索先を変更する場合は、`atcoder/dot_clangd.tmpl`も合わせて変更する。テンプレート内のホームディレクトリのパスは、chezmoiが各端末に合わせて展開する。

端末固有のビルド設定は`~/.config/competitive-programming/local.sh`で共通設定を上書きできる。このファイルはchezmoiの管理対象ではなく、`local.sh`の変更はclangdの設定には自動反映されない。

## 参考

[chezmoiの適用コマンド](https://www.chezmoi.io/reference/commands/apply/)と[atcoder-cliのテンプレート設定](https://github.com/Tatamo/atcoder-cli#provisioning-templates)を参照。
