#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/fl_draw.H>
#include <iostream>
#include <sstream>

#include "parkinglot.cpp"   // you explicitly requested including the cpp

// ----------------------- Global Parkinglot -----------------------
Parkinglot parking(10, 3, 5);

// ----------------------- Widgets -----------------------
Fl_Int_Input *queue_cap_input;
Fl_Int_Input *stack_num_input;
Fl_Int_Input *stack_cap_input;

Fl_Input *license_plate_input;
Fl_Input *model_input;
Fl_Input *type_input;

Fl_Int_Input *sort_stack_input;
Fl_Int_Input *origin_stack_input;
Fl_Int_Input *destination_stack_input;
Fl_Input *find_license_input = 0;
Fl_Input *exit_license_input;
Fl_Multiline_Output *parking_display;


// --------------------------------------------------------------
//                       Helper Functions
// --------------------------------------------------------------
void update_display() {
    std::stringstream ss;

    // Queue
    ss << "Queue: ";
    Car* q = parking.carQ.getFront();
    for (int i = 0; i < parking.carQ.getSize(); i++) {
        ss << q->getCarId() << " ";
        q = q->getNext();
    }
    ss << "\n\n";

    // Stacks
    for (int i = 0; i < parking.parkings.size(); i++) {
        ss << "Parking " << i << ": ";
        for(int j=0; j<parking.parkings[i].getCapacity()-parking.parkings[i].getSize(); j++){
            ss << "_ ";
        }
        Car* top = parking.parkings[i].getTop();
        while (top) {
            ss << top->getCarId() << " ";
            top = top->getNext();
        }
        ss << "\n";
    }

    parking_display->value(ss.str().c_str());
}


// --------------------------------------------------------------
//                          Callbacks
// --------------------------------------------------------------
void create_parkinglot_cb(Fl_Widget*, void*) {
    int q = atoi(queue_cap_input->value());
    int n = atoi(stack_num_input->value());
    int c = atoi(stack_cap_input->value());
    if (q <= 0 || n <= 0 || c <= 0) return;

    parking = Parkinglot(q, n, c);

    queue_cap_input->value("");
    stack_num_input->value("");
    stack_cap_input->value("");

    update_display();
}

void enter_parking_cb(Fl_Widget*, void*) {
    int id = atoi(license_plate_input->value());
    std::string model = model_input->value();
    std::string type = type_input->value();

    parking.addToQueue(id, model, type);

    license_plate_input->value("");
    model_input->value("");
    type_input->value("");

    update_display();
}

void sort_parking_cb(Fl_Widget*, void*) {
    int st = atoi(sort_stack_input->value());
    if (st < 0 || st >= parking.parkings.size()) return;

    parking.sort(st);
    sort_stack_input->value("");
    update_display();
}

void move_parking_cb(Fl_Widget*, void*) {
    int o = atoi(origin_stack_input->value());
    int d = atoi(destination_stack_input->value());

    parking.move(o, d);

    origin_stack_input->value("");
    destination_stack_input->value("");
    update_display();
}

void find_parking_cb(Fl_Widget*, void*) {

    /*

    */

}

void exit_parking_cb(Fl_Widget*, void*) {
    int id = atoi(exit_license_input->value());
    Car tmp(id, "", "");
    parking.popCar(&tmp);

    exit_license_input->value("");
    update_display();
}

void quit_cb(Fl_Widget*, void*) {
    exit(0);
}


// --------------------------------------------------------------
//                       Themed Button Helper
// --------------------------------------------------------------
Fl_Button* themed_button(int x, int y, int w, int h, const char* label) {
    Fl_Button* b = new Fl_Button(x, y, w, h, label);
    b->color(fl_rgb_color(40, 120, 200));
    b->labelcolor(FL_WHITE);
    b->labelfont(FL_HELVETICA_BOLD);
    b->box(FL_ROUNDED_BOX);
    return b;
}

static int centered_btn_y(int y){
    return y + (28 - 28) / 2;
}


// --------------------------------------------------------------
//                            MAIN
// --------------------------------------------------------------
int main() {
    Fl_Window *window = new Fl_Window(1000, 950, "Parking Management System");
    Fl::background(45,45,48);      // dark background
    Fl::foreground(220,220,220);   // light text
    Fl::set_color(FL_BACKGROUND2_COLOR, 60,60,63);
    // window->color(fl_rgb_color(245, 245, 245)); // light grey background

    const int ROW_H = 42;
    const int LABEL_W = 140;
    const int INPUT_W = 140;
    const int BTN_W = 100;
    const int START_X = 40;

    int y = 20;


    // ------------------------------------------------------
    //                 CREATE PARKING LOT
    // ------------------------------------------------------
    Fl_Box *title = new Fl_Box(START_X, y, 420, 30, "Create Parking Lot");
    title->labelfont(FL_BOLD);
    title->labelsize(20);
    y += ROW_H;

    // Queue capacity
    new Fl_Box(START_X, y, LABEL_W, 28, "Queue Capacity:");
    queue_cap_input = new Fl_Int_Input(START_X + LABEL_W + 8, y, INPUT_W, 28);

    // Stack number
    y += ROW_H;
    new Fl_Box(START_X, y, LABEL_W, 28, "Number of Stacks:");
    stack_num_input = new Fl_Int_Input(START_X + LABEL_W + 8, y, INPUT_W, 28);
    
    // Create button
    int btn_y = centered_btn_y(y);
    Fl_Button *create_btn = themed_button(
        START_X + LABEL_W + 8 + INPUT_W + 25,
        btn_y,
        BTN_W,
        28,
        "Create"
    );
    create_btn->callback(create_parkinglot_cb);
    
    // Stack capacity
    y += ROW_H;
    new Fl_Box(START_X, y, LABEL_W, 28, "Stack Capacity:");
    stack_cap_input = new Fl_Int_Input(START_X + LABEL_W + 8, y, INPUT_W, 28);
    
    y += ROW_H;
    
    // Separator line
    y += ROW_H + 10;
    Fl_Box *sep = new Fl_Box(START_X, y, 420, 2, "");
    sep->box(FL_FLAT_BOX);
    sep->color(fl_rgb_color(200, 200, 200));
    y += 30;


    // ------------------------------------------------------
    //                    ENTER CAR
    // ------------------------------------------------------
    Fl_Box *enter_title = new Fl_Box(START_X, y, 420, 28, "Enter Car");
    enter_title->labelfont(FL_BOLD);
    y += ROW_H;

    new Fl_Box(START_X, y, LABEL_W, 28, "License Plate:");
    license_plate_input = new Fl_Input(START_X + LABEL_W + 8, y, INPUT_W, 28);

    new Fl_Box(START_X, y + ROW_H, LABEL_W, 28, "Model:");
    model_input = new Fl_Input(START_X + LABEL_W + 8, y + ROW_H, INPUT_W, 28);

    new Fl_Box(START_X, y + 2*ROW_H, LABEL_W, 28, "Type:");
    type_input = new Fl_Input(START_X + LABEL_W + 8, y + 2*ROW_H, INPUT_W, 28);

    y+=ROW_H;
    Fl_Button *enter_btn = themed_button(
        START_X + LABEL_W + 8 + INPUT_W + 25,
        centered_btn_y(y),
        BTN_W,
        28,
        "Enter"
    );
    enter_btn->callback(enter_parking_cb);
    y += 2*ROW_H + 10;


    // ------------------------------------------------------
    //                 SORT STACK
    // ------------------------------------------------------
    Fl_Box *sort_title = new Fl_Box(START_X, y, 420, 28, "Sort Stack");
    sort_title->labelfont(FL_BOLD);
    y += ROW_H;

    new Fl_Box(START_X, y, LABEL_W, 28, "Stack Number:");
    sort_stack_input = new Fl_Int_Input(START_X + LABEL_W + 8, y, INPUT_W, 28);

    Fl_Button *sort_btn = themed_button(
        START_X + LABEL_W + 8 + INPUT_W + 25,
        centered_btn_y(y),
        BTN_W,
        28,
        "Sort"
    );
    sort_btn->callback(sort_parking_cb);
    y += ROW_H + 20;


    // ------------------------------------------------------
    //                  MOVE STACKS
    // ------------------------------------------------------
    Fl_Box *move_title = new Fl_Box(START_X, y, 420, 28, "Move Cars Between Stacks");
    move_title->labelfont(FL_BOLD);
    y += ROW_H;

    new Fl_Box(START_X, y, LABEL_W, 28, "Origin Stack:");
    origin_stack_input = new Fl_Int_Input(START_X + LABEL_W + 8, y, INPUT_W, 28);

    new Fl_Box(START_X, y + ROW_H, LABEL_W, 28, "Destination Stack:");
    destination_stack_input = new Fl_Int_Input(START_X + LABEL_W + 8, y + ROW_H, INPUT_W, 28);
    y+=ROW_H/2;
    Fl_Button *move_btn = themed_button(
        START_X + LABEL_W + 8 + INPUT_W + 25,
        centered_btn_y(y),
        BTN_W,
        28,
        "Move"
    );
    move_btn->callback(move_parking_cb);
    y += 3*ROW_H/2 + 20;


    // ---------- Find Parking Row ----------
    Fl_Box *find_title = new Fl_Box(START_X, y, 420, 28, "Find Car");
    find_title->labelfont(FL_BOLD);
    
    y += ROW_H;

    new Fl_Box(START_X, y, LABEL_W, 28, "License Plate:");
    find_license_input = new Fl_Input(START_X + LABEL_W + 8, y, INPUT_W, 28);

    find_license_input->align(FL_ALIGN_LEFT);

    Fl_Button *find_btn = themed_button(
        START_X + LABEL_W + 8 + INPUT_W + 25,
        centered_btn_y(y),
        BTN_W,
        28,
        "Find"
    );
    find_btn->callback(find_parking_cb);
    y += ROW_H;


    // ------------------------------------------------------
    //                   EXIT CAR
    // ------------------------------------------------------
    Fl_Box *exit_title = new Fl_Box(START_X, y, 420, 28, "Exit Car");
    exit_title->labelfont(FL_BOLD);
    y += ROW_H;

    new Fl_Box(START_X, y, LABEL_W, 28, "License Plate:");
    exit_license_input = new Fl_Input(START_X + LABEL_W + 8, y, INPUT_W, 28);

    Fl_Button *exit_btn = themed_button(
        START_X + LABEL_W + 8 + INPUT_W + 25,
        centered_btn_y(y),
        BTN_W,
        28,
        "Exit"
    );
    exit_btn->callback(exit_parking_cb);
    y += ROW_H + 20;


    // ------------------------------------------------------
    //                     QUIT
    // ------------------------------------------------------
    Fl_Button *quit_btn = themed_button(START_X, y, 420, 30, "Quit");
    quit_btn->callback(quit_cb);


    // ------------------------------------------------------
    //                PARKINGLOT DISPLAY
    // ------------------------------------------------------
    parking_display = new Fl_Multiline_Output(520, 20, 450, 720);
    parking_display->textsize(15);
    parking_display->color(fl_rgb_color(40, 120, 200));
    parking_display->box(FL_DOWN_BOX);


    window->end();
    window->show();

    update_display();
    return Fl::run();
}


/*
addToQueue() : 0 Success, 1 Queue Full, 2 Duplicate Car
insert() : 0 Success, 1 Parkinglot Full
insertAt() : 0 Success, 1 Stack Full
move() : 0 Success, 1 Not enough space in parking 
popCar() : 0 Success, 1 Car not at top, 2 Car not found 
find() : {-1, -1} Car not found, else {Stack, Position}
*/