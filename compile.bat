g++ -c main.cpp
g++ -c Calendar.cpp
g++ -c Day.cpp
g++ -c Event.cpp
g++ -c TextField.cpp
g++ -c Checkbox.cpp
g++ -c CheckboxOption.cpp

move *.o ./objects
g++ -g -o "Calendar.exe" ./objects/*.o ./Data/resource.res -mwindows -lmingw32 -lsfml-system -lsfml-window -lsfml-graphics


