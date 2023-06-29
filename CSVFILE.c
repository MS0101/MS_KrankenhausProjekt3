//
// Created by MS on 29.06.2023.
//
// csv_operations.c

#include "CSVFILE.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void OpenCSVFile(HS* hospital, char* filename, int cooldownPeriod) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return;
    }

    char line[MAXCHAR];
    while (fgets(line, sizeof(line), file)) {
        PT newPT;
        char* token = strtok(line, ",");
        newPT.id = atoi(token);

        token = strtok(NULL, ",");
        strncpy(newPT.name, token, sizeof(newPT.name));

        token = strtok(NULL, ",");
        newPT.priority = atoi(token);

        newPT.seatNumber = -1;  // Will be assigned in addPatient if necessary

        HS* tempHospital = addPatient(hospital, newPT, cooldownPeriod);
        if(tempHospital == NULL){
            fprintf(stderr, "Failed to add new patient from CSV file\n");
            fclose(file);
            return;
        }
        hospital = tempHospital;
    }

    fclose(file);
}
