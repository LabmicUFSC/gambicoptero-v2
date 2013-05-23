#include <sip_defs.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

__BEGIN_SYS

char *create_string(const char *src)
{
    int size = strlen(src);
    if (size == 0)
        return 0;

    char *str = new char[size + 1];
    strcpy(str, src);
    return str;
}

bool start_with(const char *text1, const char *text2)
{
    int size1 = strlen(text1);
    int size2 = strlen(text2);

    if (size1 < size2)
        return false;

    for (int i = 0; i < size2; i++)
    {
        if (text1[i] != text2[i])
            return false;
    }

    return true;
}

bool contains(const char *text1, const char *text2)
{
    int size1 = strlen(text1);
    int size2 = strlen(text2);

    for (int i = 0; i < size1; i++)
    {
        if ((size1 - i) < size2)
            return false;

        for (int j = 0; j < size2; j++)
        {
            if (text1[i + j] != text2[j])
                break;

            if (j == (size2 - 1))
                return true;
        }
    }

    return true;
}

void remove_lws(char *text)
{
    int size = strlen(text);

    int sp = 0;
    for (int i = 0; i < size; i++)
    {
        if ((text[i] == ' ') || (text[i] == '\t'))
            sp++;
        else
            break;
    }
    if (sp > 0)
        strcpy(text, &text[sp]);

    for (int i = 0; i < (size - 3); i++)
    {
        if ((text[i] == '\r') && (text[i + 1] == '\n') &&
                ((text[i + 2] == ' ') || (text[i + 2] == '\t')))
        {
            int j = i - 1;
            while (j >= 0)
            {
                if ((text[j] != ' ') && (text[j] != '\t'))
                    break;
                j--;
            }

            int k = i + 3;
            while (k < size)
            {
                if ((text[k] != ' ') && (text[k] != '\t'))
                    break;
                k++;
            }

            if ((size - k) == 0)
                text[j + 2] = 0;
            else
            {
                text[j + 1] = ' ';
                strcpy(&text[j + 2], &text[k]);
            }
            i = j + 1;
            size = strlen(text);
        }
    }
}

bool match(char *text, const char *match, char *result, bool ignore_special)
{
    char *pText = text;
    char *pResult = result;
    bool found = false;
    int size = strlen(match);

    char beginSpecial[2] = {'<', '\"'};
    char endSpecial[2] = {'>', '\"'};
    bool hasSpecial[2] = {false, false};

    while (*pText != 0)
    {
        char p = *pText++;
        *pResult++ = p;

        if (!ignore_special)
        {
            bool skip = false;
            for (unsigned int i = 0; i < sizeof(hasSpecial); i++)
            {
                if (hasSpecial[i])
                {
                    if (p == endSpecial[i])
                        hasSpecial[i] = false;
                    skip = true;
                }
            }

            if (skip)
                continue;

            for (unsigned int i = 0; i < sizeof(beginSpecial); i++)
            {
                if (p == beginSpecial[i])
                {
                    hasSpecial[i] = true;
                    skip = true;
                    break;
                }
            }

            if (skip)
                continue;
        }

        for (int i = 0; i < size; i++)
        {
            if (p == match[i])
            {
                *--pResult = 0;
                found = true;
                break;
            }
        }

        if (found)
            break;
    }

    if (!found)
    {
        *pResult = 0;
        return false;
    }

    strcpy(text, pText);
    return true;
}

bool get_line(const char *text, char *result)
{
    bool ret = match((char*)text, "\n", result);
    if (ret)
    {
        int size = strlen(result);
        if ((size > 0) && (result[size - 1] == '\r'))
            result[size - 1] = 0;
    }

    return ret;
}

void trim(char *text)
{
    char *pText = text;

    while (*pText != 0)
    {
        if ((*pText == ' ') || (*pText == '\t'))
            pText++;
        else
            break;
    }

    strcpy(text, pText);
    int size = strlen(text);

    while (--size >= 0)
    {
        if ((text[size] == ' ') || (text[size] == '\t'))
            text[size] = 0;
        else
            break;
    }
}

void skip(char *text, const char *skip)
{
    char *pText = text;

    while (*pText != 0)
    {
        const char *pSkip = skip;
        bool found = false;

        while (*pSkip != 0)
        {
            if (*pText == *pSkip)
            {
                pText++;
                found = true;
                break;
            }
            pSkip++;
        }

        if (!found)
            break;
    }

    strcpy(text, pText);
}

__END_SYS
