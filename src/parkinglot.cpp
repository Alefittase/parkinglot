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
    

    int push(Car* newest){ //returns 1 if push is successful, returns 0 if stack is full
        if(size==capacity) return 1;
        if(size>0) newest->setNext(top);
        top = newest;
        size++;
        return 0;
    }
    
    Car* pop(){
        if(size==0) return nullptr;
        Car* returnee=top;
        if(size>1) top=top->getNext();
        size--;
        return returnee;
    }

    void mergeSort(){}
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
    Car* getFront() const {return front;}
    Car* getRear() const {return rear;}

    // Setters
    void setSize(int newSize) {size=newSize;}
    void setFront(Car* newFront) {front=newFront;}
    

    int enqueue(Car* car){
        if(size==capacity) return 1;
        if(size>0) rear->setNext(newest);
        if(size==0) front=newest;
        rear = newest;
        size++;
        return 0;
    }
    
    Car* dequeue(){
        if(size==0) return nullptr;
        Car* returnee=front;
        if(size>1) front=front->getNext();
        size--;
        return returnee;
    }

    void printQueue(){
        Car* car = front;
        for(int i=0; i<size; i++){
        cout<<car->getElement()<<" ";
        car=car->getNext();
        }
        cout<<"\n";
    }
};

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

class Parkinglot {
private:
    MyQueue carQ;
    vector<MyStack> parkings;
public:
    ParkingLot();
    int addToQueue(string carId, string model, string driverName){
        Car* car(string carId, string model, string driverName);
        if(carQ.enqueue(car)) return 1;
        return 0;
    }
    int insert(Car* car){ //returns 0 if insert is successful, returns 0 if parking is full.
        for(MyStack parking : parkings){
            if(!parking.push(car)) return 0;
        }
        return 1;
    }
    int insertAt(Car* car, int i){
        return parkings[i].push(car);
    }
    void sort(int i);
    void move(int i, int j){
        int cnt=0;
        while(parkings[i].getTop()!=nullptr && cnt<parkings.size()){
            if(parkings[j].enqueue(parkings[i].dequeue())) j++;
            if(j==i) j++;
            if(j==parkings.size()) j=0;
            cnt++;
        }
    }
    void popCar(Car* car){
        for(MyStack parking : parkings){
            if(parking.getTop()==car){
                parking.pop();
                return 0;
            }
        }
        return 1;
    }
};