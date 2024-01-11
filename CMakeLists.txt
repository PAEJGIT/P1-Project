cmake_minimum_required(VERSION 3.27)
project(P1_Program C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

add_executable(P1_Program main.c
        .packages/cJSON.c
        .packages/cJSON.h
        src/components/button.c
        src/components/button.h
        src/utils/helpers.c
        src/utils/helpers.h
        src/pages/DosagePage.c
        src/pages/DosagePage.h
        src/components/menubar.c
        src/pages/ResidentListPage.c
        src/pages/CasePage.h
        src/pages/ResidentProfilePage.c
        src/pages/OrderPage.c
        src/pages/StoragePage.c
        src/pages/CasePage.c
        src/components/listElements.c
        src/components/listElements.h
        src/configs/constants.h
        src/configs/constants.h
        src/utils/Checks.c
        src/utils/Checks.h
        src/components/icon.h
        src/components/icon.c
        src/components/separator.h
        src/components/separator.c
        src/components/flag.h
        src/components/flag.c
        src/components/action.c
        src/components/action.h
        src/components/label.c
        src/components/label.h
        src/utils/handlers.c
        src/utils/handlers.h
)
target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES})