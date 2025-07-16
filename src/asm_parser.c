#include "assembler.h"
#include "asm_parser.h"
#include "asm_symbol.h"

// 入力を解析して各行をフィールドに分解
void parser(FILE *fp, assembler **head)
{
    assembler *current = NULL;
    char buf[256];
    int cnt = 0;
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        // 改行・コメント行のスキップ
        char *p = buf;
        while (*p == ' ' || *p == '\t')
            p++;
        if (*p == '\0' || *p == '\n' || *p == '/' || *p == '\r')
            continue;
        // 行内コメント削除
        char *comment = strstr(p, "//");
        if (comment)
            *comment = '\0';
        // 改行削除
        char *nl = strchr(p, '\n');
        if (nl)
            *nl = '\0';

        // 不要な空白・タブを除去
        char cleaned[256];
        int idx = 0;
        for (char *q = p; *q != '\0'; q++) {
            if (*q != ' ' && *q != '\t' && *q != '\r') {
                cleaned[idx++] = *q;
            }
        }
        cleaned[idx] = '\0';

        // ラベル判定: (XXX)形式
        if (p[0] == '(' && p[strlen(p)-1] == ')') {
            // ラベル名抽出
            char label[128];
            size_t len = strlen(p);
            if (len > 2) {
                strncpy(label, p+1, len-2);
                label[len-2] = '\0';
                // ラベルシンボル登録
                addEntryTable_label(label, cnt);
            }
            continue; // ラベル行は命令リストに追加しない
        }

        // ノード作成
        assembler *code = malloc(sizeof(assembler));
        code->program = strdup(cleaned);
        code->next = NULL;
        cnt++;
        if (*head == NULL)
        {
            *head = code;
            current = code;
        }
        else
        {
            current->next = code;
            current = code;
        }
    }
    if (current != NULL)
        current->next = NULL;
}