cmake_minimum_required(VERSION 3.20)
project(librazerblade)

set(CMAKE_CXX_STANDARD 14)
set(INSTALL_DIR ${PROJECT_BINARY_DIR}/../build/msvc)

add_definitions(-DLIBUSB_DEFAULT_IMPLEMENTATION)
add_definitions(-DWITH_DEFAULT_DEVICES)


add_library(librazerblade SHARED
        Usb/UsbInterface.cpp Usb/UsbInterface.h Core.h BladeStructs.h Utils.cpp Utils.h librazerblade.h UsbInterfaceStructs.h librazerblade.cpp Packets/PacketFactory.cpp Packets/PacketFactory.h Packets/PacketUtil.h LaptopDescription.h Laptop.cpp Laptop.h Packets/PacketUtil.cpp DescriptionStorage.cpp DescriptionStorage.h BladeDeviceId.h)


set_target_properties(librazerblade PROPERTIES PUBLIC_HEADER "librazerblade.h;Core.h;UsbInterfaceStructs.h;BladeStructs.h;BladeDeviceId.h;LaptopDescription.h")
install(TARGETS librazerblade
        RUNTIME DESTINATION "${INSTALL_DIR}/bin"
        LIBRARY DESTINATION "${INSTALL_DIR}/lib"
        ARCHIVE DESTINATION "${INSTALL_DIR}/lib"
        PUBLIC_HEADER DESTINATION "${INSTALL_DIR}/include"
        )