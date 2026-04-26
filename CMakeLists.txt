cmake_minimum_required(VERSION 3.14)
project(HotelManagement)
set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    add_definitions(-DNOMINMAX -DWIN32_LEAN_AND_MEAN)
endif()

include(FetchContent)

FetchContent_Declare(
    xlnt
    GIT_REPOSITORY https://github.com/tfussell/xlnt.git
    GIT_TAG master
)
FetchContent_MakeAvailable(xlnt)

include_directories(include)

add_executable(hotel
    src/main.cpp
    src/user.cpp
    src/room.cpp
    src/booking.cpp
    src/hotel.cpp
    src/util.cpp
)

if(MINGW)
    target_link_options(hotel PRIVATE -static-libgcc -static-libstdc++)
endif()

target_link_libraries(hotel PRIVATE xlnt)