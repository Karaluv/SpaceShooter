## How to play
The game is controlled by keyboard and mouse.
- **WASD** - move the spaceship
- **Right mouse button** - use special ability
- **ctrl** - move ship down
- **space** - move ship up

## Кто что делал
# Андрей
- Шаблоны: andrei_main, matrix; используются во всех классах этих файлов
- Работа с потоками ввода-вывода: game_core; функция ReadPhysData
- Exceptions: andrei_main, matrix; в функциях обсчёта физической процессов, ловятся в game_core
- C-Make: командная работа
- Переопределение операторов суммы, разности, ввода, вывода и другие: andrei_main, matrix; являются методами класса. 
# Сергей
- (обязательный пункт) наследование классов, виртуальные функции, правило пяти
- шаблоны
- работы с потоками ввода/вывода (для тестов работы программы)
- перегрузка операторов
- работа в CMake
- обработка исключений
# Егор
- паттрены singletone и stategy в файлах render_engine и render_object
- потоки ввода вывода в загрузку файлов obj_loader loader
- исключения напиханы везде
- работа с OpenGl 
- темплейты в loader.hpp и render_engine.cpp (где-то середина)
- cmake
- мультипоточность


