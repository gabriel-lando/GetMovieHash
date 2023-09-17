// https://trac.opensubtitles.org/projects/opensubtitles/wiki/HashSourceCodes

#include <iostream>
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
    std::string filename;

    if (argc != 2) 
    {
        std::cout << "Tip: use \"" << argv[0] << " <video_name.ext>\"" << std::endl;
        std::cout << "Enter file name: ";
        std::cin >> filename;
        return 1;
    }
    else {
        filename = std::string(argv[1]);
    }

    FILE* handle;
    auto result = fopen_s(&handle, filename.c_str(), "rb");

    if (result)
    {
        printf("Error openning file!");
        return 1;
    }

    uint64_t videoHash = compute_hash(handle);
    fclose(handle);
    
    printf("Movie Hash: %I64x\n\n", videoHash);

    printf("Access: https://www.opensubtitles.org/pb/search/sublanguageid-pob,por/moviehash-%I64x\n\n", videoHash);



    // Download URL: https://dl.opensubtitles.org/pb/download/sub/6240844

    return 0;
}