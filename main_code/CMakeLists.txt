cmake_minimum_required(VERSION 3.10)
project(FifteenPuzzle)

# Указываем стандарт
set(CMAKE_CXX_STANDARD 17)

# ПУТЬ К SFML (Убедитесь, что он ведет к версии для Visual Studio!)
set(SFML_DIR "C:/Users/user/SFML-2.6.1/lib/cmake/SFML") 

# Поиск библиотек
find_package(SFML 2.6.1 COMPONENTS graphics window system REQUIRED)

# Исходные файлы
add_executable(FifteenPuzzle 
    main.cpp 
    GameLogic.cpp 
    GameLogic.h
    Render.cpp 
    Render.h
    Menu.cpp 
    Menu.h
)

# Линковка (компоновка) — ЭТО РЕШАЕТ ВАШУ ОШИБКУ
target_link_libraries(FifteenPuzzle sfml-graphics sfml-window sfml-system)

# Копирование DLL, чтобы игра запустилась
if(WIN32)
    add_custom_command(TARGET FifteenPuzzle POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SFML_DIR}/../../../bin"
        $<TARGET_FILE_DIR:FifteenPuzzle>
    )
endif()