#pragma once
#include <stdio.h>
#include <stdexcept>

inline void problemLoading(const char *filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
    throw std::runtime_error("Loading failed");
}
