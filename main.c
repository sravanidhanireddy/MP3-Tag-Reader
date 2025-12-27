#include <stdio.h>
#include <string.h>
#include "def.h"
#include "valid.h"

int main(int argc, char *argv[])
{
    char arr[6][5] = {"TPE1", "TIT2", "TALB", "TYER", "TCON", "TCOM"};

    if(argc < 2)
    {
        printf("\nUsage:\n");
        printf("  %s -v <filename.mp3>\n", argv[0]);
        printf("  %s -e <filename.mp3> <option> <new_data>\n", argv[0]);
        printf("  %s -h\n", argv[0]);
        return 1;
    }

    if(strcmp(argv[1], "-h") == 0)
    {
        printf("\n=================== HELP ===================\n");
        printf("Usage:\n");
        printf("  -v <file>                View MP3 tags\n");
        printf("  -e <file> <option> data  Edit tag\n");
        printf("  -h                       Show help\n");
        printf("--------------------------------------------\n");
        printf("Options:\n");
        printf("  -t   Title\n");
        printf("  -y   Year\n");
        printf("  -a   Album\n");
        printf("  -c   Composer\n");
        printf("  -A   Artist\n");
        printf("  -C   Genre / Content Type\n");
        printf("============================================\n\n");
        return 0;
    }
    else if(strcmp(argv[1], "-v") == 0)
    {
        if(argc != 3)
        {
            printf("[ERROR] Missing MP3 filename.\n");
            return 1;
        }
        if(!vail(argv[2]))
        {
            printf("[ERROR] Invalid MP3 file name.\n");
            return 1;
        }
        viewmp(arr, 6, 5, argv[2]);
    }
    else if(strcmp(argv[1], "-e") == 0)
    {
        if(argc < 5)
        {
            printf("[ERROR] Incorrect usage.\n");
            printf("Usage: %s -e <file.mp3> <option> <new_data>\n", argv[0]);
            return 1;
        }

        char *file = argv[2];
        char *opt = argv[3];
        char newdata[1024] = "";
        for(int i = 4; i < argc; i++)
        {
            strcat(newdata, argv[i]);
            if(i < argc - 1) strcat(newdata, " ");
        }

        if(!vail(file))
        {
            printf("[ERROR] Invalid MP3 file name.\n");
            return 1;
        }

        if(strcmp(opt, "-t") == 0)      
            editmp3(file, "TIT2", newdata);
        else if(strcmp(opt, "-y") == 0) 
            editmp3(file, "TYER", newdata);
        else if(strcmp(opt, "-a") == 0) 
            editmp3(file, "TALB", newdata);
        else if(strcmp(opt, "-c") == 0) 
            editmp3(file, "TCOM", newdata);
        else if(strcmp(opt, "-A") == 0) 
            editmp3(file, "TPE1", newdata);
        else if(strcmp(opt, "-C") == 0) 
            editmp3(file, "TCON", newdata);
        else 
            printf("[ERROR] Unknown option. Use -h for help.\n");
    }
    else
    {
        printf("[ERROR] Invalid command. Use -h for help.\n");
    }
    return 0;
}