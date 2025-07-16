# 実行方法

## 1. コンパイル

```sh
gcc src/assembler.c src/asm_parser.c src/asm_code.c src/asm_symbol.c -o assembler
```

## 2. 実行

```sh
./assembler [FILENAME]
```

- `[FILENAME]` にはアセンブルしたいHackアセンブリファイル（例: `Add.asm` など）を指定してください。

---

## 例

```sh
./assembler Add.asm
```
