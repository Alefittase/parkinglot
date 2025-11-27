#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Int_Input.H>
#include <cstdlib>
#include <cstdio>

using namespace std;

// Function implementations for testing
void enter_parking(const char* license_plate, const char* model, const char* type) {
    cout << "Enter Parking: " << license_plate << " " << model << " " << type << "\n";
}

void sort_parking(int stack_number) {
    cout << "Sort Parking: " << stack_number << "\n";
}

void move_parking(int origin_stack, int destination_stack) {
    cout << "Move Parking: " << origin_stack << " " << destination_stack << "\n";
}

void exit_parking(const char* license_plate) {
    cout << "Exit Parking: " << license_plate << "\n";
}

// Global pointers to input fields
// Enter Parking fields
Fl_Input *license_plate_input;
Fl_Input *model_input;
Fl_Input *type_input;

// Sort field
Fl_Int_Input *sort_stack_input;

// Move fields
Fl_Int_Input *origin_stack_input;
Fl_Int_Input *destination_stack_input;

// Exit field
Fl_Input *exit_license_input;

void enter_parking_cb(Fl_Widget* w, void*)
{
    const char* license_plate = license_plate_input->value();
    const char* model = model_input->value();
    const char* type = type_input->value();
    
    // Basic validation - check if license plate is not empty
    if (license_plate && strlen(license_plate) > 0) {
        enter_parking(license_plate, model, type);
        
        // Clear inputs after submission
        license_plate_input->value("");
        model_input->value("");
        type_input->value("");
    }
}

void sort_parking_cb(Fl_Widget* w, void*)
{
    const char* stack_str = sort_stack_input->value();
    if (stack_str && strlen(stack_str) > 0) {
        int stack_number = atoi(stack_str);
        sort_parking(stack_number);
        sort_stack_input->value("");
    }
}

void move_parking_cb(Fl_Widget* w, void*)
{
    const char* origin_str = origin_stack_input->value();
    const char* dest_str = destination_stack_input->value();
    
    if (origin_str && strlen(origin_str) > 0 && dest_str && strlen(dest_str) > 0) {
        int origin = atoi(origin_str);
        int destination = atoi(dest_str);
        move_parking(origin, destination);
        origin_stack_input->value("");
        destination_stack_input->value("");
    }
}

void exit_parking_cb(Fl_Widget* w, void*)
{
    const char* license_plate = exit_license_input->value();
    if (license_plate && strlen(license_plate) > 0) {
        exit_parking(license_plate);
        exit_license_input->value("");
    }
}

void quit_cb(Fl_Widget* w, void*) {
    exit(0);
}

int main()
{
    Fl_Window *window = new Fl_Window(600, 650, "Parking Management System");
    
    // Title
    Fl_Box *title = new Fl_Box(200, 20, 200, 30, "Parking Management System");
    title->labelfont(FL_BOLD);
    title->labelsize(18);
    
    int y_position = 70;
    int label_width = 120;
    int input_width = 200;
    int button_width = 100;
    int spacing = 50;
    
    // Enter Parking Section
    Fl_Box *enter_label = new Fl_Box(50, y_position, 200, 25, "Enter Car Information:");
    enter_label->labelfont(FL_BOLD);
    y_position += 30;
    
    // License Plate
    Fl_Box *license_label = new Fl_Box(50, y_position, label_width, 25, "License Plate:");
    license_plate_input = new Fl_Input(50 + label_width, y_position, input_width, 25);
    y_position += 35;
    
    // Model
    Fl_Box *model_label = new Fl_Box(50, y_position, label_width, 25, "Model:");
    model_input = new Fl_Input(50 + label_width, y_position, input_width, 25);
    y_position += 35;
    
    // Type
    Fl_Box *type_label = new Fl_Box(50, y_position, label_width, 25, "Type:");
    type_input = new Fl_Input(50 + label_width, y_position, input_width, 25);
    y_position += 40;
    
    Fl_Button *enter_btn = new Fl_Button(50 + label_width + input_width + 20, y_position - 80, button_width, 30, "Enter Parking");
    enter_btn->callback(enter_parking_cb);
    
    // Sort Section
    Fl_Box *sort_label = new Fl_Box(50, y_position, 200, 25, "Sort Parking Stack:");
    sort_label->labelfont(FL_BOLD);
    y_position += 30;
    
    Fl_Box *sort_stack_label = new Fl_Box(50, y_position, label_width, 25, "Stack Number:");
    sort_stack_input = new Fl_Int_Input(50 + label_width, y_position, input_width, 25);
    y_position += 40;
    
    Fl_Button *sort_btn = new Fl_Button(50 + label_width + input_width + 20, y_position - 40, button_width, 30, "Sort");
    sort_btn->callback(sort_parking_cb);
    
    // Move Section
    Fl_Box *move_label = new Fl_Box(50, y_position, 200, 25, "Move Between Stacks:");
    move_label->labelfont(FL_BOLD);
    y_position += 30;
    
    // Origin Stack
    Fl_Box *origin_label = new Fl_Box(50, y_position, label_width, 25, "Origin Stack:");
    origin_stack_input = new Fl_Int_Input(50 + label_width, y_position, input_width, 25);
    y_position += 35;
    
    // Destination Stack
    Fl_Box *destination_label = new Fl_Box(50, y_position, label_width, 25, "Destination Stack:");
    destination_stack_input = new Fl_Int_Input(50 + label_width, y_position, input_width, 25);
    y_position += 40;
    
    Fl_Button *move_btn = new Fl_Button(50 + label_width + input_width + 20, y_position - 40, button_width, 30, "Move");
    move_btn->callback(move_parking_cb);
    
    // Exit Parking Section
    Fl_Box *exit_label = new Fl_Box(50, y_position, 200, 25, "Exit Parking:");
    exit_label->labelfont(FL_BOLD);
    y_position += 30;
    
    Fl_Box *exit_license_label = new Fl_Box(50, y_position, label_width, 25, "License Plate:");
    exit_license_input = new Fl_Input(50 + label_width, y_position, input_width, 25);
    y_position += 40;
    
    Fl_Button *exit_btn = new Fl_Button(50 + label_width + input_width + 20, y_position - 40, button_width, 30, "Exit Parking");
    exit_btn->callback(exit_parking_cb);
    
    // Quit Button at the bottom
    Fl_Button *quit_btn = new Fl_Button(250, y_position + 20, 100, 30, "Quit");
    quit_btn->callback(quit_cb);
    
    window->end();
    window->show();
    
    return Fl::run();
}