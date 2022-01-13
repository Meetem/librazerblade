//
// Created by alex on 10/01/2022.
//

#include "DescriptionStorage.h"
#include <array>

namespace librazerblade {
    DescriptionStorage gStorage;

    LaptopDescription DescriptionStorage::get(uint16_t vendor, uint16_t product, int32_t* idx)
    {
        if (idx != nullptr)
            *idx = -1;

        for (int32_t i = 0; i < descriptions.size(); i++) {
            auto d = descriptions[i];

            if (d.vendorId == vendor && d.productId == product) {
                if (idx != nullptr)
                    *idx = i;

                return d;
            }
        }

        return {};
    }

    void DescriptionStorage::put(LaptopDescription description)
    {
        if (description.productId == 0 && description.vendorId == 0)
            return;

        int idx = -1;
        auto old = get(description.vendorId, description.productId, &idx);

        if (idx < 0) {
            descriptions.push_back(description);
            return;
        }

        descriptions[idx] = description;
    }

    LaptopDescription* DescriptionStorage::getAll(int32_t* size)
    {
        if (size != nullptr)
            *size = (int32_t) descriptions.size();

        return descriptions.data();
    }

    void DescriptionStorage::set(int32_t idx, LaptopDescription description)
    {
        if (idx < 0 || idx >= descriptions.size())
            return;

        descriptions[idx] = description;
    }

    void DescriptionStorage::initializeWithDefault()
    {
        auto _fan2018 = LaptopFan{DEFAULT_MIN, 5000};
        auto _fanDefault = LaptopFan{DEFAULT_MIN, 5000};
        auto _fanHigh = LaptopFan{DEFAULT_MIN, 5300};
        auto _fanStealth = LaptopFan{DEFAULT_MIN, 5300};

        auto defaultCaps = (BladeCapabilities) BladeCapabilities::None;
        auto creator = (BladeCapabilities) BladeCapabilities::Creator;
        auto boost = (BladeCapabilities) BladeCapabilities::Boost;
        auto creatorAndBoost = (BladeCapabilities) (creator | boost);

        std::array<LaptopDescription, 27> laptops{
                //14"
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_14_2021, "Blade 14\" 2021",
                                  _fanDefault, defaultCaps},

                //15"
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2016_END, "Blade 15\" 2016",
                                  _fan2018, defaultCaps},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2018_ADV, "Blade 2018 15\" advanced",
                                  _fan2018, defaultCaps},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2018_BASE, "Blade 2018 15\" base",
                                  _fan2018, defaultCaps},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2018_MERC, "Blade 2018 15\" Mercury edition",
                                  _fan2018, defaultCaps},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2019_BASE, "Blade 2019 15\" base",
                                  _fanHigh, defaultCaps},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2019_ADV, "Blade 2019 15\" advanced",
                                  _fanHigh, creator},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2019_MERC, "Blade 2019 15\" Mercury edition",
                                  _fanHigh, creator},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2020_BASE, "Blade 2020 15\" base",
                                  _fanHigh, defaultCaps},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2021_BASE, "Blade 2021 15\" base",
                                  _fanHigh, defaultCaps},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2020_ADV, "Blade 2020 15\" advanced",
                                  _fanHigh, creatorAndBoost},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2021_ADV, "Blade 2021 15\" advanced",
                                  _fanHigh, creatorAndBoost},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_LATE_2020_BASE,
                                  "Blade 2020 (Late) 15\" base",
                                  _fanHigh, defaultCaps},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2021_MID_ADV,
                                  "Blade 2021 (Mid) 15\" advanced",
                                  _fanHigh, creatorAndBoost},

                //Stealth
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2017_STEALTH_MID, "Blade 2017 Stealth",
                                  _fanStealth, defaultCaps},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2017_STEALTH_END,
                                  "Blade 2017 (Late) stealth",
                                  _fanStealth, defaultCaps},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2019_STEALTH, "Blade 2019 Stealth",
                                  _fanStealth, defaultCaps},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2019_STEALTH_GTX,
                                  "Blade 2019 Stealth (With GTX)",
                                  _fanStealth, defaultCaps},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2020_STEALTH, "Blade 2020 Stealth",
                                  _fanStealth, defaultCaps},

                //Book
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BOOK_2020, "Book 2020",
                                  _fanHigh},

                //Pro
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2021_PRO_FHD, "Blade 2021 Pro FHD",
                                  _fanHigh, defaultCaps},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2020_PRO_FHD, "Blade 2020 pro FHD",
                                  _fanHigh, defaultCaps},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_PRO_2019, "Blade 2019 pro",
                                  _fanHigh, defaultCaps},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2018_PRO_FHD, "Blade 2018 pro FHD",
                                  _fanHigh, defaultCaps},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2017_PRO, "Blade 2017 pro",
                                  _fanHigh, defaultCaps},
                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_2016_PRO, "Blade 2016 pro",
                                  _fanHigh, defaultCaps},

                LaptopDescription{RAZER_BLADE_VENDOR, BladeDeviceId::BLADE_QHD, "Blade QHD",
                                  _fanDefault, defaultCaps},
        };

        for (auto d: laptops) {
            this->put(d);
        }
    }

    void DescriptionStorage::clear()
    {
        for(auto &d : descriptions){
            if(d.userData.ptr != nullptr && d.userData.autoFree > 0){
                free(d.userData.ptr);
            }
        }

        descriptions.clear();
    }
}