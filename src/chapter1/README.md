# Chapter 1 pongゲーム
## 概要
簡単なPongゲーム．

四角い領域でボールが跳ね回る．

それを上下に動く棒(パドル)を動かして跳ね返す．

ボールを跳ね返せず画面外へ行くと終了．

## 操作方法
+ [W]：バーを上へ
+ [S]：バーを下へ
+ [Esc]：ゲームを終了
+ [I]：バーを上へ(2P)
+ [K]：バーを下へ(2P)

## 遊び方
```bash
pong.exe
```

2Pプレイ時
```bash
pong.exe p
```

## 実装・仕様について
`namespase mygame`で管理されています．

### クラス一覧
+ `Main`
  + ゲームを起動するエントリポイント
+ `Game`
  + ゲーム全体のループ制御
    + ユーザの入力を受け付けてパドルを操作
    + ボールやパドルの座標更新，勝敗判定
    + ボールやパドル，壁の描画
+ `Paddle`
  + プレイヤーが操作してボールを跳ね返す
    + 1PはWで上，Sで下．2PはIで上，Kで下
+ `Ball`
  + パドルや壁に衝突して跳ね返る
    + スタート時の速度はランダム．真ん中当たりに生成
+ その他(`util`)
