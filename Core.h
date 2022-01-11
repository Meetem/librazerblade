//
// Created by alex on 08/01/2022.
//

#ifndef LIBRAZERBLADE_CORE_H
#define LIBRAZERBLADE_CORE_H

#include <cinttypes>

typedef struct
{
    uint8_t r, g, b;
} RGB24;

#define UNWRAP(...) __VA_ARGS__
#define CPPENUM(x, entries) namespace Enum##x {enum x UNWRAP entries;}; using namespace Enum##x

#define DllExport __declspec( dllexport )
#define CallType __cdecl

#endif //LIBRAZERBLADE_CORE_H
