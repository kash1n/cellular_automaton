# cellular_automaton
**Сellular automaton interpreter**

**Build**:  
    make

**Run**:  
    ./main [command file] [input states file]  
    ./main [command file] [input states file] [flag] [milliseconds]

**Example**:  
    ./main ./input_files/game_of_life_commands.txt ./input_files/game_of_life_structs/gosper_gun.txt  
    ./main ./input_files/game_of_life_commands.txt ./input_files/game_of_life_structs/gosper_gun.txt -r 200

- - -

**Description (Russian)**:  
В файле [command file] записываются команды для клеточного автомата в формате [previous state] [states of neighbors] [next state].  
Соседние клетки в последовательности [states of neighbors] перечислены по часовой стрелке, т.е. следующим образом:  
    1 2 3  
    8 - 4  
    7 6 5  
В файле [input states file] перечислены индексы клеток и начальные состояния в них в формате [i] [j] [state]. Во всех остальных клетках, не перечисленных в файле
[input states file], начальное состояние будет равным 0. Пример (Планер для игры "Жизнь"):  
    ; Glider  
    ; - - o  
    ; o - o  
    ; - o o  
    2 0 1  
    2 1 1  
    2 2 1  
    1 2 1  
    0 1 1  
При задании флага -r или --run и указании скорости обновления в аргументе [milliseconds] клеточный автомат будет обновляться самостоятельно до остановки (т.е. ситуации, когда следующее состояние поля клеток совпадает с предыдущим) или до бесконечности.  
В представленной реализации поле ограничено, верхняя граница поля «соединена» с нижней, а левая граница — с правой, что представляет собой эмуляцию поверхности тора, но на экране поле всегда отображается в виде равномерной сетки.
