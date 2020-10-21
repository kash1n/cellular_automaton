# cellular_automaton
Сellular automaton interpreter

Build: make

Run: ./main [command file] [input states file]  
or   ./main [command file] [input states file] [flag] [milliseconds]

Example: ./main ./input_files/game_of_life_commands.txt ./input_files/game_of_life_structs/gosper_gun.txt  
or       ./main ./input_files/game_of_life_commands.txt ./input_files/game_of_life_structs/gosper_gun.txt -r 200

В файле [command file] записываются команды для клеточного автомата в формате [previous state] [states of neighbors] [next state].  
Соседние клетки в последовательности [states of neighbors] перечислены по часовой стрелке, т.е. следующим образом:  
; 1 2 3  
; 8 - 4  
; 7 6 5  
