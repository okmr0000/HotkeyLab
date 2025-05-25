# RGBDelay

After Effects用のRGBチャンネル別ディレイエフェクトプラグイン

## 概要

RGBDelayは、After EffectsでRGBチャンネルごとに異なるディレイ時間を設定できるエフェクトプラグインです。
各チャンネル（Red、Green、Blue）に対して0-30フレームのディレイを設定できます。

## 対応環境

- After Effects 2024以降
- macOS 11.0以降
- Windows 10以降

## インストール方法

### macOS
1. `RGBDelay.aex`を以下のディレクトリにコピーします：
   - `/Library/Application Support/Adobe/Common/Plug-ins/7.0/MediaCore/`

### Windows
1. `RGBDelay.aex`を以下のディレクトリにコピーします：
   - `C:\Program Files\Adobe\Common\Plug-ins\7.0\MediaCore\`

## 使用方法

1. After Effectsでエフェクトを適用したいレイヤーを選択
2. エフェクトメニューから「Hotkey lab.」→「RGBDelay」を選択
3. 各チャンネルのディレイ時間を調整（0-30フレーム）

## ビルド方法

### 必要条件
- Adobe After Effects SDK
- Visual Studio 2022（Windows）
- Xcode 14以降（macOS）

### ビルド手順
1. リポジトリをクローン
2. 各プラットフォームのビルドディレクトリでビルドを実行

## ライセンス

Copyright (C) 2025 Tsuyoshi Okumura/Hotkey ltd.
All Rights Reserved.

## サポート

問題や質問がある場合は、GitHubのIssuesにてご報告ください。 