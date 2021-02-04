# Pong サンプル
書籍「ゲームプログラミングC++」のサンプルコード

## 内容
+ 第1章 Pongゲーム `src/chapter1`
  + 簡単なPongゲームを作る
  + 練習問題 1.1
    + プレイヤーを二人に(まだ)
  + 練習問題 1.2
    + ボールを複数に(まだ)

## 開発環境の構築
1. MinGW-w64のインストール

    [インストーラのURL](http://mingw-w64.org/doku.php/download)

2. 「Architecture」を「i686」から「x86_64」に切り替える．インストールするディレクトリパスから`Program Files`を削除してC:直下にインストールさせる
3. 環境変数Pathにmingw-w64内の`bin`ディレクトリへのパスを通す

4. SDL2のダウンロード
[SDL2のダウンロードページ](https://www.libsdl.org/download-2.0.php)から，
+ 「Runtime Binaries」から`SDL2-2.0.14-win32-x64.zip (64-bit Windows)`
+ 「Development Libraries」から`SDL2-devel-2.0.14-mingw.tar.gz (MinGW 32/64-bit)`
 
  をダウンロード

5. 前者を普通に解凍，後者は`tar`コマンドで解凍
```bash
tar -xvfz SDL2-devel-2.0.14-mingw.tar.gz
```
6. 前者を解凍すると出てくる`SDL2.dll`を実行ファイルと同じ階層(つまり`./build`に配置)

7. 後者を解凍して出てくる`x86_64-w64-mingw32`フォルダの中身をmingw-w64内の同名フォルダの中に配置

## 設定
以下の設定ファイルでは適宜`<~~>`の部分を環境・状況に合わせて置換．

### VSCodeの設定
VSCodeの設定は，`.vscode/c_cpp_properties.json`に記述．
```json:.vscode/c_cpp_properties.json
{
  "configurations": [
    {
      "name": "Win32",
      "includePath": [
        "${workspaceFolder}/**",
        "<MinGW-w64内にあるincludeフォルダへのパス>"
      ],
      "defines": [
        "_DEBUG",
        "UNICODE",
        "_UNICODE"
      ],
      "compilerPath": "<MinGW-w64内のgcc.exeへのパス>",
      "cStandard": "c11",
      "cppStandard": "c++17",
      "intelliSenseMode": "gcc-x64"
    }
  ],
  "version": 4
}
```

### ビルドタスクの設定
ビルドタスクの設定は，`.vscode/tasks.json`に記述．
```json:.vscode/tasks.json
{
  "version": "2.0.0",
  "tasks": [
    {
      "type": "shell",
      "label": "<各章ごとのラベル名>",
      "command": "<MinGW内のg++.exeへのパス>",
      "args": [
        "-g",
        "src\\chapter<章番号>\\*.cpp",
        "-o",
        "build\\<生成する実行ファイル名>.exe",
        "-I<MinGW-w64内にあるincludeフォルダへのパス>",
        "-L<MinGW-w64内にあるlibフォルダへのパス>",
        "-lmingw32",
        "-lSDL2main",
        "-lSDL2",
        "-mwindows"
      ],
      "options": {
        "cwd": "${workspaceFolder}"
      },
      "problemMatcher": [
        "$gcc"
      ],
      "group": "build"
    }
  ]
}
```

規定のビルドタスクにしたければ，`"group"`プロパティを以下のように設定．
```json
      "group": {
        "kind": "build",
        "isDefault": true
      }
```
`"kind"`はビルド`"build"`，テスト`"test"`の2種類．

`"isDefault"`は`true`のみ．

ビルドするときは`Ctrl+Shift+B`

### デバッグの設定
デバッグの設定は，`.vscode/launch.json`に記述．
```json:.vscode/launch.json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "(gdb)",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}\\build\\<tasks.jsonで指定した実行ファイル名>.exe",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false,
      "MIMode": "gdb",
      "miDebuggerPath": "MinGW-w64フォルダ内のgdb.exeへのパス",
      "setupCommands": [
        {
          "description": "Enably pretty printing",
          "text": "-enable-pretty-printing",
          "ignoreFailures": true
        }
      ],
      "preLaunchTask": "<tasks.jsonで指定したラベル名>"
    }
  ]
}
```

デバッグするときは`F5`
