#include <stdio.h>
#include <string.h>

size_t MAX_BUF = 2047;


const char *trimSpaceL(const char *strval, char *buf, size_t bufsz);

int main(int argc, char *argv[])
{
    const char *buf1 = "    string with leading spaces";
    const char *buf2 = "string with trailing spaces    \t  ";
    const char *buf3 = "    string with surrounding spaces\t ";
    const char *buf4 = "string with no surrounding spaces";
    const char *buf5 = "";
    const char *buf6 = "   \t      \t";

    int pos = 0;
    size_t len = 0;

    char buf[MAX_BUF+1];

    printf("Test app to simplify string trim methods\n\n");

    if (trimSpaceL(NULL, buf, MAX_BUF))
    {
        printf("\x1B[31mBad test, got value, expected NULL for NULL input.\x1b[0m\n");
    }
    printf("--------------\n");
    if (trimSpaceL(buf1, NULL, MAX_BUF))
    {
        printf("\x1B[31mBad test, got value, expected NULL for NULL output buffer.\x1b[0m\n");
    }
    printf("--------------\n");
    if (trimSpaceL(buf1, buf, 1))
    {
        printf("\x1B[31mBad test, got value, expected NULL for too small output buffer.\x1b[0m\n");
    }
    printf("--------------\n");

    // Actual values
    if (!trimSpaceL(buf1, buf, MAX_BUF))
    {
        printf("\x1B[31mBad test, expected value, got NULL.\x1b[0m\n");
    }
    else
    {
        printf("\x1B[32mPASS:\x1B[0m got result [%s] from [%s]\n", buf, buf1);
        memset(buf, 0, sizeof(char) * MAX_BUF);
    }
    printf("--------------\n");

    if (!trimSpaceL(buf2, buf, MAX_BUF))
    {
        printf("\x1B[31mBad test, expected value, got NULL.\x1b[0m\n");
    }
    else
    {
        printf("\x1B[32mPASS:\x1B[0m got result [%s] from [%s]\n", buf, buf2);
        memset(buf, 0, sizeof(char) * MAX_BUF);
    }
    printf("--------------\n");

    if (!trimSpaceL(buf3, buf, MAX_BUF))
    {
        printf("\x1B[31mBad test, expected value, got NULL.\x1b[0m\n");
    }
    else
    {
        printf("\x1B[32mPASS:\x1B[0m got result [%s] from [%s]\n", buf, buf3);
        memset(buf, 0, sizeof(char) * MAX_BUF);
    }
    printf("--------------\n");

    if (!trimSpaceL(buf4, buf, MAX_BUF))
    {
        printf("\x1B[31mBad test, expected value, got NULL.\x1b[0m\n");
    }
    else
    {
        printf("\x1B[32mPASS:\x1B[0m got result [%s] from [%s]\n", buf, buf4);
        memset(buf, 0, sizeof(char) * MAX_BUF);
    }
    printf("--------------\n");

    if (!trimSpaceL(buf5, buf, MAX_BUF))
    {
        printf("\x1B[31mBad test, expected value, got NULL.\x1b[0m\n");
    }
    else
    {
        printf("\x1B[32mPASS:\x1B[0m got result [%s] from [%s]\n", buf, buf5);
        memset(buf, 0, sizeof(char) * MAX_BUF);
    }
    printf("--------------\n");

    if (!trimSpaceL(buf6, buf, MAX_BUF))
    {
        printf("\x1B[31mBad test, expected value, got NULL.\x1b[0m\n");
    }
    else
    {
        printf("\x1B[32mPASS:\x1B[0m got result [%s] from [%s]\n", buf, buf6);
        memset(buf, 0, sizeof(char) * MAX_BUF);
    }
    printf("--------------\n");

    return 0;
}

const char *trimSpaceL(const char *strval, char *buf, size_t bufsz)
{
    int pos = 0;
    size_t inLen = 0;
    if (!strval)
    {
        printf("\x1B[31mBad call, NULL input string.\x1B[0m\n");
        return NULL;
    }
    else if (!buf)
    {
        printf("\x1B[31mBad call, NULL output buffer.\x1B[0m\n");
        return NULL;
    }
    else if ((inLen = strlen(strval)) > bufsz)
    {
        printf("\x1B[31mBad call, Output buffer is smaller then the input string.\x1B[0m\n");
        return NULL;
    }
    else
    {
        memset(buf, 0, sizeof(char) * bufsz);
        pos = strcspn(strval, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXZY0123456789.,/<>?;:'\"[]{}=+-_()*&^%$#@!~`");

        if (pos >= 0)
        {
            printf("\x1B[33mDBG:\x1B[0m Got pos [%d], char at pos [%c]\n", pos, strval[pos]);
            strcpy(buf, strval+pos);
        }

        return buf;
    }
}

