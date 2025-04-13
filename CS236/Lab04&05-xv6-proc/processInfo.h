#pragma once
struct processInfo
{
    int ppid;
    int psize;
    int numberContextSwitches;
    char name[16];
};
