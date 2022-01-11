//
// Created by alex on 10/01/2022.
//

#ifndef LIBRAZERBLADE_DESCRIPTIONSTORAGE_H
#define LIBRAZERBLADE_DESCRIPTIONSTORAGE_H

#include "BladeStructs.h"
#include "LaptopDescription.h"
#include <vector>

namespace librazerblade {
    class DllExport DescriptionStorage
    {
    public:
        LaptopDescription get(uint16_t vendor, uint16_t product, int32_t* idx);
        void put(LaptopDescription description);
        LaptopDescription *getAll(int32_t *size);
        void set(int32_t idx, LaptopDescription description);

        void initializeWithDefault();
    protected:
        std::vector<LaptopDescription> descriptions;
    };

    extern DescriptionStorage gStorage;
}

#endif //LIBRAZERBLADE_DESCRIPTIONSTORAGE_H
