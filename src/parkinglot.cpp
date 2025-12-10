#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

class Car { // Classifying car as Node in Stack and Queue
public:
    int id;
    string model;
    Car* next;

    // Constructors **Time Complexity: O(1)**
    Car(){
        id=-1;
        model="";
        next=nullptr;
    }
    Car(int idin, string mod){
        id=idin;
        model=mod;
        next=nullptr;
    }
};

class MyStack { //Implementing Stack
public:
    int size;
    int capacity;
    Car* top=nullptr;
    
    // Constructor **Time Complexity: O(1)**
    MyStack() : size(0), capacity(0), top(nullptr) {}

    void merge_sort(vector<Car> &a, int l, int r){ //Implementation of merge sort on linkedlist (stack) **Time Complexity: O(nlogn)**
        if(l>=r) return;
        int m=l+(r-l)/2;
        merge_sort(a, l, m);
        merge_sort(a, m+1, r);

        vector<Car> tmp;
        int i=l, j=m+1;
        while(i<=m && j<=r)
            tmp.push_back(a[i].id<=a[j].id?a[i++]:a[j++]);
        while(i<=m) tmp.push_back(a[i++]);
        while(j<=r) tmp.push_back(a[j++]);
        
        for(int k=0; k<tmp.size(); ++k)
            a[l+k]=tmp[k];
        
            return;
    }
    
    bool isEmpty(){ // **Time Complexity: O(1)**
        return (size==0);
    }

    bool isFull(){ // **Time Complexity: O(1)**
        return (size==capacity);
    }

    bool push(int id, string model){ //Makes a Car object and pushes it into Stack, returns 0 for success and 1 for fail when stack is full **Time Complexity: O(1)**
        if(isFull()) return 1;
        Car*newCar = new Car(id, model);
        newCar->next = top;
        top = newCar;
        size++;
        return 0;
    }
    
    Car pop(){ //pops and returns the popped element if not empty **Time Complexity: O(1)**
        if(isEmpty()) return Car(-1, "CNF");
        Car* tmp=top;
        Car answer = *tmp;
        top=tmp->next;
        delete tmp;
        size--;
        
        return answer;
    }

    void sort(){ //calls merge_sort appropriately **Time Complexity: O(nlogn)**
        if(isEmpty()) return;
        vector<Car> v;
        while(size) v.push_back(pop());
        merge_sort(v, 0, v.size()-1);
        while(v.size()){
            push(v.back().id, v.back().model);
            v.pop_back();
        }
    }

    string print(){//prints the stack for debugging purposes **Time Complexity: O(n)**
        Car* iter=top;
        string res;
        stringstream ss;
        ss<<"";
        while(iter!=nullptr){
            ss<<setw(2)<<(*iter).id<<" - ";
            iter=(*iter).next;
        }
        res=ss.str();
        return res;
    }

    ~MyStack() { //Deconstructor, deletes every node **Time Complexity: O(n)**
        while (top != nullptr) {
            Car* temp = top;
            top = top->next;
            delete temp;
        }
    }
};

class MyQueue { // Implementing Queue
public:
    int size;
    int capacity;
    Car* front; 
    Car* rear;

    // Constructor **Time Complexity: O(1)**
    MyQueue() : size(0), capacity(0), front(nullptr), rear(nullptr) {}

    bool isEmpty(){ // **Time Complexity: O(1)**
        return (size==0);
    }

    bool isFull(){ // **Time Complexity: O(1)**
        return (size==capacity);
    }

    bool enqueue(int id, string model){ // Makes and enqueues a new Car, returns 0 for success and 1 for fail when queue is full **Time Complexity: O(1)**
        if(isFull()) return 1;

        Car* newCar = new Car(id, model);
        newCar->next = nullptr;

        if (front == nullptr) front = newCar;
        else rear->next = newCar;
        rear = newCar;

        size++;
        return 0;
    }

    Car dequeue(){ // Dequeues and returns the oldest element **Time Complexity: O(1)**
        if (isEmpty()) return Car(-1, "CNF");

        Car* temp = front;
        Car answer = *temp;

        front = front->next;
        if (front == nullptr) rear = nullptr; // Queue is empty

        delete temp;
        size--;
        return answer;
    }

    string print(){//prints queue the queue for debugging purposes **Time Complexity: O(n)**
        Car* iter = front;
        stringstream ss;
        ss << "";

        while (iter != nullptr) {
            ss << setw(2) << iter->id << " : ";
            iter = iter->next;
        }

        return ss.str();
    }

    ~MyQueue(){ //Deconstructor, deletes every node **Time Complexity: O(n)**
        while (front != nullptr) {
            Car* temp = front;
            front = front->next;
            delete temp;
        }
        rear = nullptr;
    }
};


class Parkinglot { //Main class with implementation of functions requested in the project
private:
public:
    MyQueue Q; //queue
    vector<MyStack> parkings;
    
    void display(){ // Displays the Queue and Parking for ease at testing in the terminal **Time Complexity O(n)**
        cout<<"Cars in queue:\n";
        cout<<Q.print();
        cout<<"\nCars in parkinglot:\n";
        for(int i = 0; i < parkings.size(); i++){
            cout<<parkings[i].print();
            cout<<"\n";
        }
    }

    Parkinglot(int queueCap, int stackNum, int stackCap){//Constructor **Time Complexity: O(n)**
        Q.capacity=queueCap;
        for(int i=0; i<stackNum; i++){
            MyStack stack;
            stack.capacity=stackCap;
            parkings.push_back(stack);
        }
    }

    int initialize(){ //Inserts every element from queue into the parkinglot if possible, called after every addition to queue and every pop.
        // returns 0 if it successfully empties the queue and 1 if the parkinglot gets full before the queue is empty. **Time Complexity: O(n)**
        while(!Q.isEmpty()){
            Car car = *(Q.front);
            if(insert(car.id, car.model)) return 1;
            Q.dequeue();
        }
        if(Q.isEmpty()) return 0;
        return 1;
    }

    pair<int, int> find(int carId){ //Finds the requested carId, returns {-1, -1} if car doesn't exist in parkinglot and {stack number, position}(both zero based) if it is found **Time Complexity O(n)**
        pair<int, int> result; result.first=-1; result.second=-1;
        Car* pcar;
        for(int i=0; i<parkings.size(); i++){
            if(parkings[i].isEmpty()) continue; 
            pcar = parkings[i].top;
            for(int j=0; j<parkings[i].size; j++){
                if(pcar->id==carId){
                    result.first=i;
                    result.second=j;
                    return result;
                }
                pcar=pcar->next;
            }
        }
        return result;
    }

    bool isInQueue(int carId){  //Checks if the car is already in queue **Time Complexity O(n)**
        if(Q.isEmpty()) return 0;
        Car* car=Q.front;
        for(int i=0; i<Q.capacity; i++){
            if(car==nullptr) return 0;
            if(car->id==carId) return 1;
            car=car->next;
        }
        return 0;
    }

    int addToQueue(int carId, string model){ //Adds a new car to queue based on the car's information
        // returns 0 when the car is successfully added, 1 when queue is full and 2 when the car id already exists in parkinglot or queue
        // **Time Complexity O(n)**
        if(find(carId).first!=-1 || isInQueue(carId)) return 2;
        int result = Q.enqueue(carId, model);
        initialize();
        return result;
    }
    int insert(int id, string model){ //Inserts the car at the first available spot in the parkinglot. returns 0 if successful and 1 if the parkinglot is full **Time Complexity O(n)**
        for(MyStack &parking : parkings){
            if(parking.push(id, model) == 0) return 0;
        }
        return 1;
    }
    bool insertAt(int carId, string model, int i){ // Inserts the car at a specific stack if it's not full, returns 0 if successful and 1 if the stack is full. **Time Complexity O(1)**
        return parkings[i].push(carId, model);
    }
    void sort(int i){ // Sorts the i-th parking(stack) using the function in MyStack. **Time Complexity O(nlogn)**
        parkings[i].sort();
    }
    int move(int i, int j){// moves as many cars as possible from the i-th parking to the j-th parking, if the j-th parkings is full, it will continue into the j+1-th and j+2-th and so on
    // upon reaching the end of stacks, it will loop back and try to insert the cars into parkings 0 to i-1. and after that, from i+1 to j-1. (it jumps over the i-th stack)
    // it will return 0 if the move is successful and all cars in i have been moved and returns 1 if there's not enough space in parkinglot for emptying parking i.
    // **Time Complexity O(n)**
        int cnt=0;
        int cap=parkings[0].capacity;
        while(!parkings[i].isEmpty() && cnt<(parkings.size()*cap+1)){
            if(parkings[j].isFull()) j++;
            if(j==i) j++;
            if(j==parkings.size()) j=0;
            Car poped = parkings[i].pop();
            parkings[j].push(poped.id, poped.model);
            cnt++;
        }
        if(parkings[i].isEmpty()) return 0;
        return 1;
    }
    int popCar(int carId){ // Pops the given car if it's the top of one of the parkings, returns 0 if successful, 1 if car not is not at top and 2 if car not in parking
        // **Time Complexity O(n)**
        pair<int, int> pos = find(carId);
        if(pos.first==-1) return 2;
        if(pos.second!=0) return 1;
        parkings[pos.first].pop();
        initialize();
        return 0;
    }

};

// temporary main function for testing purposes without gui
/*
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

    int queueCapacity = 30, stackNumber = 6, stackCapacity = 10;
    Parkinglot parkinglot(queueCapacity, stackNumber, stackCapacity);
    
    cout<<"=== INITIAL STATE ===\n";
    cout<<"Queue capacity: "<<queueCapacity<<endl;
    cout<<"Number of stacks: "<<stackNumber<<endl;
    cout<<"Stack capacity: "<<stackCapacity<<endl;
    cout<<"Total parking capacity: "<<stackNumber * stackCapacity<<endl;
    
    // parkinglot.display();
    cout<<"----------------------------------------\n\n";
    
    //////////////////////////////////////////////////////////////////
    cout<<"EVENT 1: Enqueue 21 cars\n";
    int added = 0;
    for(int i = 0; i < 21; i++) {
        int result = parkinglot.addToQueue(carIDs[i], models[i]);
        if(result == 0) {
            added++;
        } else {
            // cout<<"Queue full! Could not add car "<<carIDs[i]<<endl;
        }
    }
    // cout<<"\nSuccessfully added "<<added<<" cars to queue\n";
    // cout<<"\nAfter Event 1:\n";
    // parkinglot.display();
    // cout<<"----------------------------------------\n\n";

    ///////////////////////////////////////////////////////////////////
    cout<<"EVENT 2: Sort stack number 0\n";
    parkinglot.sort(0);
    // cout<<"\nSuccessfully sorted stack 0\n";
    // cout<<"\nAfter Event 2:\n";
    // parkinglot.display();
    // cout<<"----------------------------------------\n\n";
    
    
    //////////////////////////////////////////////////////////////////
    cout<<"EVENT 3: Move all cars from stack 0 to stack 2\n";
    int moveResult = parkinglot.move(0, 2);
    if(moveResult == 0) {
        // cout<<"Move successful: All cars moved from stack 0 to stack 2\n";
    } else {
        // cout<<"Move failed: Not enough space to empty stack 0\n";
    }
    // cout<<"\nAfter Event 3:\n";
    // parkinglot.display();
    // cout<<"----------------------------------------\n\n";
    
    //////////////////////////////////////////////////////////////////
    cout<<"EVENT 4: Enqueue 30 more cars (cars 21-50)\n";
    added = 0;
    for(int i = 21; i < 51; i++) {
        int result = parkinglot.addToQueue(carIDs[i], models[i]);
        if(result == 0) {
            added++;
        } else {
            // cout<<"Queue full! Could not add car "<<carIDs[i]<<endl;
        }
    }
    // cout<<"\nSuccessfully added "<<added<<" more cars\n";
    // cout<<"\nAfter Event 4:\n";
    // parkinglot.display();
    // cout<<"----------------------------------------\n\n";
    
    //////////////////////////////////////////////////////////////////
    cout<<"EVENT 5: Pop car 20\n";
    int popResult = parkinglot.popCar(20);
    if(popResult == 0) {
        // cout<<"Car 20 successfully popped\n";
    } else {
        // cout<<"Car 20 not found (may not be at top of any stack)\n";
    }
    // cout<<"\nAfter Event 5:\n";
    // parkinglot.display();
    // cout<<"----------------------------------------\n\n";
    
    //////////////////////////////////////////////////////////////////
    cout<<"EVENT 6: Find car 12\n";
    pair<int, int> findResult = parkinglot.find(12);
    if(findResult.first != -1) {
        // cout<<"Car 12 found at stack "<<findResult.first 
            // <<", position "<<findResult.second<<" (0 = top)\n";
    } else {
        // cout<<"Car 12 not found\n";
    }
    // cout<<"\nAfter Event 6:\n";
    // parkinglot.display();
    // cout<<"----------------------------------------\n\n";
    
    //////////////////////////////////////////////////////////////////
    cout<<"EVENT 7: Pop cars 31, 30, 29, 28, 19, 18, 50, 49, 48, 47, 2, 3, 4, 5, 40, 39\n";
    parkinglot.popCar(31);
    parkinglot.popCar(30);
    parkinglot.popCar(29);
    parkinglot.popCar(28);
    parkinglot.popCar(19);
    parkinglot.popCar(18);
    parkinglot.popCar(50);
    parkinglot.popCar(49);
    parkinglot.popCar(48);
    parkinglot.popCar(47);
    parkinglot.popCar(2);
    parkinglot.popCar(3);
    parkinglot.popCar(4);
    parkinglot.popCar(5);
    parkinglot.popCar(40);
    parkinglot.popCar(39);

    // cout<<"\nAfter Event 7:\n";
    // parkinglot.display();
    // cout<<"----------------------------------------\n\n";
    
    //////////////////////////////////////////////////////////////////
    cout<<"EVENT 8: Insert new car at stack 5 (index 4)\n";
    int insertResult = parkinglot.insertAt(100, "Test Model", 4);
    if(insertResult == 0) {
        cout<<"Car 100 inserted at stack 5\n";
    } else {
        cout<<"Stack 5 is full, could not insert car 100\n";
    }

    cout<<"\nAfter Event 8:\n";
    parkinglot.display();
    cout<<"----------------------------------------\n\n";

    cout<<"EVENT 9: Move from 0 to 2\n";
    parkinglot.move(0, 2);
    parkinglot.display();
    cout<<"----------------------------------------\n\n";
    
    // //////////////////////////////////////////////////////////////////
    cout<<"EVENT 9: Enqueue 30 more cars\n";
    // Use cars 51-60 (10 cars)
    for(int i = 51; i < 73; i++) {
        int result = parkinglot.addToQueue(carIDs[i], models[i]);
    }
    parkinglot.display();
    cout<<"a buncha cars added\n";
    // Use cars 61-80 (20 cars)
    for(int i = 73; i < 85; i++) {
        int result = parkinglot.addToQueue(carIDs[i], models[i]);
    }
    cout<<"more than enough cars added\n";
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

    cout<<"EVENT 11: pop again\n";
    parkinglot.popCar(26);
    parkinglot.display();
    cout<<"ALL EVENTS COMPLETED\n";
}
*/