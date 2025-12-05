#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Car {
private:
    string carId, model, driverName;
    Car* next;

public:
    // Constructors
    Car(string carID, string mod, string driver) : carId(carID), model(mod), driverName(driver), next(nullptr) {}
    Car(string carID, string mod, string driver, Car* nextCar) : carId(carID), model(mod), driverName(driver), next(nextCar) {}
    
    // Getters
    string getCarId() const {return carId;}
    string getModel() const {return model;}
    string getDriverName() const {return driverName;}
    string getInfo() const {return "Car ID: "+carId+"\nModel: "+model+"\nDriver: "+driverName+"\n";}
    Car* getNext() const {return next;}
    
    // Setters
    void setCarId(string carID) {carId=carID;}
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

    void initialize(){
        for(int i=0; i<parkings.size(); i++){
            while(!carQ.isEmpty() && !parkings[i].isFull()){
                parkings[i].push(carQ.dequeue());
            }
        }
    }

    int addToQueue(string carId, string model, string driverName){
        Car* car = new Car(carId, model, driverName);
        int result = carQ.enqueue(car);
        initialize();
        return result;
    }
    int insert(Car* car){ //0 -> "Successful", 1 -> "parkinglot is full"
        for(MyStack parking : parkings){
            if(!parking.push(car)) return 0;
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
        for(MyStack parking : parkings){
            if(parking.getTop()==car){
                parking.pop();
                initialize();
                return 0;
            }
        }
        return 1;
    }

    pair<int, int> find(Car* car){
        //search all stacks and return <stack number, the depth in which the car is>
    }
};

// temporary main function for testing purposes without gui
int main(){
    int queueCapacity=30, stackNumber=6, stackCapacity=10;
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
- 
*/