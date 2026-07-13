[![tests](https://github.com/kurrrru/toolbox/actions/workflows/tests.yml/badge.svg)](https://github.com/kurrrru/toolbox/actions/workflows/tests.yml)
[![format](https://github.com/kurrrru/toolbox/actions/workflows/format.yml/badge.svg)](https://github.com/kurrrru/toolbox/actions/workflows/format.yml)
# toolbox

アルゴリズムとデータ構造のヘッダオンリー集。各実装は `includes/toolbox/` 以下に置き、
テストは `tests/` 以下に置く。ビルド・テストは CMake + CTest で行う。

## 必要環境

- CMake 3.20 以上
- C++20 対応コンパイラ（GCC / Clang）
- （整形チェックを行う場合）clang-format 21

## ビルドとテスト

CMake プリセットを利用する。

```bash
cmake --preset default
cmake --build --preset default -j
ctest --preset default -j
```

利用可能なプリセット:

| プリセット | 内容 |
|---|---|
| `default` | Debug ビルド + テスト |
| `release` | Release ビルド + テスト |
| `asan`    | AddressSanitizer + UndefinedBehaviorSanitizer 付き |

サニタイザ付きで実行する場合:

```bash
cmake --preset asan
cmake --build --preset asan -j
ctest --preset asan -j
```

## 新しいアルゴリズムの追加

1. 実装を `includes/toolbox/<category>/<algorithm>/<algorithm>.hpp` に置く。
2. テストを `tests/<category>/<algorithm>/main.cpp` に置く。
   - ライブラリのヘッダはすべて `#include "toolbox/..."` の形で参照する
     （include ルートは `includes/`）。
   - テストヘルパは `#include "utils/test_util.hpp"` で参照する
     （`tests/` が include パスに入っている）。
   - テストランナは `tests/utils/test_util.hpp` の `toolbox::test_utils::run_tests` を使う。
3. これだけでよい。`main.cpp` は CMake が自動検出し、`<ディレクトリ名>_test`
   という名前のテストとして登録する（`CMakeLists.txt` の編集は不要）。

> 自動検出は `file(GLOB_RECURSE ... CONFIGURE_DEPENDS)` で行っている。ファイルを
> 追加・削除した後は、再度 `cmake --build` すれば構成が更新される。

## コード整形

`.clang-format`（Google ベース）に従う。CI で整形チェックが走る。

```bash
# 差分チェック（CI と同じ）
find includes tests \( -name '*.hpp' -o -name '*.cpp' \) -print0 \
  | xargs -0 clang-format --dry-run --Werror

# 一括整形
find includes tests \( -name '*.hpp' -o -name '*.cpp' \) -print0 \
  | xargs -0 clang-format -i
```

## CI

GitHub Actions で以下を実行する（`.github/workflows/`）。

- `tests.yml` — GCC / Clang それぞれでビルド + CTest、加えて ASan + UBSan 実行。
- `format.yml` — clang-format による整形チェック。
