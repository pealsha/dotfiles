# miniPCのUbuntu VMを使う

miniPCではProxmoxを動かし、その上のUbuntu VMを普段のLinux開発環境にする。Tailscale、OpenSSH、Neovim、AtCoder用ツールはUbuntu VM内に導入する。ここでは既存のdotfilesに合わせてUbuntu 24.04を使う。Proxmoxホストにdotfilesや開発ツールを適用する必要はない。

## 各環境の役割

| 環境 | 役割 | 保存するもの |
|---|---|---|
| Ubuntu VM | 普段の編集・ビルド・テスト・AtCoderの操作 | 開発ツール、作業コード、VM用の認証情報 |
| デスクトップPC / ノートPC | ターミナル表示、SSH接続、ブラウザー | それぞれのSSH鍵と接続先設定 |
| 各PCのWSL | VM停止時、回線不調時、遅延が気になるときのローカル開発 | 同じdotfilesとツール、必要なコード、WSL用の認証情報 |
| Proxmoxホスト | VMの起動・停止・バックアップ | VM設定とバックアップ設定 |

接続元を変えても、Ubuntu VM内では同じホームディレクトリとツールを使う。WSLへ切り替えたときは別のファイルとプロセスを使う。共通設定はchezmoi、作業コードは開発対象のGitリポジトリ、障害からの復元はバックアップで管理する。

## Ubuntu VMの準備

1. ProxmoxでUbuntu 24.04のVMを作成する。ネットワークはLANにつながるブリッジ（通常は`vmbr0`）に接続し、Ubuntuからインターネットへ出られることを確認する。CPU・メモリー・ディスクはminiPCの容量と他のVMの使用量に合わせる。
2. VMのOptionsで「Start at boot」と「QEMU Guest Agent」を有効にする。Guest Agentの設定変更後は、必要に応じてVMを一度停止して起動する。
3. Ubuntuに一般ユーザーを作成し、Proxmoxのコンソールからログインする。
4. Ubuntu VM内にTailscaleを導入し、自分のtailnetへ接続する。miniPCのProxmoxホストにTailscaleが導入済みでも、Ubuntu VMは別の端末として登録する。

VM内で確認する。

```bash
cat /etc/os-release
uname -m
ip route
sudo apt update
sudo apt install openssh-server qemu-guest-agent tmux
sudo systemctl enable --now ssh
sudo systemctl start qemu-guest-agent
systemctl is-active ssh qemu-guest-agent
```

Tailscaleは[公式のLinux導入手順](https://tailscale.com/docs/install/linux)に沿ってインストールする。VM内で初回の登録とサービス確認を行う。

```bash
sudo tailscale up
sudo systemctl enable --now tailscaled
systemctl is-active tailscaled
tailscale status
tailscale ip -4
```

`tailscale up`が示すURLは接続元PCのブラウザーで開いて認証する。認証用のキーをdotfilesに記録しない。

Proxmoxホストのシェルでは、対象のVM IDを確認してから設定を調べる。以下の`VM_ID`は実際のIDに置き換える。

```bash
qm list
qm config VM_ID
qm status VM_ID
```

`onboot: 1`、Guest Agentの有効化、ネットワークブリッジを確認する。ホストの再起動後にVMとSSH・Tailscaleが自動で復帰することも、外出前に確認する。VMのスナップショットをバックアップの代わりにせず、Proxmoxのバックアップジョブと復元先を用意する。VMのバックアップには認証情報も含まれるため、保管先へのアクセスを制限する。

## Tailscale経由のSSH接続

通常はWindows TerminalのPowerShellからWindowsの`ssh`を使う。Windows側にTailscaleを導入し、Ubuntu VMと同じtailnetに接続しておく。これならWSL内のTailscale・DNS設定に接続が依存しない。

ここではOpenSSHによる公開鍵認証を使う。Tailscaleは接続経路とtailnetの通信権限を管理する。Tailscale SSHは別の認証方式であり、`sudo tailscale set --ssh`はこの手順では実行しない。既にTailscale SSHが有効な場合は、Tailscale側のポート22への接続を引き受けるため、OpenSSHの鍵設定がその接続に使われるとは限らない。現在の方式を確認してから選ぶ。

WindowsのPowerShellで確認する。

```powershell
Get-Command ssh, tailscale
tailscale status
```

各PCに専用のSSH鍵がなければ、そのPCで作成する。以下は専用ファイル名の例で、同名の鍵が既にある場合は上書きしない。パスフレーズを設定する。

```powershell
ssh-keygen -t ed25519 -f "$env:USERPROFILE\.ssh\id_ed25519_devmini"
Get-Content "$env:USERPROFILE\.ssh\id_ed25519_devmini.pub"
```

表示した**公開鍵**をUbuntu VMの一般ユーザーの`~/.ssh/authorized_keys`に追記する。デスクトップPCとノートPCの公開鍵をそれぞれ登録する。秘密鍵は接続元から移さない。VMのコンソールでディレクトリとファイルの権限を確認する。

```bash
mkdir -p ~/.ssh
chmod 700 ~/.ssh
# エディターでauthorized_keysに公開鍵を追記してから実行
chmod 600 ~/.ssh/authorized_keys
sudo sshd -t
sudo sshd -T | rg '^(pubkeyauthentication|passwordauthentication|permitrootlogin|listenaddress) '
```

`rg`が未導入なら`sshd -T`の出力を直接読む。接続先PCの`%USERPROFILE%\.ssh\config`を編集し、次のブロックを既存の`Host *`より前に追加する。`MINI_PC_TAILSCALE_NAME`はUbuntu VMのMagicDNS名またはTailscale IP、`UBUNTU_USER`はVMの一般ユーザー名に置き換える。この設定はリポジトリへ追加しない。

```sshconfig
Host dev-mini
    HostName MINI_PC_TAILSCALE_NAME
    User UBUNTU_USER
    IdentityFile ~/.ssh/id_ed25519_devmini
    IdentitiesOnly yes
    ForwardAgent no
    ForwardX11 no
    ServerAliveInterval 30
    ServerAliveCountMax 3
    ConnectTimeout 10
```

```powershell
ssh -G dev-mini
ssh dev-mini
```

初回はVMのコンソールで`ssh-keygen -lf /etc/ssh/ssh_host_ed25519_key.pub`を実行し、接続時に表示されるホスト鍵の指紋と照合する。接続後に`hostname`と`whoami`で開発先を確認する。通常の作業では以下を使う。

```powershell
ssh -t dev-mini 'tmux new-session -A -s dev'
```

Ubuntu VMのtailnetへのTCP 22通信が許可されている必要がある。ルーターのインターネット向けポート開放は不要。OpenSSHは通常LAN側でも待ち受けるため、tailnetの通信制御だけでLAN側のSSHが制限されるわけではない。接続確認後に、必要なアクセス範囲に合わせてVM側の待ち受け・ファイアウォールを設定する。Tailscaleの通信は専用のnetfilterルールも使うため、UFWの表示だけでtailnet側の制限を判断しない。パスワード認証などを変更する場合は、VMコンソールを使える状態で`sshd -t`と別セッションでの鍵接続を確認する。

WSLから`ssh dev-mini`を使いたい場合は、そのWSLの`~/.ssh/config`と鍵を別途用意し、VMまでの到達性を確認する。WindowsとWSLではホームディレクトリ、SSH設定、鍵、`known_hosts`が別になる。Tailscaleの診断コマンドは、実際にTailscaleを動かしているWindows側で実行する。

## 開発ツールの導入

Ubuntu VM内で[READMEの導入手順](../README.md#導入と更新)を実行し、C++環境を構築する。

```bash
sudo apt update
sudo apt install g++-13 clangd nodejs npm pipx git ripgrep fd-find build-essential unzip
chezmoi init https://github.com/pealsha/dotfiles.git
chezmoi diff
chezmoi apply
export PATH="$HOME/.local/bin:$PATH"
cp-setup
```

Neovim本体とLazyVimの依存ツールもVM内に必要になる。[LazyVimの要件](https://www.lazyvim.org/)を満たすNeovimを導入する。Ubuntu 24.04標準の古いNeovimだけではこの設定を使えない。既存のdotfilesがLazyVimの設定を配置するので、別途Starterを`~/.config/nvim`にcloneして上書きする必要はない。Nerd Fontは接続元のWindows Terminalに設定する。

```bash
nvim --version
g++-13 --version
clangd --version
acc --version
oj --version
tmux -V
```

Neovimを起動してプラグインの導入を待ち、`:LazyHealth`で依存ツールを確認する。C++ファイルを開き、`:LspInfo`でclangdの起動と補完を確認する。`~/atcoder/.clangd`はchezmoiがVMのホームディレクトリに合わせて生成する。GCC 13 / gnu++23やACLの固定値はminiPCへ移すために変更する必要はない。

## クリップボードの確認

まずtmuxを使わないSSH接続でNeovimを開き、次にtmux内で同じ操作を試す。

1. 日本語を含む行を入力してノーマルモードで`yy`を押す。
2. Windowsのメモ帳に貼り付けて、コピーが届いたことを確認する。
3. Neovimで`p`を押し、コピーした行が待ち時間なく貼り付けられることを確認する。
4. メモ帳で別の文字列をコピーする。Neovimを挿入モードにしてWindows Terminalの貼り付け操作（通常は`Ctrl+Shift+V`）を使う。

OSC 52はコピー先の端末の対応が必要。OSからの貼り付けは端末操作を使う。`p`はこのNeovim内のコピー内容を使い、OSのクリップボードを読み取らない。

```vim
:set clipboard?
:lua print(vim.g.clipboard and vim.g.clipboard.name)
:checkhealth vim.provider
```

WSL / SSHでは`unnamedplus`と`OSC 52 (copy only)`になる。tmux内だけでコピーできない場合は確認する。

```bash
tmux show -s set-clipboard
tmux info | rg 'Ms:'
```

`set-clipboard`は`on`、`Ms`はエスケープシーケンスが表示されることを確認する。設定適用後の既存tmuxには`tmux source-file ~/.tmux.conf`で読み込ませる。`Ms`が`[missing]`なら、tmuxの外で`echo "$TERM"`を確認し、その端末がOSC 52対応であることを確認してから`~/.tmux.conf.local`に`set -as terminal-features ',実際のTERM:clipboard'`を記載する。全端末に対応を仮定する設定は追加しない。

SSH接続前から存在するtmuxペインにはSSHの環境変数がないことがある。その場合は`NVIM_OSC52=1 nvim`で起動する。OSC 52非対応の端末では`NVIM_OSC52=0 nvim`でこの設定を無効にできる。端末ごとに固定したい場合は、管理対象外の`~/.bashrc.local`に環境変数を記述する。tmuxの`set-clipboard on`はペイン内のアプリケーションによるコピーを許可する設定。

## AtCoderの認証とビルド確認

VMへSSH接続して`acc`や`oj`を実行すると、VM内の認証状態を使う。WindowsのブラウザーでログインしただけではVMのCLIはログイン済みにならない。`acc`と`oj`も別に認証状態を持つ。

VM内で認証状態を確認する。

```bash
acc session
oj login --check https://atcoder.jp/
```

未認証の場合は、その環境で使っている認証手順をVM内で実施する。標準の入口は`acc login`と`oj login https://atcoder.jp/`だが、AtCoder側の認証・アクセス制限によってCLIログインやダウンロードが失敗することがある。GUIのないVMでブラウザー認証が必要な場合も、自動で成功するとは限らない。失敗時は認証方式を確認し、利用できるブラウザーで問題文の閲覧・コード提出を行う。CookieをREADME、コマンドの引数、Gitへ貼り付けない。

ログイン後は公開済みの練習問題で取得・ビルド・テストまで確認する。これは接続先の認証とツールの動作確認で、提出は含めない。

```bash
mkdir -p ~/atcoder
cd ~/atcoder
# 既にpracticeディレクトリがある場合は別の作業場所を使う
acc new practice
cd practice/a
# main.cppに解答を書いてから実行
accdebug
acctest
acccheck
```

CLIで取得できなくても、ブラウザーから問題文とサンプルを取得すればビルド・テストはできる。各WSLでも、代替環境として使う前に同じ確認を済ませる。VMの`session.json`や`cookie.jar`を共有する構成にはしない。

## 切断・遅延とWSLへの切り替え

tmux内の作業は、SSH切断後もVMが動いている間は続く。`Ctrl+b`、`d`で切り離し、同じ接続コマンドで再開する。別PCから同じ`dev`セッションへ接続すると同じ画面を共有する。独立した作業には別のセッション名を使う。VMの停止・再起動ではプロセスが終了するので、編集内容は保存する。

WindowsのPowerShellで、次の`MINI_PC_TAILSCALE_NAME`を接続先のUbuntu VMに置き換える。`dev-mini`はSSHだけの別名なので、Tailscaleのコマンドには実際のMagicDNS名またはIPを渡す。

```powershell
tailscale ping --c 10 --until-direct=false MINI_PC_TAILSCALE_NAME
tailscale netcheck
ssh dev-mini
```

`direct`は直接接続、`relay`は中継接続を示す。最初は中継になり、途中で直接接続へ変わる場合もある。自宅、帰省先、テザリングで確認し、Neovimの文字入力・移動・補完を実際に試す。距離だけで操作感は決まらず、回線と接続経路、VMの負荷にも左右される。keepaliveは切断の検出に使う設定で、入力遅延を短縮する設定ではない。

VMへ接続できない、または入力の待ち時間が気になる場合は、Windows TerminalでWSLのプロファイルを開き、ローカルのNeovimとビルドコマンドを使う。自動で切り替える設定は作らず、Bashの`user@host`表示と`hostname`で実行先を確認する。

外出前・コンテスト前に各WSLで行う。

```bash
chezmoi git -- pull --ff-only
chezmoi diff
chezmoi apply
cp-setup
acc session
oj login --check https://atcoder.jp/
# 手元にある問題ディレクトリでacctestを実行
```

必要な開発対象のリポジトリもWSLへ取得し、コードを最新にする。普段からVMで保存したコードをそのリポジトリへpushし、WSLではpullする。未保存のバッファーや未pushの変更は、VMに接続できないと取得できない。コンテスト中の解答は公開せず、必要なら非公開のリポジトリや手元へのコピーで保存する。復旧後はWSLの変更を保存・同期してからVMで続きを行い、同じファイルを両方で編集し続けない。

## リポジトリへ保存しない情報

接続先の名前・IP・ユーザー名、SSH鍵、`authorized_keys`、`known_hosts`、Tailscaleの認証情報、AtCoderのCookie・`session.json`は各環境だけに置く。Git用の認証もVMとWSLで別に用意する。SSHエージェント転送は基本設定で無効にしている。

`~/.bashrc.local`、`~/.tmux.conf.local`、`~/.config/competitive-programming/local.sh`も端末ごとに管理する。`.chezmoiignore`はこれらの適用先を除外し、`.gitignore`は既知の認証ファイルと端末固有設定が誤ってGitに入ることを防ぐ。任意の名前の秘密ファイルまで検出する仕組みではないため、`chezmoi add ~/.ssh`や認証ディレクトリ全体の追加は行わず、commit前に差分を確認する。

## 参考資料

- [Proxmoxのネットワーク設定](https://pve.proxmox.com/wiki/Network_Configuration)
- [ProxmoxのVM設定・起動・Guest Agent](https://pve.proxmox.com/pve-docs/qm.1.html)
- [Tailscale経由のSSH](https://tailscale.com/docs/reference/ssh-over-tailscale)
- [Tailscale SSHの認証方式](https://tailscale.com/docs/features/tailscale-ssh)
- [Tailscale CLIの診断コマンド](https://tailscale.com/docs/reference/tailscale-cli)
- [OpenSSHの接続設定](https://man.openbsd.org/ssh_config)
- [Neovimのクリップボード](https://neovim.io/doc/user/provider/#clipboard-osc52)
- [tmuxのクリップボード](https://github.com/tmux/tmux/wiki/Clipboard)
