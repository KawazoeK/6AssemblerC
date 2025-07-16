#include "asm_code.h"
#include "asm_symbol.h"

void insertComp(int *binary, int *comp);

// フィールドをバイナリコードに変換
void code(assembler *data)
{
    if (data->program[0] == '@') // ＠XXX形式の場合
    {
        data->binary[15] = 0;
        char *valstr = data->program + 1;
        int isnum = 1;
        for (char *p = valstr; *p; p++) {
            if (!isdigit((unsigned char)*p)) {
                isnum = 0;
                break;
            }
        }
        int value = 0;
        if (isnum) {
            value = atoi(valstr);
        } else {
            // シンボルの場合
            value = addEntryTable_variable(valstr);
        }
        // 2進数変換
        for (int i = 0; i < 15; i++) {
            data->binary[i] = value % 2;
            value /= 2;
        }
    }
    else
    {
        data->binary[15] = 1;
        data->binary[14] = 1;
        data->binary[13] = 1;
        // 命令に変換
        // data->programをdest=comp;jumpで文字列を分解する
        // 正しいdest=comp;jump形式で分割
        char *dest = NULL;
        char *comp = NULL;
        char *jump = NULL;
        char *eq = strchr(data->program, '=');
        char buf[64];
        strcpy(buf, data->program);
        if (eq)
        {
            *eq = '\0';
            dest = data->program;
            char *tmp = eq + 1;
            char *sc = strchr(tmp, ';');
            if (sc)
            {
                *sc = '\0';
                comp = tmp;
                jump = sc + 1;
            }
            else
            {
                comp = tmp;
            }
        }
        else
        {
            char *tmp = buf;
            char *sc = strchr(tmp, ';');
            if (sc)
            {
                *sc = '\0';
                comp = tmp;
                jump = sc + 1;
            }
            else
            {
                comp = tmp;
            }
        }
        // dest/comp/jumpが空文字列ならNULLに
        if (dest && strlen(dest) == 0)
            dest = NULL;
        if (comp && strlen(comp) == 0)
            comp = NULL;
        if (jump && strlen(jump) == 0)
            jump = NULL;
        // デバッグ用: 分割結果を安全に出力
        if (dest)
            printf("dest: %s\n", dest);
        else
            printf("dest: (null)\n");
        if (comp)
            printf("comp: %s\n", comp);
        else
            printf("comp: (null)\n");
        if (jump)
            printf("jump: %s\n", jump);
        else
            printf("jump: (null)\n");
        // compに'M'が含まれる場合
        if (strchr(comp, 'M'))
        {
            data->binary[12] = 1;
        }
        else
        {
            data->binary[12] = 0;
        }
        // compの値ごとに判定
        if (strcmp(comp, "0") == 0)
        {
            int cs[] = {1, 0, 1, 0, 1, 0};
            insertComp(data->binary, cs);
        }
        else if (strcmp(comp, "1") == 0)
        {
            int cs[] = {1, 1, 1, 1, 1, 1};
            insertComp(data->binary, cs);
        }
        else if (strcmp(comp, "-1") == 0)
        {
            int cs[] = {1, 1, 1, 0, 1, 0};
            insertComp(data->binary, cs);
        }
        else if (strcmp(comp, "D") == 0)
        {
            int cs[] = {0, 0, 1, 1, 0, 0};
            insertComp(data->binary, cs);
        }
        else if (strcmp(comp, "A") == 0 || strcmp(comp, "M") == 0)
        {
            int cs[] = {1, 1, 0, 0, 0, 0};
            insertComp(data->binary, cs);
        }
        else if (strcmp(comp, "!D") == 0)
        {
            int cs[] = {0, 0, 1, 1, 0, 1};
            insertComp(data->binary, cs);
        }
        else if (strcmp(comp, "!A") == 0 || strcmp(comp, "!M") == 0)
        {
            int cs[] = {1, 1, 0, 0, 0, 1};
            insertComp(data->binary, cs);
        }
        else if (strcmp(comp, "-D") == 0)
        {
            int cs[] = {0, 0, 1, 1, 1, 1};
            insertComp(data->binary, cs);
        }
        else if (strcmp(comp, "-A") == 0 || strcmp(comp, "-M") == 0)
        {
            int cs[] = {1, 1, 0, 0, 1, 1};
            insertComp(data->binary, cs);
        }
        else if (strcmp(comp, "D+1") == 0)
        {
            int cs[] = {0, 1, 1, 1, 1, 1};
            insertComp(data->binary, cs);
        }
        else if (strcmp(comp, "A+1") == 0 || strcmp(comp, "M+1") == 0)
        {
            int cs[] = {1, 1, 0, 1, 1, 1};
            insertComp(data->binary, cs);
        }
        else if (strcmp(comp, "D-1") == 0)
        {
            int cs[] = {0, 0, 1, 1, 1, 0};
            insertComp(data->binary, cs);
        }
        else if (strcmp(comp, "A-1") == 0 || strcmp(comp, "M-1") == 0)
        {
            int cs[] = {1, 1, 0, 0, 1, 0};
            insertComp(data->binary, cs);
        }
        else if (strcmp(comp, "D+A") == 0 || strcmp(comp, "D+M") == 0)
        {
            int cs[] = {0, 0, 0, 0, 1, 0};
            insertComp(data->binary, cs);
        }
        else if (strcmp(comp, "D-A") == 0 || strcmp(comp, "D-M") == 0)
        {
            int cs[] = {0, 1, 0, 0, 1, 1};
            insertComp(data->binary, cs);
        }
        else if (strcmp(comp, "A-D") == 0 || strcmp(comp, "M-D") == 0)
        {
            int cs[] = {0, 0, 0, 1, 1, 1};
            insertComp(data->binary, cs);
        }
        else if (strcmp(comp, "D&A") == 0 || strcmp(comp, "D&M") == 0)
        {
            int cs[] = {0, 0, 0, 0, 0, 0};
            insertComp(data->binary, cs);
        }
        else if (strcmp(comp, "D|A") == 0 || strcmp(comp, "D|M") == 0)
        {
            int cs[] = {0, 1, 0, 1, 0, 1};
            insertComp(data->binary, cs);
        }

        data->binary[5] = 0;
        data->binary[4] = 0;
        data->binary[3] = 0;
        if (dest)
        {
            if (strchr(dest, 'A'))
            {
                data->binary[5] = 1;
            }
            if (strchr(dest, 'D'))
            {
                data->binary[4] = 1;
            }
            if (strchr(dest, 'M'))
            {
                data->binary[3] = 1;
            }
        }

        data->binary[2] = 0;
        data->binary[1] = 0;
        data->binary[0] = 0;
        if (jump)
        {
            if (strcmp(jump, "JGT") == 0)
            {
                data->binary[0] = 1;
            }
            else if (strcmp(jump, "JEQ") == 0)
            {
                data->binary[1] = 1;
            }
            else if (strcmp(jump, "JGE") == 0)
            {
                data->binary[1] = 1;
                data->binary[0] = 1;
            }
            else if (strcmp(jump, "JLT") == 0)
            {
                data->binary[2] = 1;
            }
            else if (strcmp(jump, "JNE") == 0)
            {
                data->binary[2] = 1;
                data->binary[0] = 1;
            }
            else if (strcmp(jump, "JLE") == 0)
            {
                data->binary[2] = 1;
                data->binary[1] = 1;
            }
            else if (strcmp(jump, "JMP") == 0)
            {
                data->binary[2] = 1;
                data->binary[1] = 1;
                data->binary[0] = 1;
            }
        }
    }
}


void insertComp(int *binary, int *comp)
{
    int j = 11;
    for (int i = 0; i < 6; i++)
    {
        binary[j] = comp[i];
        j--;
    }
}
