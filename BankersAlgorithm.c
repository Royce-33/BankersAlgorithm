/*
-------------------------------------
File:   BankersAlgorithm.c           for submission change it to "190416070_190824620_a04.c"
Project: CP386 Assignment 4
Creates threads in a function and with the main and prints which is active, each runs 5 times
-------------------------------------
Author:  Riley Adams & Torin Borton-McCallum
ID:      190416070 & 190824620
Email:   adam6070@mylaurier.ca & bort4620@mylaurier.ca
Version  2021-06-18
-------------------------------------
 */

/**
 * List of functions/functionality:
 *  - Banker should keep track of the available, maximum, currently allocated, and remaining need amounts of each resource
 *  - Implements the function for requesting resources, returns 0 if successful, -1 otherwise
 *  - Implements the function for releasing resources
 *  - Includes a safety algorithm to grant a request if it leaves the system in a safe state, denies it if otherwise
 *  - Allows the user to interactively enter a request for resources, to release resources, or to output the values of the different data structures
 *  - Should be invoked by passing the number of resources of each type via CLI to initialize the available array by said values.
 *    For example: "./q4 10 5 7 8" would set available to 10 for type A, 5 for type B, 7 for type C, and 8 for type D
 *  - Read in the sample input file, it contains the maximum number of requests for each customer (five customers, four resources)
 *    Where each line represents the maximum request of each resource type for each customer, the maximum data structure is initialized to these values.
 *  - Code will take in user entered commands regarding the requesting, releasing, and displaying of resources.
 *    Commands are as follows:
 *      * "RQ" -> requests resources and fills the allocation array, remember that a customer cannot request anything more than its maximum
 *      * "RL" -> releases resources entered from the the given customer 
 *      * "*" -> Display the current state of available, maximum, allocation, and need arrays
 *      * "Run" -> Executes the safe sequence based on the current state and all the threads run the same function code and prints (see assign page for example output)
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdbool.h>

/* BEGIN STRUCTURE FUNCTION AND GLOBAL VARIABLE DEFINITION */
#define NUM_RESOURCES 4 //Change me if you want to change the number resources available
const char *FILE_NAME = "sample4_in.txt";
const int BUFFER_SIZE = 32;
typedef struct customer {

    char *maximum[NUM_RESOURCES];
    char *allocated[NUM_RESOURCES];
    char *need[NUM_RESOURCES];
    int id;

} customer;


int main(int argc, char *args[]);
int read_file();
int customer_init(char *maximum_resources[]);
int request_resources(customer customer);
int release_resources(customer customer);





int num_customers = 0;
int availabe_resources[NUM_RESOURCES];
/* END DEFINITIONS */




int main (int argc, char *args[]) {

    read_file(); //read in the contents of the file and initialize customer objects

    return 0;
}


/**
 * Reads the given input file and passes each line as an array of ints to the customer_init function
 */
int read_file() {

    FILE *in_file = fopen(FILE_NAME, "r"); //open the file in read mode
    
    char line[BUFFER_SIZE];
     
    //while we haven't reached the end of the file, read the current line into a char array and pass it to customer_init
    while ( !feof(in_file) ) {
        //printf("passed end of file check\n");
        char *curr_max[NUM_RESOURCES];

        for (int i = 0; fgets(line, BUFFER_SIZE, in_file) != NULL ; i++ ) { //this for loop currently only takes the first number from each line because of how strtok works

            printf("contents of line: %s", line);
            curr_max[i] = strtok(line, ",");
            printf("curr_max array contents: %s\n", *curr_max);
            printf("Adding %s to curr_max array\n", curr_max[i]);
            strtok(NULL, "");

            num_customers++; //update the number of customers
            printf("updated num customers: %d\n", num_customers);
            customer_init(curr_max); //current maximum resource allocation is set, initialize the customer object

        }

        

    }

}

/**
 * Helper function used in read_file(), takes the given array of ints and initializes a new customer object
 */
int customer_init(char *maximum_resources[]){



}