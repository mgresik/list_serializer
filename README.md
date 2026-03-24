# Создаем директорию для сборки
mkdir build
cd build

# Конфигурируем проект
cmake ..

# Собираем
cmake --build .

# Запускаем (из директории build, там же должен быть файл .in)
./list_serializer
