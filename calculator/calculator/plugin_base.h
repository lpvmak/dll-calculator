#ifndef PLUGIN_BASE_H
#define PLUGIN_BASE_H
#include "operation.h"


extern "C" {
    __declspec(dllexport)  Operation* loadOperation();
}

#endif // !PLUGIN_BASE_H
