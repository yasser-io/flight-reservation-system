#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_FLIGHTS 100
#define MAX_PASSENGERS 500
#define MAX_SEATS 180
#define FILENAME "flight_data.dat"

typedef struct {
    int flight_id;
    char flight_number[10];
    char airline[30];
    char departure_city[30];
    char arrival_city[30];
    char departure_time[6];
    char arrival_time[6];
    char departure_date[11];
    int total_seats;
    int available_seats;
    float economy_price;
    float business_price;
    int is_active;
} Flight;

typedef struct {
    int booking_id;
    int flight_id;
    char passenger_name[50];
    char passenger_email[50];
    char passenger_phone[15];
    int seat_number;
    char seat_class[20];
    char booking_date[11];
    float ticket_price;
    char status[20];
    char special_requests[100];
} Booking;

Flight flights[MAX_FLIGHTS];
Booking bookings[MAX_PASSENGERS];
int flight_count = 0;
int booking_count = 0;
int next_flight_id = 1001;
int next_booking_id = 5001;

void display_menu();
void add_flight();
void display_flights();
void search_flights();
void book_ticket();
void cancel_booking();
void view_bookings();
void check_in();
void generate_reports();
void manage_flights();
void save_to_file();
void load_from_file();
void get_current_date(char *date);

int main() {
    load_from_file();
    int choice;
    
    printf("✈️  Flight Reservation System\n");
    printf("============================\n");
    
    do {
        display_menu();
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: add_flight(); break;
            case 2: display_flights(); break;
            case 3: search_flights(); break;
            case 4: book_ticket(); break;
            case 5: cancel_booking(); break;
            case 6: view_bookings(); break;
            case 7: check_in(); break;
            case 8: generate_reports(); break;
            case 9: manage_flights(); break;
            case 10: save_to_file(); break;
            case 0: printf("Thank you for using the system!\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 0);
    
    return 0;
}

void display_menu() {
    printf("\n🏠 Main Menu:\n");
    printf("1. Add New Flight\n");
    printf("2. Display All Flights\n");
    printf("3. Search Flights\n");
    printf("4. Book Ticket\n");
    printf("5. Cancel Booking\n");
    printf("6. View Bookings\n");
    printf("7. Check-in Passenger\n");
    printf("8. Generate Reports\n");
    printf("9. Manage Flights\n");
    printf("10. Save Data\n");
    printf("0. Exit\n");
}

void add_flight() {
    if(flight_count >= MAX_FLIGHTS) {
        printf("Cannot add more flights!\n");
        return;
    }
    
    Flight new_flight;
    new_flight.flight_id = next_flight_id++;
    
    printf("\nAdd New Flight:\n");
    printf("Flight Number: ");
    scanf("%s", new_flight.flight_number);
    printf("Airline: ");
    scanf(" %[^\n]", new_flight.airline);
    printf("Departure City: ");
    scanf(" %[^\n]", new_flight.departure_city);
    printf("Arrival City: ");
    scanf(" %[^\n]", new_flight.arrival_city);
    printf("Departure Time (HH:MM): ");
    scanf("%s", new_flight.departure_time);
    printf("Arrival Time (HH:MM): ");
    scanf("%s", new_flight.arrival_time);
    printf("Departure Date (YYYY-MM-DD): ");
    scanf("%s", new_flight.departure_date);
    printf("Total Seats: ");
    scanf("%d", &new_flight.total_seats);
    printf("Economy Price: ");
    scanf("%f", &new_flight.economy_price);
    printf("Business Price: ");
    scanf("%f", &new_flight.business_price);
    
    new_flight.available_seats = new_flight.total_seats;
    new_flight.is_active = 1;
    
    flights[flight_count++] = new_flight;
    printf("Flight added successfully! ID: %d\n", new_flight.flight_id);
}

void display_flights() {
    if(flight_count == 0) {
        printf("No flights available!\n");
        return;
    }
    
    printf("\nAll Flights:\n");
    printf("========================================================================================================\n");
    printf("ID  | Flight  | Airline       | From          | To            | Departure | Seats | Economy | Business\n");
    printf("========================================================================================================\n");
    
    for(int i = 0; i < flight_count; i++) {
        if(flights[i].is_active) {
            printf("%-3d | %-7s | %-13s | %-13s | %-13s | %-9s | %-5d | $%-6.2f | $%-7.2f\n",
                   flights[i].flight_id,
                   flights[i].flight_number,
                   flights[i].airline,
                   flights[i].departure_city,
                   flights[i].arrival_city,
                   flights[i].departure_time,
                   flights[i].available_seats,
                   flights[i].economy_price,
                   flights[i].business_price);
        }
    }
}

void search_flights() {
    if(flight_count == 0) {
        printf("No flights to search!\n");
        return;
    }
    
    char departure[30], arrival[30], date[11];
    printf("\nSearch Flights:\n");
    printf("Departure City: ");
    scanf(" %[^\n]", departure);
    printf("Arrival City: ");
    scanf(" %[^\n]", arrival);
    printf("Date (YYYY-MM-DD): ");
    scanf("%s", date);
    
    printf("\nAvailable Flights:\n");
    int found = 0;
    
    for(int i = 0; i < flight_count; i++) {
        if(flights[i].is_active && 
           strcasecmp(flights[i].departure_city, departure) == 0 &&
           strcasecmp(flights[i].arrival_city, arrival) == 0 &&
           strcmp(flights[i].departure_date, date) == 0) {
            
            printf("%d. %s %s -> %s | %s | Seats: %d | Economy: $%.2f | Business: $%.2f\n",
                   flights[i].flight_id,
                   flights[i].flight_number,
                   flights[i].departure_city,
                   flights[i].arrival_city,
                   flights[i].departure_time,
                   flights[i].available_seats,
                   flights[i].economy_price,
                   flights[i].business_price);
            found = 1;
        }
    }
    
    if(!found) {
        printf("No flights found for your search criteria.\n");
    }
}

void book_ticket() {
    if(flight_count == 0) {
        printf("No flights available for booking!\n");
        return;
    }
    
    if(booking_count >= MAX_PASSENGERS) {
        printf("Cannot accept more bookings!\n");
        return;
    }
    
    int flight_id;
    printf("Enter Flight ID to book: ");
    scanf("%d", &flight_id);
    
    int flight_index = -1;
    for(int i = 0; i < flight_count; i++) {
        if(flights[i].flight_id == flight_id && flights[i].is_active) {
            flight_index = i;
            break;
        }
    }
    
    if(flight_index == -1) {
        printf("Flight not found!\n");
        return;
    }
    
    if(flights[flight_index].available_seats <= 0) {
        printf("No seats available on this flight!\n");
        return;
    }
    
    Booking new_booking;
    new_booking.booking_id = next_booking_id++;
    new_booking.flight_id = flight_id;
    
    printf("\nPassenger Details:\n");
    printf("Full Name: ");
    scanf(" %[^\n]", new_booking.passenger_name);
    printf("Email: ");
    scanf("%s", new_booking.passenger_email);
    printf("Phone: ");
    scanf("%s", new_booking.passenger_phone);
    
    printf("Seat Class (economy/business): ");
    scanf("%s", new_booking.seat_class);
    
    if(strcasecmp(new_booking.seat_class, "economy") == 0) {
        new_booking.ticket_price = flights[flight_index].economy_price;
    } else if(strcasecmp(new_booking.seat_class, "business") == 0) {
        new_booking.ticket_price = flights[flight_index].business_price;
    } else {
        printf("Invalid seat class!\n");
        return;
    }
    
    printf("Special Requests: ");
    scanf(" %[^\n]", new_booking.special_requests);
    
    new_booking.seat_number = flights[flight_index].total_seats - flights[flight_index].available_seats + 1;
    get_current_date(new_booking.booking_date);
    strcpy(new_booking.status, "confirmed");
    
    bookings[booking_count++] = new_booking;
    flights[flight_index].available_seats--;
    
    printf("Booking successful! Booking ID: %d\n", new_booking.booking_id);
    printf("Seat Number: %d | Total Amount: $%.2f\n", new_booking.seat_number, new_booking.ticket_price);
}

void cancel_booking() {
    if(booking_count == 0) {
        printf("No bookings to cancel!\n");
        return;
    }
    
    int booking_id;
    printf("Enter Booking ID to cancel: ");
    scanf("%d", &booking_id);
    
    for(int i = 0; i < booking_count; i++) {
        if(bookings[i].booking_id == booking_id) {
            printf("Booking Found: %s - Flight ID: %d\n", 
                   bookings[i].passenger_name, bookings[i].flight_id);
            
            char confirm;
            printf("Are you sure you want to cancel? (y/n): ");
            scanf(" %c", &confirm);
            
            if(confirm == 'y' || confirm == 'Y') {
                for(int j = 0; j < flight_count; j++) {
                    if(flights[j].flight_id == bookings[i].flight_id) {
                        flights[j].available_seats++;
                        break;
                    }
                }
                
                strcpy(bookings[i].status, "cancelled");
                printf("Booking cancelled successfully!\n");
            }
            return;
        }
    }
    
    printf("Booking not found!\n");
}

void view_bookings() {
    if(booking_count == 0) {
        printf("No bookings found!\n");
        return;
    }
    
    printf("\nAll Bookings:\n");
    printf("==================================================================================\n");
    printf("Booking ID | Passenger Name       | Flight | Seat | Class    | Status    | Amount\n");
    printf("==================================================================================\n");
    
    for(int i = 0; i < booking_count; i++) {
        printf("%-10d | %-20s | %-6d | %-4d | %-8s | %-9s | $%-6.2f\n",
               bookings[i].booking_id,
               bookings[i].passenger_name,
               bookings[i].flight_id,
               bookings[i].seat_number,
               bookings[i].seat_class,
               bookings[i].status,
               bookings[i].ticket_price);
    }
}

void check_in() {
    if(booking_count == 0) {
        printf("No bookings for check-in!\n");
        return;
    }
    
    int booking_id;
    printf("Enter Booking ID for check-in: ");
    scanf("%d", &booking_id);
    
    for(int i = 0; i < booking_count; i++) {
        if(bookings[i].booking_id == booking_id) {
            if(strcmp(bookings[i].status, "confirmed") == 0) {
                strcpy(bookings[i].status, "checked-in");
                printf("Check-in successful for %s!\n", bookings[i].passenger_name);
                printf("Seat Number: %d | Flight ID: %d\n", 
                       bookings[i].seat_number, bookings[i].flight_id);
            } else {
                printf("Booking is already %s\n", bookings[i].status);
            }
            return;
        }
    }
    
    printf("Booking not found!\n");
}

void generate_reports() {
    if(flight_count == 0) {
        printf("No data for reports!\n");
        return;
    }
    
    printf("\n📊 Flight System Reports\n");
    printf("=======================\n");
    
    int total_bookings = 0;
    int confirmed_bookings = 0;
    float total_revenue = 0;
    
    for(int i = 0; i < booking_count; i++) {
        total_bookings++;
        if(strcmp(bookings[i].status, "confirmed") == 0 || 
           strcmp(bookings[i].status, "checked-in") == 0) {
            confirmed_bookings++;
            total_revenue += bookings[i].ticket_price;
        }
    }
    
    printf("Total Flights: %d\n", flight_count);
    printf("Total Bookings: %d\n", total_bookings);
    printf("Confirmed Bookings: %d\n", confirmed_bookings);
    printf("Total Revenue: $%.2f\n", total_revenue);
    
    printf("\nFlight Occupancy:\n");
    for(int i = 0; i < flight_count; i++) {
        if(flights[i].is_active) {
            float occupancy = ((float)(flights[i].total_seats - flights[i].available_seats) / flights[i].total_seats) * 100;
            printf("%s: %.1f%% occupied\n", flights[i].flight_number, occupancy);
        }
    }
}

void manage_flights() {
    if(flight_count == 0) {
        printf("No flights to manage!\n");
        return;
    }
    
    int flight_id;
    printf("Enter Flight ID to manage: ");
    scanf("%d", &flight_id);
    
    for(int i = 0; i < flight_count; i++) {
        if(flights[i].flight_id == flight_id) {
            printf("Current: %s %s -> %s | Seats: %d/%d\n",
                   flights[i].flight_number,
                   flights[i].departure_city,
                   flights[i].arrival_city,
                   flights[i].available_seats,
                   flights[i].total_seats);
            
            printf("Activate/Deactivate flight? (a/d/n): ");
            char choice;
            scanf(" %c", &choice);
            
            if(choice == 'a') {
                flights[i].is_active = 1;
                printf("Flight activated!\n");
            } else if(choice == 'd') {
                flights[i].is_active = 0;
                printf("Flight deactivated!\n");
            }
            return;
        }
    }
    
    printf("Flight not found!\n");
}

void save_to_file() {
    FILE *file = fopen(FILENAME, "wb");
    if(file == NULL) {
        printf("Error saving file!\n");
        return;
    }
    
    fwrite(&flight_count, sizeof(int), 1, file);
    fwrite(&booking_count, sizeof(int), 1, file);
    fwrite(&next_flight_id, sizeof(int), 1, file);
    fwrite(&next_booking_id, sizeof(int), 1, file);
    fwrite(flights, sizeof(Flight), flight_count, file);
    fwrite(bookings, sizeof(Booking), booking_count, file);
    
    fclose(file);
    printf("Data saved successfully!\n");
}

void load_from_file() {
    FILE *file = fopen(FILENAME, "rb");
    if(file == NULL) {
        printf("No previous data found.\n");
        return;
    }
    
    fread(&flight_count, sizeof(int), 1, file);
    fread(&booking_count, sizeof(int), 1, file);
    fread(&next_flight_id, sizeof(int), 1, file);
    fread(&next_booking_id, sizeof(int), 1, file);
    fread(flights, sizeof(Flight), flight_count, file);
    fread(bookings, sizeof(Booking), booking_count, file);
    
    fclose(file);
    printf("Data loaded successfully! (%d flights, %d bookings)\n", flight_count, booking_count);
}

void get_current_date(char *date) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date, "%04d-%02d-%02d", 
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}
