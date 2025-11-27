UI input -> enterance queue -> first empty stack (if all are full, return "parking is full")

parking:
    n stacks // based on linked list
    each stack has m nodes at most
        each node is a car:
            id, type, ...


functions:
    insert(car) // adds car to first not-full stack
    insertAt(car, i) // adds car to ith stack
    sort(i) // sorts cars in ith stack based on id
    move(i, j) //moves as many cars as possible from ith stack to jth stack
               //if j gets full, continue to j+1, j+2, ...
    pop(car) // removes the said car if possible

notes:
    dont use Stack/Queue/LinkedList libraries

how to compile and run:
    //g++ -o build/parkinglot src/main.cpp `fltk-config --cxxflags --ldflags`; ./build/parkinglot