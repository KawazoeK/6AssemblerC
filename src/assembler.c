#include "assembler.h"
#include "asm_parser.h"
#include "asm_code.h"
#include "asm_symbol.h"

void freeAssemblerList(assembler *head);
void hack(FILE*, assembler **);
void outputBinary(assembler *, const char *inputFile);
char *getFileBaseName(const char *path);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s inputfile\n", argv[0]);
        return 1;
    }
    FILE *fp = fopen(argv[1], "r");
    if (!fp)
    {
        perror("ファイルを開けません");
        return 1;
    }
    assembler *head = NULL;
    hack(fp, &head);
    outputBinary(head, argv[1]);
    if (head == NULL)
        printf("No instructions found.\n");
    freeAssemblerList(head);
    freeSymbolTable();
    return 0;
}

// 全体の変換プロセス
void hack(FILE *fp, assembler **head)
{
    parser(fp, head);
    fclose(fp);

    assembler *data = *head;
    while (data != NULL) {
        code(data);
        data = data->next;
    }
}

void outputBinary(assembler *head, const char *inputFile)
{
    char *basename = getFileBaseName(inputFile);
    char outfilename[256];
    snprintf(outfilename, sizeof(outfilename), "%s.hack", basename);
    free(basename);
    FILE *fp = fopen(outfilename, "w");
    if (!fp)
    {
        perror("出力ファイルを開けません");
        return;
    }
    assembler *data = head;
    while (data != NULL) // 1行ずつ処理する
    {
        for (int i = 15; i >= 0; i--)
        {
            printf("%d", data->binary[i]);
            fprintf(fp, "%d", data->binary[i]);
        }
        printf("\n");
        if(data->next != NULL)
            fprintf(fp, "\n");
        data = data->next;
    }
    fclose(fp);
}

// assemblerリストの解放
void freeAssemblerList(assembler *head) {
    while (head) {
        assembler *next = head->next;
        free(head->program);
        free(head);
        head = next;
    }
}

// ファイルパスから拡張子・ディレクトリなしのベース名を取得
char *getFileBaseName(const char *path)
{
    const char *slash = strrchr(path, '/');
    const char *bslash = strrchr(path, '\\');
    const char *base = path;
    if (slash && bslash)
        base = (slash > bslash) ? slash + 1 : bslash + 1;
    else if (slash)
        base = slash + 1;
    else if (bslash)
        base = bslash + 1;
    // 拡張子を除去
    char *dot = strrchr(base, '.');
    size_t len = dot ? (size_t)(dot - base) : strlen(base);
    char *result = (char *)malloc(len + 1);
    strncpy(result, base, len);
    result[len] = '\0';
    return result;
}
