cmake_minimum_required(VERSION 3.10)
project(ListSerializer)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Опции компиляции
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# Добавляем исполняемый файл
add_executable(list_serializer 
    main.cpp
)

# Если нужно линковать дополнительные библиотеки (например, для UTF-8)
# target_link_libraries(list_serializer PRIVATE)