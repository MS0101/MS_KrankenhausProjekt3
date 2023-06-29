#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hospital.h"
#include "CSVFILE.h"

int main() {
    HS* hospital = InitializeHospital();
    int cooldownPeriod = 60;  // define the cooldown period
    if (!hospital) {
        fprintf(stderr, "Failed to initialize the hospital\n");
        return EXIT_FAILURE;
    }

    int choice;
    do {
        UI();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: {
                PT newPatient;
                printf("Enter the patient ID: ");
                scanf("%d", &newPatient.id);
                printf("Enter the patient's name: ");
                scanf("%s", newPatient.name);

                newPatient.seatNumber = -1;
                newPatient.priority = 1;
                hospital = addPatient(hospital, newPatient, cooldownPeriod);
                break;
            }
            case 2: {
                PT newPatient;
                printf("Enter the patient ID: ");
                scanf("%d", &newPatient.id);
                printf("Enter the patient's name: ");
                fgets(newPatient.name, sizeof(newPatient.name), stdin);
                newPatient.name[strcspn(newPatient.name, "\n")] = '\0';
                printf("Enter the patient's priority: ");
                scanf("%d", &newPatient.priority);
                getchar();
                newPatient.seatNumber = -1;
                newPatient.priority = 0;
                hospital = addPatient(hospital, newPatient, cooldownPeriod);
                break;
            }
            case 3:
                hospital = rmPatient(hospital);
                break;
            case 4: {
                int patientId;
                printf("Enter the ID of the patient: ");
                scanf("%d", &patientId);
                SeatLocation(hospital, patientId);
                break;
            }
            case 5:
                print_patients(hospital);
                break;
            case 6: {
                char filename[MAXCHAR];
                printf("Enter the filename: ");
                scanf("%s", filename);
                OpenCSVFile(hospital, filename, cooldownPeriod);
                break;
            }
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please choose a valid option.\n");
        }
    } while (choice != 0);

    free_hospital(hospital);

    return 0;
}
