#include "hospital.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void UI() {
    printf("\n1. Add a patient who came by themselves\n");
    printf("2. Add a patient who came by ambulance\n");
    printf("3. Remove a patient after successful treatment\n");
    printf("4. Query seat partners of a patient\n");
    printf("5. Print all patients\n");
    printf("6. Load patients from CSV\n");
    printf("0. Exit\n");
}
HS* InitializeHospital() {

    //Allocating memory for the Hospital struct
    HS* hospital = (HS*)malloc(sizeof(HS));
    if (hospital == NULL) {
        fprintf(stderr, "Failed to allocate memory for hospital\n");
        exit(EXIT_FAILURE);
    }
    hospital->patients = NULL;
    for (int i = 0; i < ROWS * COLS; i++) {
        hospital->seats[i].seat_number = i;
        hospital->seats[i].busy_seat = 0;
    }
    return hospital;
}

HS* addPatient(HS* hospital, PT patient, int cooldownPeriod) {
    PT* newPatient = malloc(sizeof(PT));
    if(newPatient == NULL){
        fprintf(stderr, "Failed to allocate memory for new patient\n");
        return hospital;
    }
    *newPatient = patient;  // copy data
    newPatient->next = NULL;

    // Assign a seat to the patient
    if (newPatient->priority == 1) {  // self-arrived
        newPatient->seatNumber = assign_seat(hospital, cooldownPeriod);
    } else {
        newPatient->seatNumber = -1;  // indicates no seat, came by ambulance
    }

    // Add the patient to the hospital list
    if (hospital->patients == NULL) {
        hospital->patients = newPatient;
    } else {
        insert_patient(hospital->patients, newPatient); // directly insert the node into the list
    }

    return hospital;
}

HS* rmPatient(HS* hospital) {
    if (hospital->patients == NULL) {
        printf("No patients to treat and remove.\n");
        return hospital;
    }

    PT *temp = hospital->patients, *prev = NULL;
    PT *toRemove = temp, *prevToRemove = NULL;

    // Traverse the patient list to find the patient with the highest priority
    while (temp != NULL) {
        if (temp->priority > toRemove->priority) {
            toRemove = temp;
            prevToRemove = prev;
        }
        prev = temp;
        temp = temp->next;
    }

    // If the patient to remove is at the head of the list
    if (toRemove == hospital->patients) {
        hospital->patients = toRemove->next;
    } else {
        prevToRemove->next = toRemove->next;
    }

    // If the patient had a seat, free up the seat
    if (toRemove->priority == 0) {
        int seatNumber = toRemove->seatNumber;
        hospital->seats[seatNumber].busy_seat = 0;
        hospital->seats[seatNumber].lastFreed = time(NULL); // update the time when the seat was last freed
    }

    printf("Patient %d successfully treated and removed.\n", toRemove->id);
    free(toRemove);

    return hospital;
}

void SeatLocation(HS* hospital, int patientId) {
    PT* current = hospital->patients;

    // Find the patient
    while (current != NULL) {
        if (current->id == patientId) {
            break;
        }
        current = current->next;
    }

    // Check if the patient was found
    if (current == NULL) {
        printf("Patient with ID: %d does not exist.\n", patientId);
        return;
    }

    // Check if the patient has a seat
    if (current->seatNumber == -1) {
        printf("Patient with ID: %d came by ambulance and does not have a seat.\n", patientId);
        return;
    }

    // If the patient exists and has a seat, find their seat partners
    getSeatPartners(hospital, current->seatNumber);
}

int assign_seat(HS* hospital, int cooldownPeriod) {
    time_t currentTime;
    time(&currentTime); // get the current time

    int minAdjacent = ROWS * COLS;
    int thisSeat = -1;

    for (int i = 0; i < ROWS * COLS; i++) {
        if (hospital->seats[i].busy_seat == 0 && difftime(currentTime, hospital->seats[i].lastFreed) >= cooldownPeriod) {
            int row = i / COLS;
            int col = i % COLS;
            int occupiedAdjacent = 0;

            // Check left seat
            if (col > 0 && hospital->seats[i - 1].busy_seat) {
                occupiedAdjacent++;
            }

            // Check right seat
            if (col < COLS - 1 && hospital->seats[i + 1].busy_seat) {
                occupiedAdjacent++;
            }

            // Check top seat
            if (row > 0 && hospital->seats[i - COLS].busy_seat) {
                occupiedAdjacent++;
            }

            // Check bottom seat
            if (row < ROWS - 1 && hospital->seats[i + COLS].busy_seat) {
                occupiedAdjacent++;
            }

            if (occupiedAdjacent < minAdjacent) {
                minAdjacent = occupiedAdjacent;
                thisSeat = i;
            }
        }
    }

    if (thisSeat != -1) {
        hospital->seats[thisSeat].busy_seat = 1;
        hospital->seats[thisSeat].lastFreed = 0; // Reset lastFreed as it's now occupied
    }

    return thisSeat;
}



void clearSeat(HS* hospital, int seatNumber) {
    hospital->seats[seatNumber].busy_seat = 0;
    time(&hospital->seats[seatNumber].lastFreed); // set lastFreed to the current time
}

void getSeatPartners(HS* hospital, int seatNumber) {
    int row = seatNumber / COLS;
    int col = seatNumber % COLS;
    int partnerSeat;

    // Check the seat to the left
    if (col > 0) {
        partnerSeat = seatNumber - 1;
        if (hospital->seats[partnerSeat].busy_seat) {
            printf("Seat %d is a partner of seat %d\n", partnerSeat, seatNumber);
        }
    }

    // Check the seat to the right
    if (col < COLS - 1) {
        partnerSeat = seatNumber + 1;
        if (hospital->seats[partnerSeat].busy_seat) {
            printf("Seat %d is a partner of seat %d\n", partnerSeat, seatNumber);
        }
    }

    // Check the seat in front
    if (row > 0) {
        partnerSeat = seatNumber - COLS;
        if (hospital->seats[partnerSeat].busy_seat) {
            printf("Seat %d is a partner of seat %d\n", partnerSeat, seatNumber);
        }
    }

    // Check the seat behind
    if (row < ROWS - 1) {
        partnerSeat = seatNumber + COLS;
        if (hospital->seats[partnerSeat].busy_seat) {
            printf("Seat %d is a partner of seat %d\n", partnerSeat, seatNumber);
        }
    }
}

void insert_patient(PT* head, PT* newPatient) {
    PT* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newPatient;
}

void print_patients(HS* hospital) {
    PT* current = hospital->patients;
    while (current != NULL) {
        printf("Patient ID: %d, Name: %s, Priority: %d, Seat: %d\n", current->id, current->name, current->priority, current->seatNumber);
        current = current->next;
    }
}


void free_hospital(HS* hospital) {
    // First deallocate the memory for the patients
    PT* current = hospital->patients;
    while (current != NULL) {
        PT* next = current->next;
        free(current);
        current = next;
    }

    // Then deallocate the memory for the hospital
    free(hospital);
}


