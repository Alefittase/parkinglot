#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Car {
private:
    int carId;
    string model, driverName;
    Car* next;

public:
    // Constructors
    Car() : carId(-1), model(""), driverName(""), next(nullptr) {}
    Car(int carID, string mod, string driver) : carId(carID), model(mod), driverName(driver), next(nullptr) {}
    Car(int carID, string mod, string driver, Car* nextCar) : carId(carID), model(mod), driverName(driver), next(nextCar) {}
    
    // Getters
    int getCarId() const {return carId;}
    string getModel() const {return model;}
    string getDriverName() const {return driverName;}
    // string getInfo() const {return "Car ID: " + (string)carId + "\nModel: " + model + "\nDriver: " + driverName + "\n";} ---> causes problems !?
    Car* getNext() const {return next;}
    
    // Setters
    void setCarId(int carID) {carId=carID;}
    void setModel(string mod) {model=mod;}
    void setDriverName(string driver) {driverName=driver;}
    void setNext(Car* nextCar) {next=nextCar;}
};

class MyStack {
private:
    int size;
    int capacity;
    Car* top=nullptr;

public:
    // Constructors
    MyStack() : size(0), top(nullptr) {}
    MyStack(Car* firstCar) : size(1) {
        push(firstCar);
    }
    
    // Getters
    int getSize() const {return size;}
    int getCapacity() const {return capacity;}
    Car* getTop() const {
        if(size==0) return nullptr;
        return top;
    }
    
    // Setters
    void setSize(int newSize) {size=newSize;}
    void setCapacity(int newCap) {capacity=newCap;}
    void setTop(Car* newTop) {top=newTop;}
    
    int isEmpty(){
        return (size==0);
    }

    int isFull(){
        return (size==capacity);
    }

    int push(Car* newest){ //0 -> "successful", 1 -> "Stack is full"
        if(isFull()) return 1;
        if(size>0) newest->setNext(top);
        top = newest;
        size++;
        return 0;
    }
    
    Car* pop(){
        if(isEmpty()) return nullptr;
        Car* returnee=top;
        if(size>1) top=top->getNext();
        size--;
        return returnee;
    }

    void sort(){}

    ~MyStack() {
        while (top != nullptr) {
            Car* temp = top;
            top = top->getNext();
            delete temp;
        }
    }
};

class MyQueue {
private:
    int size;
    int capacity;
    Car* front=nullptr;
    Car* rear=nullptr;

public:
    // Constructors
    MyQueue() : size(0), front(nullptr), rear(nullptr) {}
    MyQueue(Car* firstCar) : size(1) {
        enqueue(firstCar);
    }
    
    // Getters
    int getSize() const {return size;}
    int getCapacity() const {return capacity;}
    Car* getFront() const {return front;}
    Car* getRear() const {return rear;}

    // Setters
    void setSize(int newSize) {size=newSize;}
    void setFront(Car* newFront) {front=newFront;}
    void setCapacity(int newCap) {capacity=newCap;}
    
    int isEmpty(){
        return (size==0);
    }

    int isFull(){
        return (size==capacity);
    }

    int enqueue(Car* car){ //0 -> "successful", 1 -> "Queue is full"
        if(isFull()) return 1;
        if(size>0) rear->setNext(car);
        if(isEmpty()) front=car;
        rear = car;
        size++;
        return 0;
    }
    
    Car* dequeue(){
        if(isEmpty()) return nullptr;
        Car* returnee=front;
        if(size>1) front=front->getNext();
        size--;
        return returnee;
    }

    void printQueue(){
        Car* car = front;
        for(int i=0; i<size; i++){
        cout<<car->getCarId()<<" ";
        car=car->getNext();
        }
        cout<<"\n";
    }

    ~MyQueue() {
        while (front != nullptr) {
            Car* temp = front;
            front = front->getNext();
            delete temp;
        }
    }
};

class Parkinglot {
private:
    MyQueue carQ;
    vector<MyStack> parkings;
public:
    Parkinglot(int queueCap, int stackNum, int stackCap){
        carQ.setCapacity(queueCap);
        for(int i=0; i<stackNum; i++){
            MyStack stack;
            stack.setCapacity(stackCap);
            parkings.push_back(stack);
        }
    }

    int initialize(){ //0 -> queue successfully emptied. 1 -> parkinglot is full please wait
        for(int i=0; i<parkings.size(); i++)
            while(!carQ.isEmpty() && !insert(carQ.dequeue())); //while queue is not empty and insert to parking is successful
        if(carQ.isEmpty()) return 0;
        return 1;
    }

    int addToQueue(int carId, string model, string driverName){
        Car* car = new Car(carId, model, driverName);
        int result = carQ.enqueue(car);
        initialize();
        return result;
    }
    int insert(Car* car){
        for(MyStack &parking : parkings){
            if(parking.push(car) == 0) return 0;
        }
        return 1;
    }
    int insertAt(Car* car, int i){ // returns as push states
        return parkings[i].push(car);
    }
    void sort(int i){
        parkings[i].sort();
    }
    int move(int i, int j){// 0 -> "move successful", 1 -> "not enough space in parkinglot for emptying parking i"
        int cnt=0;
        while(!parkings[i].isEmpty() && cnt<parkings.size()){
            if(parkings[j].push(parkings[i].pop())) j++;
            if(j==i) j++;
            if(j==parkings.size()) j=0;
            cnt++;
        }
        if(parkings[i].isEmpty()) return 0;
        return 1;
    }
    int popCar(Car* car){ // 0 -> "successful", 1 -> "car not found"
        for(int i=0; i<parkings.size(); i++){
            if(parkings[i].isEmpty()) continue;
            if(parkings[i].getTop()->getCarId()==car->getCarId()){
                parkings[i].pop();
                initialize();
                return 0;
            }
        }
        return 1;
    }

    pair<int, int> find(Car* car){
        pair<int, int> result={-1, -1};
        Car* pcar;
        for(int i=0; i<parkings.size(); i++){
            if(parkings[i].isEmpty()) continue; 
            pcar = parkings[i].getTop();
            for(int j=0; j<parkings[i].getSize(); j++){
                if(pcar->getCarId()==car->getCarId()){
                    result.first=i;
                    result.second=j;
                    return result;
                }
                pcar=pcar->getNext();
            }
        }
        return result;
    }

    void display(){
    cout<<"Cars in queue:\n";
    if(carQ.isEmpty()) {
        cout << "(empty)";
    } else {
        Car* car = carQ.getFront();
        for(int i=0; i<carQ.getSize(); i++){
            if(car != nullptr) {
                cout<<car->getCarId()<<" ";
                car = car->getNext();
            }
        }
    }
    cout<<"\nCars in parkinglot:\n";
    for(int stackIdx = 0; stackIdx < parkings.size(); stackIdx++){ 
        cout << "Stack " << stackIdx << ": ";
        MyStack& parking = parkings[stackIdx];
        
        // Show empty slots
        for(int i=0; i<parking.getCapacity()-parking.getSize(); i++) 
            cout<<"- ";
        
        // Show cars (top to bottom)
        Car* currentCar = parking.getTop();
        while(currentCar != nullptr){
            cout<<currentCar->getCarId()<<" ";
            currentCar = currentCar->getNext();
        }
        cout<<"\n";
    }
}
};

// temporary main function for testing purposes without gui
int main(){
    string carIDs[60] = {
        "CAR001", "CAR002", "CAR003", "CAR004", "CAR005",
        "CAR006", "CAR007", "CAR008", "CAR009", "CAR010",
        "CAR011", "CAR012", "CAR013", "CAR014", "CAR015",
        "CAR016", "CAR017", "CAR018", "CAR019", "CAR020",
        "CAR021", "CAR022", "CAR023", "CAR024", "CAR025",
        "CAR026", "CAR027", "CAR028", "CAR029", "CAR030",
        "CAR031", "CAR032", "CAR033", "CAR034", "CAR035",
        "CAR036", "CAR037", "CAR038", "CAR039", "CAR040",
        "CAR041", "CAR042", "CAR043", "CAR044", "CAR045",
        "CAR046", "CAR047", "CAR048", "CAR049", "CAR050",
        "CAR051", "CAR052", "CAR053", "CAR054", "CAR055",
        "CAR056", "CAR057", "CAR058", "CAR059", "CAR060"
    };
    string models[60] = {
        "Toyota Camry", "Honda Civic", "Ford Mustang", "Toyota Camry", "Honda Accord",
        "BMW 3 Series", "Tesla Model 3", "Audi A4", "Mercedes C-Class", "Toyota Camry",
        "Ford F-150", "Chevrolet Silverado", "Honda CR-V", "Toyota RAV4", "BMW 3 Series",
        "Tesla Model Y", "Subaru Outback", "Jeep Wrangler", "Toyota Corolla", "Honda Civic",
        "Ford Explorer", "Chevrolet Equinox", "Nissan Altima", "Hyundai Sonata", "Kia Sportage",
        "Volkswagen Golf", "Mazda CX-5", "Lexus RX", "Acura MDX", "Toyota Camry",
        "BMW X5", "Mercedes E-Class", "Audi Q5", "Tesla Model S", "Ford Mustang",
        "Chevrolet Camaro", "Dodge Charger", "Toyota Highlander", "Honda Pilot", "Ford F-150",
        "Ram 1500", "GMC Sierra", "Toyota Tacoma", "Honda Civic", "Jeep Grand Cherokee",
        "Land Rover Range Rover", "Porsche 911", "Subaru Forester", "Mazda3", "Hyundai Elantra",
        "Kia Telluride", "Volkswagen Tiguan", "Genesis G80", "Cadillac Escalade", "Ford Bronco",
        "Chevrolet Tahoe", "Toyota 4Runner", "Honda Odyssey", "Nissan Rogue", "BMW 5 Series"
    };
    string driverNames[60] = {
        "John Smith", "Emma Johnson", "Michael Williams", "Sophia Brown", "David Jones",
        "Olivia Garcia", "James Miller", "Ava Davis", "Robert Rodriguez", "Isabella Martinez",
        "William Hernandez", "Mia Lopez", "Joseph Gonzalez", "Charlotte Wilson", "Thomas Anderson",
        "Amelia Thomas", "Charles Taylor", "Harper Moore", "Daniel Jackson", "Evelyn Martin",
        "Matthew Lee", "Abigail Perez", "Christopher Thompson", "Emily White", "Andrew Harris",
        "Elizabeth Sanchez", "Joshua Clark", "Sofia Ramirez", "Ryan Lewis", "Avery Robinson",
        "Nicholas Walker", "Ella Young", "Jonathan King", "Scarlett Wright", "Benjamin Scott",
        "Grace Torres", "Samuel Nguyen", "Chloe Hill", "Kevin Flores", "Victoria Green",
        "Jason Adams", "Lily Nelson", "Eric Baker", "Zoey Hall", "Brian Rivera",
        "Penelope Campbell", "Timothy Mitchell", "Luna Carter", "Steven Roberts", "Hannah Phillips",
        "Justin Evans", "Aria Turner", "Brandon Parker", "Addison Collins", "Alexander Edwards",
        "Natalie Stewart", "Patrick Morris", "Stella Rogers", "Nathan Reed", "Savannah Cook"
    };


    ////////////////////////////////////////////////////////////////////////////////////////
    int queueCapacity=30, stackNumber=6, stackCapacity=10;
    Parkinglot parkinglot(queueCapacity, stackNumber, stackCapacity);
    
    // Convert carIDs to integers
    int carIDnums[60];
    for(int i = 0; i < 60; i++) {
        string numStr = carIDs[i].substr(3);
        carIDnums[i] = stoi(numStr);
    }
    
    cout << "=== Initial State ===" << endl;
    cout << "Queue capacity: " << queueCapacity << endl;
    cout << "Number of stacks: " << stackNumber << endl;
    cout << "Stack capacity: " << stackCapacity << endl;
    cout << endl;
    
    // Event 1: Enqueue 21 cars
    cout << "=== Event 1: Enqueue 21 cars ===" << endl;
    int added = 0;
    for(int i = 0; i < 21; i++) {
        int result = parkinglot.addToQueue(carIDnums[i], models[i], driverNames[i]);
        if(result == 0) {
            added++;
        } else {
            cout << "Failed to add car " << carIDnums[i] << " (queue full)" << endl;
        }
    }
    cout << "Successfully added " << added << " cars to queue" << endl;
    
    cout << "\n=== Current State (simplified) ===" << endl;
    
    // Event 2: Sort stack number 5
    cout << "\n=== Event 2: Sort stack 5 (index 4) ===" << endl;
    
    // Event 3: Move all cars from stack 0 to 2
    cout << "\n=== Event 3: Move cars from stack 0 to stack 2 ===" << endl;
    int moveResult = parkinglot.move(0, 2);
    cout << "Move result: " << moveResult << endl;
    
    // Event 4: Enqueue 30 more cars
    cout << "\n=== Event 4: Enqueue 30 more cars ===" << endl;
    added = 0;
    for(int i = 21; i < 51; i++) {
        int result = parkinglot.addToQueue(carIDnums[i], models[i], driverNames[i]);
        if(result == 0) {
            added++;
        }
    }
    cout << "Added " << added << " more cars" << endl;
    
    // Event 5: Pop car 30
    cout << "\n=== Event 5: Try to pop car 30 ===" << endl;

    // Event 6: Find car 12
    cout << "\n=== Event 6: Find car 12 ===" << endl;
    Car* tempCar12 = new Car(12, "temp", "temp");
    pair<int, int> findResult = parkinglot.find(tempCar12);
    delete tempCar12;  // Clean up
    if(findResult.first != -1) {
        cout << "Car 12 found at stack " << findResult.first 
             << ", position " << findResult.second << endl;
    } else {
        cout << "Car 12 not found"<< endl;
    }
    
    // Event 7: Pop a bunch of cars
    cout << "\n=== Event 7: Pop cars 5, 10, 15 ===" << endl;
    // Create dummy cars to pass to popCar
    Car* tempCar5 = new Car(5, "", "");
    Car* tempCar10 = new Car(10, "", "");
    Car* tempCar15 = new Car(15, "", "");
    
    cout << "Attempting to pop car 5..." << endl;
    parkinglot.popCar(tempCar5);
    cout << "Attempting to pop car 10..." << endl;
    parkinglot.popCar(tempCar10);
    cout << "Attempting to pop car 15..." << endl;
    parkinglot.popCar(tempCar15);
    
    delete tempCar5;
    delete tempCar10;
    delete tempCar15;
    
    // Event 8: Insert car at stack 5
    cout << "\n=== Event 8: Insert new car at stack 5 ===" << endl;
    Car* newCar = new Car(100, "Test Model", "Test Driver");
    int insertResult = parkinglot.insertAt(newCar, 4);
    if(insertResult == 0) {
        cout << "Car inserted at stack 5" << endl;
    } else {
        cout << "Stack 5 is full" << endl;
        delete newCar;  // Clean up if insertion failed
    }
    
    // Event 9: Enqueue 30 more cars
    cout << "\n=== Event 9: Enqueue 30 more cars ===" << endl;
    added = 0;
    // Use remaining cars
    for(int i = 51; i < 60; i++) {
        if(parkinglot.addToQueue(carIDnums[i], models[i], driverNames[i]) == 0) {
            added++;
        }
    }
    // Create additional cars
    for(int i = 60; i < 81; i++) {
        if(parkinglot.addToQueue(1000 + i, "Extra Model", "Extra Driver") == 0) {
            added++;
        }
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    return 0;
}
/*
series of events:
- enqueue 21 cars and put them normally into stacks (question: should parkinglot initialize after each enqueue or only when it's called or what?)
- sort stack number 5
- move all cars from stack 0 to 2
- enqueue 30 more cars
- pop car 30
- find car 12
- pop a bunch of cars
- insert car at stack 5
- enqueue 30 more
*/