# dotfiles

Ubuntu 24.04 / WSLで日常的に使うシェル、エディター、開発用コマンドの設定をchezmoiで管理する。普段はminiPCのProxmox上に作成するUbuntu VMを開発環境として使い、デスクトップPCとノートPCからTailscale経由でSSH接続する。各PCのWSLは、VMに接続できない場合や遠隔接続の遅延が気になる場合の代替環境として維持する。

共通の設定はこのリポジトリからUbuntu VMと各WSLに適用する。接続先、SSH鍵、AtCoderの認証情報は各環境で管理する。VMの作成、SSH接続、遅延の確認、WSLへの切り替えは[miniPCの導入・運用手順](docs/miniPC.md)を参照する。

## 管理する設定

| 設定 | 内容と適用先 |
|---|---|
| [Bash](dot_bashrc) | プロンプト、履歴、補完、PATHなどを `~/.bashrc` に設定する |
| [Neovim](private_dot_config/nvim/) | LazyVimを基にしたエディター設定を `~/.config/nvim/` に配置する |
| [AtCoder用の設定](private_dot_config/atcoder-cli-nodejs/) | atcoder-cliの設定とC++の解答用テンプレートを `~/.config/atcoder-cli-nodejs/` に配置する |
| [開発用コマンド](dot_local/bin/) | C++環境のセットアップ・ビルド・テスト用スクリプトを `~/.local/bin/` に配置する |
| [C++のビルド設定](private_dot_config/competitive-programming/env.sh) | コンパイラ、規格、ツールのバージョンを `~/.config/competitive-programming/env.sh` に設定する |
| [clangd](atcoder/dot_clangd.tmpl) | AtCoder用のコンパイラ、規格、ヘッダーの検索先を `~/atcoder/.clangd` に設定する |
| [tmux](dot_tmux.conf) | SSH切断後も作業を続けるための設定とOSC 52連携を `~/.tmux.conf` に配置する |

## 導入と更新

### インストール

Ubuntu VMと各PCのWSLに[chezmoiをインストール](https://www.chezmoi.io/install/)してから、それぞれで次のコマンドを実行する。既存環境では下の更新手順を使う。`chezmoi init`でこのリポジトリを取得し、`chezmoi diff`で端末の設定との差分を確認する。`chezmoi apply`で設定ファイルとスクリプトを端末に配置する。

```bash
chezmoi init https://github.com/pealsha/dotfiles.git
chezmoi diff
chezmoi apply
```

Neovimの導入とC++環境のセットアップは、それぞれの節を参照する。

### ローカルdotfilesの更新

共通設定を変更するときは、chezmoiのソースディレクトリにあるファイルを編集する。`chezmoi diff`で確認してから`chezmoi apply`で使用中の端末に適用し、GitHubへpushして別端末でも使えるようにする。

既に導入している別端末では、次のコマンドでソースをGitHubの内容に更新し、差分を確認してから適用する。

```bash
chezmoi git -- pull --ff-only
chezmoi diff
chezmoi apply
```

C++環境を利用する端末では、適用後に`cp-setup`を実行する。

chezmoiは設定の配布と環境の再構築に使う。SSHでUbuntu VMを使う間は、接続元のWSLへ変更を反映する必要はない。WSLを代替環境として使えるよう、帰省前やコンテスト前には設定・ツールを更新し、ローカルでビルドできることを確認する。作業中のコードは別のGitリポジトリなどで保存する。chezmoiはコード、認証情報、VMのバックアップを同期しない。

## Bash

`~/.local/bin`と`~/.local/share/bob/nvim-bin`をPATHに追加する。Rustの環境設定、Codexの環境設定、nvmの補完は、それぞれのファイルがあれば読み込む。

端末固有の設定は`~/.bashrc.local`に記述できる。端末ごとに管理し、ファイルがあれば`.bashrc`の最後で読み込む。

## Neovim

LazyVimのC/C++（clangd）、CMake、.NET、Python、LaTeXとデバッグ用の拡張を有効にしている。

インデントはスペース4個で、C/C++とPythonでは保存時の自動整形を無効にしている。プレーンテキストではスペルチェックを無効にし、C/C++ではインレイヒントを表示しない。

配色はTokyo Nightで、背景を透過する設定にしている。

WSL上、またはSSH接続先のNeovimでコピーすると、OSC 52に対応した接続元の端末を通じてクリップボードに送る。通常の`p`は、そのNeovimで最後にコピー・削除した内容を貼り付ける。Windowsなど接続元のクリップボードから貼り付ける場合は、挿入モードで端末の貼り付け操作を使う。OSC 52によるクリップボードの読み取りは行わない。

tmuxを使う場合も、接続元の端末がOSC 52に対応している必要がある。動作確認と`NVIM_OSC52`による端末ごとの切り替えは[miniPCの導入・運用手順](docs/miniPC.md#クリップボードの確認)に記載している。

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

`cp-setup`は、ツールが未導入か指定バージョンと異なる場合にインストールする。atcoder-cliにはnpmを使い、online-judge-toolsとonline-judge-api-clientはpipxの仮想環境に導入する。既存の`~/.local/bin/oj`もpipxの仮想環境に属していることを前提とする。

ACLは`~/lib/ac-library-master`に取得し、指定リビジョンに切り替える。既にその場所にACLがあり、リビジョンが指定と異なる場合は、既存の内容を変更せずに停止する。

AtCoderの認証はコマンドを実行する環境ごとに行う。SSHでUbuntu VMに接続している間は、接続元のPCによらずVM内の認証を使う。WSLで直接実行するときは、そのWSL内の認証が必要になる。Cookie、`session.json`、認証用スクリプトはこのリポジトリに含めない。認証状態の確認は[miniPCの導入・運用手順](docs/miniPC.md#atcoderの認証とビルド確認)を参照する。

### 問題を解くとき

セットアップとAtCoderの認証を済ませてから、`acc`（atcoder-cli）で問題を取得する。次の例では、`abc123`のa問題のディレクトリでビルド・テスト・提出を行う。

```bash
cd ~/atcoder
acc new abc123
cd abc123/a
accdebug       # main.cppをDEBUG付きでコンパイルしてmainを作る
./main         # 入力例を渡して動作を確認
acctest        # 最適化付きでコンパイルしてmainを作り、testsのサンプルを実行
acccheck       # ASan・UBSan・STLの検査付きでmain.sanを作り、testsのサンプルを実行
acc submit     # main.cppを提出
```

各ビルドコマンドでは、ソースファイルを指定できる。`acctest`と`acccheck`では、テストディレクトリも指定できる。例えば`acctest solution.cpp samples`は、`solution.cpp`をコンパイルして`samples`内のテストを実行する。省略時は`main.cpp`と`tests`を使う。

ビルドコマンドは`~/.local/bin`に配置されるBashスクリプトで、Bash以外のシェルからも呼び出せる。`acccheck`では、スタック上限が無制限の場合にASanのメモリー確保が失敗することがあるため、上限を8 MiBに設定する。

### 設定の更新

chezmoiのソースディレクトリで、共通設定とツールのバージョンは`private_dot_config/competitive-programming/env.sh`、解答用テンプレートは`private_dot_config/atcoder-cli-nodejs/cpp/`を編集する。以前のテンプレート用Gitリポジトリが`~/.config/atcoder-cli-nodejs/cpp/`に残っている場合も、共有する変更はこのdotfilesに記録する。

コンパイラやC++の規格、ヘッダーの検索先を変更する場合は、`atcoder/dot_clangd.tmpl`も合わせて変更する。テンプレート内のホームディレクトリのパスは、chezmoiが各端末に合わせて展開する。

端末固有のビルド設定は`~/.config/competitive-programming/local.sh`で共通設定を上書きできる。`env.sh`の最後で、ファイルがあれば読み込む。`local.sh`はchezmoiの管理対象に含めず、端末ごとに管理する。変更は`~/atcoder/.clangd`には自動反映されない。

## 参考

[chezmoiの適用コマンド](https://www.chezmoi.io/reference/commands/apply/)と[atcoder-cliのテンプレート設定](https://github.com/Tatamo/atcoder-cli#provisioning-templates)を参照。
