#include <string.h>
#include <ctype.h>

int vail(char *s)
{
    char *pos1 = strstr(s, ".mp3");
    if(!pos1 || pos1 == s || *(pos1 + 4) != '\0')
        return 0;

    for(int i = 0; s[i] != '\0'; i++)
    {
        if(!isalpha(s[i]) && !isalnum(s[i]) && s[i] != '.')
            return 0;
    }
    return 1;
}
