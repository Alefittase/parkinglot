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

    void merge_sort(vector<Car> &a, int l, int r){
        if(l>=r) return;
        int m=l+(r-l)/2;
        merge_sort(a, l, m);
        merge_sort(a, m+1, r);

        vector<Car> tmp;
        int i=l, j=m+1;
        while(i<=m && j<=r)
            tmp.push_back(a[i].getCarId()<=a[j].getCarId()?a[i++]:a[j++]);
        while(i<=m) tmp.push_back(a[i++]);
        while(j<=r) tmp.push_back(a[j++]);
        for(int k=0; k<tmp.size(); ++k)
            a[l+k]=tmp[k];
    }

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
        top=top->getNext();
        size--;
        return returnee;
    }

    void sort(){
        if(isEmpty()) return;
        vector<Car> v;
        while(size) v.push_back(*pop());
        merge_sort(v, 0, v.size()-1);
        while(v.size()){
            push(new Car(v[v.size()-1].getCarId(), v[v.size()-1].getModel(), v[v.size()-1].getModel()));
            v.pop_back();
        }
    }

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
        for(int i=0; i<parkings.size(); i++){
            while(!carQ.isEmpty()){
                Car* car = carQ.getFront();
                if(insert(car)) return 1;
                carQ.dequeue();
            }
        }
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
        int cap=parkings[0].getSize();
        while(!parkings[i].isEmpty() && cnt<(parkings.size()*cap+1)){
            if(parkings[j].isFull()) j++;
            if(j==i) j++;
            if(j==parkings.size()) j=0;
            parkings[j].push(parkings[i].pop());
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
        pair<int, int> result; result.first=-1; result.second=-1;
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
            cout<<"(empty)";
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
    int carIDs[100] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
        21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
        61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81
    };
    
    string models[100] = {
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
        "Chevrolet Tahoe", "Toyota 4Runner", "Honda Odyssey", "Nissan Rogue", "BMW 5 Series",
        "Tesla Model X", "Audi A6", "Mercedes GLC", "Toyota Prius", "Honda Fit",
        "Ford Escape", "Chevrolet Malibu", "Nissan Sentra", "Hyundai Tucson", "Kia Sorento",
        "Volkswagen Jetta", "Mazda6", "Lexus ES", "Acura RDX", "BMW X3",
        "Mercedes A-Class", "Audi Q7", "Tesla Cybertruck", "Ford Ranger", "Chevrolet Traverse",
        "Toyota Sienna", "Honda Passport"
    };
    
    string driverNames[100] = {
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
        "Natalie Stewart", "Patrick Morris", "Stella Rogers", "Nathan Reed", "Savannah Cook",
        "Oliver Murphy", "Ethan Rivera", "Logan Cooper", "Lucas Richardson", "Mason Howard",
        "Carter Ward", "Jayden Brooks", "Gabriel Sanders", "Luke Price", "Isaiah Bennett",
        "Henry Wood", "Sebastian Barnes", "Jack Ross", "Owen Henderson", "Wyatt Coleman",
        "Grayson Jenkins", "Leo Perry", "Julian Powell", "Mateo Long", "Dominic Hughes",
        "Lincoln Flores", "Josiah Washington"
    };

    int queueCapacity = 30, stackNumber = 6, stackCapacity = 10;
    Parkinglot parkinglot(queueCapacity, stackNumber, stackCapacity);
    
    cout<<"=== INITIAL STATE ===\n";
    cout<<"Queue capacity: "<<queueCapacity<<endl;
    cout<<"Number of stacks: "<<stackNumber<<endl;
    cout<<"Stack capacity: "<<stackCapacity<<endl;
    cout<<"Total parking capacity: "<<stackNumber * stackCapacity<<endl;
    
    parkinglot.display();
    cout<<"----------------------------------------\n\n";
    
    //////////////////////////////////////////////////////////////////
    cout<<"EVENT 1: Enqueue 21 cars\n";
    int added = 0;
    for(int i = 0; i < 21; i++) {
        int result = parkinglot.addToQueue(carIDs[i], models[i], driverNames[i]);
        if(result == 0) {
            added++;
        } else {
            cout<<"Queue full! Could not add car "<<carIDs[i]<<endl;
        }
    }
    cout<<"\nSuccessfully added "<<added<<" cars to queue\n";
    cout<<"\nAfter Event 1:\n";
    parkinglot.display();
    cout<<"----------------------------------------\n\n";

    ///////////////////////////////////////////////////////////////////
    cout<<"EVENT 2: Sort stack number 0\n";
    parkinglot.sort(0);
    cout<<"\nSuccessfully sorted stack 0 (no change, to be written later)\n";
    cout<<"\nAfter Event 2:\n";
    parkinglot.display();
    cout<<"----------------------------------------\n\n";
    
    
    //////////////////////////////////////////////////////////////////
    cout<<"EVENT 3: Move all cars from stack 0 to stack 2\n";
    int moveResult = parkinglot.move(0, 2);
    if(moveResult == 0) {
        cout<<"Move successful: All cars moved from stack 0 to stack 2\n";
    } else {
        cout<<"Move failed: Not enough space to empty stack 0\n";
    }
    cout<<"\nAfter Event 3:\n";
    parkinglot.display();
    cout<<"----------------------------------------\n\n";
    
    //////////////////////////////////////////////////////////////////
    cout<<"EVENT 4: Enqueue 30 more cars (cars 21-50)\n";
    added = 0;
    for(int i = 21; i < 51; i++) {
        int result = parkinglot.addToQueue(carIDs[i], models[i], driverNames[i]);
        if(result == 0) {
            added++;
        } else {
            cout<<"Queue full! Could not add car "<<carIDs[i]<<endl;
        }
    }
    cout<<"\nSuccessfully added "<<added<<" more cars\n";
    cout<<"\nAfter Event 4:\n";
    parkinglot.display();
    cout<<"----------------------------------------\n\n";
    
    //////////////////////////////////////////////////////////////////
    cout<<"EVENT 5: Pop car 20\n";
    Car* tempCar20 = new Car(20, "", "");
    int popResult = parkinglot.popCar(tempCar20);
    if(popResult == 0) {
        cout<<"Car 20 successfully popped\n";
    } else {
        cout<<"Car 20 not found (may not be at top of any stack)\n";
    }
    delete tempCar20;
    cout<<"\nAfter Event 5:\n";
    parkinglot.display();
    cout<<"----------------------------------------\n\n";
    
    //////////////////////////////////////////////////////////////////
    cout<<"EVENT 6: Find car 12\n";
    Car* tempCar12 = new Car(12, "", "");
    pair<int, int> findResult = parkinglot.find(tempCar12);
    if(findResult.first != -1) {
        cout<<"Car 12 found at stack "<<findResult.first 
            <<", position "<<findResult.second<<" (0 = top)\n";
    } else {
        cout<<"Car 12 not found\n";
    }
    delete tempCar12;
    cout<<"\nAfter Event 6:\n";
    parkinglot.display();
    cout<<"----------------------------------------\n\n";
    
    //////////////////////////////////////////////////////////////////
    cout<<"EVENT 7: Pop cars 31, 30, 29, 28, 19, 18, 50, 49, 48, 47, 2, 3, 4, 5, 40, 39\n";
    Car* tempCar31 = new Car(31, "", "");
    Car* tempCar30 = new Car(30, "", "");
    Car* tempCar29 = new Car(29, "", "");
    Car* tempCar28 = new Car(28, "", "");
    Car* tempCar19 = new Car(19, "", "");
    Car* tempCar18 = new Car(18, "", "");
    Car* tempCar50 = new Car(50, "", "");
    Car* tempCar49 = new Car(49, "", "");
    Car* tempCar48 = new Car(48, "", "");
    Car* tempCar47 = new Car(47, "", "");
    Car* tempCar2 = new Car(2, "", "");
    Car* tempCar3 = new Car(3, "", "");
    Car* tempCar4 = new Car(4, "", "");
    Car* tempCar5 = new Car(5, "", "");
    Car* tempCar40 = new Car(40, "", "");
    Car* tempCar39 = new Car(39, "", "");
    
    parkinglot.popCar(tempCar30);
    parkinglot.popCar(tempCar29);
    parkinglot.popCar(tempCar28);
    parkinglot.popCar(tempCar19);
    parkinglot.popCar(tempCar18);
    parkinglot.popCar(tempCar50);
    parkinglot.popCar(tempCar49);
    parkinglot.popCar(tempCar48);
    parkinglot.popCar(tempCar47);
    parkinglot.popCar(tempCar2);
    parkinglot.popCar(tempCar3);
    parkinglot.popCar(tempCar4);
    parkinglot.popCar(tempCar5);
    parkinglot.popCar(tempCar40);
    parkinglot.popCar(tempCar39);

    delete tempCar31;
    delete tempCar30;
    delete tempCar29;
    delete tempCar28;
    delete tempCar19;
    delete tempCar18;
    delete tempCar50;
    delete tempCar49;
    delete tempCar48;
    delete tempCar47;
    delete tempCar2;
    delete tempCar3;
    delete tempCar4;
    delete tempCar5;
    delete tempCar40;
    delete tempCar39;

    cout<<"\nAfter Event 7:\n";
    parkinglot.display();
    cout<<"----------------------------------------\n\n";
    
    //////////////////////////////////////////////////////////////////
    cout<<"EVENT 8: Insert new car at stack 5 (index 4)\n";
    Car* newCar = new Car(100, "Test Model", "Test Driver");
    int insertResult = parkinglot.insertAt(newCar, 4);
    if(insertResult == 0) {
        cout<<"Car 100 inserted at stack 5\n";
    } else {
        cout<<"Stack 5 is full, could not insert car 100\n";
        delete newCar;
    }
    cout<<"\nAfter Event 8:\n";
    parkinglot.display();
    cout<<"----------------------------------------\n\n";
    
    //////////////////////////////////////////////////////////////////
    cout<<"EVENT 9: Enqueue 30 more cars\n";
    added = 0;
    // Use cars 51-60 (10 cars)
    for(int i = 51; i < 61; i++) {
        int result = parkinglot.addToQueue(carIDs[i], models[i], driverNames[i]);
        if(result == 0) {
            added++;
        }
    }
    // Use cars 61-80 (20 cars)
    for(int i = 61; i < 81; i++) {
        int result = parkinglot.addToQueue(carIDs[i], models[i], driverNames[i]);
        if(result == 0) {
            added++;
        }
    }
    cout<<"\nSuccessfully added "<<added<<" more cars\n";
    cout<<"\nAfter Event 9:\n";
    parkinglot.display();
    cout<<"----------------------------------------\n\n";
    //////////////////////////////////////////////////////////////////
    cout<<"EVENT 10: sort stack 2\n";
    parkinglot.sort(2);
    cout<<"\nAfter Event 10 (FINAL STATE):\n";
    parkinglot.display();
    
    
    cout<<"----------------------------------------\n\n";
    cout<<"ALL EVENTS COMPLETED\n";
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
}