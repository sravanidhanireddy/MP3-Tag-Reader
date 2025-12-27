#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "def.h"

void viewmp(char arr[][5], int n, int m, char *file)
{
    FILE *fp = fopen(file, "rb");
    if(!fp)
    {
        printf("[ERROR] Could not open file: %s\n", file);
        return;
    }

    // Check ID3 header
    char header[10];
    fread(header, 1, 10, fp);
    if(strncmp(header, "ID3", 3) != 0)
    {
        printf("[ERROR] No ID3 tag found in this MP3.\n");
        fclose(fp);
        return;
    }

    printf("\n================ MP3 TAG DETAILS ================\n");
    printf("File       : %s\n", file);
    printf("-----------------------------------------------\n");

    // Read frames until we find our tags
    while(1)
    {
        char frameHeader[10];
        if(fread(frameHeader, 1, 10, fp) < 10)
            break; // End of frames

        // Frame ID
        char frameID[5] = {0};
        memcpy(frameID, frameHeader, 4);

        // Frame size
        int size = (frameHeader[4] << 24) |
                   (frameHeader[5] << 16) |
                   (frameHeader[6] << 8)  |
                   (frameHeader[7]);

        if(size <= 0) break;

        // ✅ FIX: use char * for malloc
        char *data = malloc(size + 1);
        if(!data)
        {
            printf("[ERROR] Memory allocation failed.\n");
            fclose(fp);
            return;
        }

        fread(data, 1, size, fp);
        data[size] = '\0';   // null terminate

        char *text = data + 1;   // skip encoding byte

        if(strcmp(frameID, "TIT2") == 0) printf("Title      -> %s\n", text);
        if(strcmp(frameID, "TPE1") == 0) printf("Artist     -> %s\n", text);
        if(strcmp(frameID, "TALB") == 0) printf("Album      -> %s\n", text);
        if(strcmp(frameID, "TYER") == 0) printf("Year       -> %s\n", text);
        if(strcmp(frameID, "TCOM") == 0) printf("Composer   -> %s\n", text);
        if(strcmp(frameID, "TCON") == 0) printf("Genre      -> %s\n", text);

        free(data);
    }

    printf("-----------------------------------------------\n\n");
    fclose(fp);
}

void editmp3(char *filename, char *tag, char *newdata)
{
    FILE *fp = fopen(filename, "rb+");
    if(!fp)
    {
        printf("[ERROR] Could not open file: %s\n", filename);
        return;
    }

    // Skip ID3 header
    char header[10];
    fread(header, 1, 10, fp);
    if(strncmp(header, "ID3", 3) != 0)
    {
        printf("[ERROR] No ID3 tag found in this MP3.\n");
        fclose(fp);
        return;
    }

    // Loop through frames
    while(1)
    {
        long pos = ftell(fp);
        char frameHeader[10];
        if(fread(frameHeader, 1, 10, fp) < 10)
            break; // End of frames

        char frameID[5] = {0};
        memcpy(frameID, frameHeader, 4);

        int size = (frameHeader[4] << 24) |
                   (frameHeader[5] << 16) |
                   (frameHeader[6] << 8)  |
                   (frameHeader[7]);

        if(size <= 0) break;

        if(strcmp(frameID, tag) == 0)
        {
            // ✅ FIX: use char * for malloc
            char *data = malloc(size + 1);
            if(!data)
            {
                printf("[ERROR] Memory allocation failed.\n");
                fclose(fp);
                return;
            }

            fread(data, 1, size, fp);
            data[size] = '\0';

            if((int)strlen(newdata) > size - 1)  // -1 for encoding byte
            {
                printf("[ERROR] New data is too large for tag %s.\n", tag);
                free(data);
                fclose(fp);
                return;
            }

            // Seek back to frame data start (skip 10-byte header)
            fseek(fp, pos + 10, SEEK_SET);

            // Write encoding byte (keep as 0 for ISO-8859-1)
            fputc(0, fp);

            // Write new text
            fwrite(newdata, 1, strlen(newdata), fp);

            // Pad with zeros if needed
            int remaining = size - 1 - strlen(newdata);
            for(int i = 0; i < remaining; i++)
                fputc(0, fp);

            printf("[OK] Tag updated successfully\n");
            printf("Field : %s\n", tag);
            printf("Value : %s\n", newdata);

            free(data);
            fclose(fp);
            return;
        }

        // Skip frame data if not matching
        fseek(fp, size, SEEK_CUR);
    }

    printf("[ERROR] Tag %s not found in file.\n", tag);
    fclose(fp);
}