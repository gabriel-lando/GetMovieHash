// https://trac.opensubtitles.org/projects/opensubtitles/wiki/HashSourceCodes

#include <stdio.h>
#include <stdlib.h>

#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#ifndef uint64_t
#define uint64_t unsigned long long
#endif

uint64_t compute_hash(FILE* handle)
{
    uint64_t hash, fsize;

    fseek(handle, 0, SEEK_END);
    fsize = ftell(handle);
    fseek(handle, 0, SEEK_SET);

    hash = fsize;

    for (uint64_t tmp = 0, i = 0; i < 65536 / sizeof(tmp) && fread((char*)&tmp, sizeof(tmp), 1, handle); hash += tmp, i++);
    fseek(handle, (long)MAX(0, fsize - 65536), SEEK_SET);
    for (uint64_t tmp = 0, i = 0; i < 65536 / sizeof(tmp) && fread((char*)&tmp, sizeof(tmp), 1, handle); hash += tmp, i++);

    return hash;
}

int main(int argc, char* argv[])
{
    FILE* handle;
    uint64_t myhash;

    if (argc != 2) 
    {
        printf("Use \"%s <video_name.ext>\"!", argv[0]);
        return 1;
    }

    auto result = fopen_s(&handle, (const char*)argv[1], "rb");

    if (result)
    {
        printf("Error openning file!");
        return 1;
    }

    myhash = compute_hash(handle);
    printf("Movie Hash: %I64x\n\n", myhash);

    printf("Access: https://www.opensubtitles.org/pb/search/sublanguageid-pob,por/moviehash-%I64x\n\n", myhash);


    fclose(handle);

    //system("pause");
    return 0;
}