#include "assembler.h"
#include "asm_symbol.h"

typedef struct symbol
{
    char *name;
    int value;
    struct symbol *next;
} symbol;

symbol *searchTable(const char *name);
int containsTable(const char *name);

symbol definedSymbol[] = {
    {"R0", 0, NULL},
    {"R1", 1, NULL},
    {"R2", 2, NULL},
    {"R3", 3, NULL},
    {"R4", 4, NULL},
    {"R5", 5, NULL},
    {"R6", 6, NULL},
    {"R7", 7, NULL},
    {"R8", 8, NULL},
    {"R9", 9, NULL},
    {"R10", 10, NULL},
    {"R11", 11, NULL},
    {"R12", 12, NULL},
    {"R13", 13, NULL},
    {"R14", 14, NULL},
    {"R15", 15, NULL},
    // プリセットシンボル
    {"SP", 0, NULL},
    {"LCL", 1, NULL},
    {"ARG", 2, NULL},
    {"THIS", 3, NULL},
    {"THAT", 4, NULL},
    // メモリマップドI/O
    {"SCREEN", 16384, NULL},
    {"KBD", 24576, NULL},
    // 終端
    {NULL, 0, NULL}};

// シンボルテーブルの先頭ポインタ
symbol *symbolTable = NULL;

// ラベルシンボルの追加
void addEntryTable_label(const char *label, int row)
{
    if (!containsTable(label))
    {
        symbol *newsym = malloc(sizeof(symbol));
        newsym->name = strdup(label);
        newsym->value = row;
        newsym->next = symbolTable;
        symbolTable = newsym;
    }
}

// 変数シンボルの管理
int addEntryTable_variable(char *valstr)
{
    symbol *s = searchTable(valstr);
    if (!s)
    {
        // 新規シンボル作成（変数領域: 16番地以降を割り当て）
        static int varAddr = 16;
        // 定義済みシンボルの値と重複しないアドレスを探す
        int isDefined;
        do {
            isDefined = 0;
            for (int i = 0; definedSymbol[i].name != NULL; i++) {
                if (definedSymbol[i].value == varAddr) {
                    isDefined = 1;
                    varAddr++;
                    break;
                }
            }
        } while (isDefined);

        s = malloc(sizeof(symbol));
        s->name = strdup(valstr);
        s->value = varAddr++;
        s->next = symbolTable;
        symbolTable = s;
    }
    return s->value;
}

// シンボルテーブルから名前を検索して返す
symbol *searchTable(const char *name)
{
    // 動的シンボルテーブル
    for (symbol *s = symbolTable; s != NULL; s = s->next)
    {
        if (strcmp(s->name, name) == 0)
            return s;
    }
    // 定義済みシンボル
    for (int i = 0; definedSymbol[i].name != NULL; i++)
    {
        if (strcmp(definedSymbol[i].name, name) == 0)
            return &definedSymbol[i];
    }
    return NULL;
}

// シンボルテーブルに名前が存在するか判定
int containsTable(const char *name)
{
    return searchTable(name) != NULL;
}

// symbolTableの解放（動的シンボルのみ）
void freeSymbolTable()
{
    while (symbolTable)
    {
        symbol *next = symbolTable->next;
        // definedSymbol配列の要素は解放しない
        int isDefined = 0;
        for (int i = 0; definedSymbol[i].name != NULL; i++)
        {
            if (symbolTable == &definedSymbol[i])
            {
                isDefined = 1;
                break;
            }
        }
        if (!isDefined)
        {
            free(symbolTable->name);
            free(symbolTable);
        }
        symbolTable = next;
    }
}