#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <bits/types/time_t.h>

#define MAXCHAR 1000
#define ROWS 5
#define COLS 5

// Define the structures
typedef struct patient {
    int id;
    char name[50];
    int priority; // 0 for patients who come by themselves, 1 for patients who come by ambulance
    int seatNumber;
    struct patient* next;
} PT;

typedef struct {
    int seat_number;
    int busy_seat; // 0 for unoccupied, 1 for occupied
    time_t lastFreed; // time when the seat was last freed
} ST;


typedef struct {
    PT* patients;
    ST seats[ROWS * COLS];
} HS;

// Define the functions
void UI();
HS* InitializeHospital();
HS* addPatient(HS* hospital, PT patient, int cooldownPeriod);
HS* rmPatient(HS* hospital);
void SeatLocation(HS* hospital, int patientId);
int assign_seat(HS* hospital, int cooldownPeriod);
void clearSeat(HS* hospital, int seatNumber);
void getSeatPartners(HS* hospital, int seatNumber);
void insert_patient(PT* head, PT* newPatient);
void print_patients(HS* hospital);
void free_hospital(HS* hospital);



#endif // HOSPITAL_H
