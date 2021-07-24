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

    char *maximum;
    char *allocated;
    char *need;
    int id;

} customer;


int main(int argc, char *args[]);
int read_file();
customer *customer_init(char **maximum_resources);
int request_resources(customer *customer);
int release_resources(customer customer);





int num_customers = 0;
char availabe_resources[32];
customer *customers = NULL; //pointer to a list of customers

/* END DEFINITIONS */




int main (int argc, char *args[]) {

    //customers = (customer *) malloc(sizeof(customer) * 5);
    
    if (argc < 2) {

        printf("main -> Not enough available resources inputted, please try again!\n");
        return -1;
    }

    else {
        
        for (int i = 0; i < argc; i++) {

            printf("%d argument: %s\n", i, args[i]);

        }




        for (int i = 0; i < argc; i++) {

            //printf("Current argument: %s\n", args[i]);
            if ( i == 0 )
                continue;


            else if (i == argc - 1) {
                strcat(availabe_resources, args[i]);
            }

            else {
                char *temp = malloc(32);
                strcpy(temp, args[i]);
                strcat(temp, ",");
                printf("temp value: %s\n", temp);
                strcat(availabe_resources, temp);
                free(temp);
            }
               
        }
        
        
        read_file(); //read in the contents of the file and initialize customer objects
    //printf("contents of the global customer list: %s\n", customers);

        for (int i = 0; i < num_customers; i++) { //in current state everything gets assigned properly within read_file's local list, but once the global variable is assigned that list, all the needs and max values are the same
            //printf("%d: current resource allocation: %s\n", customers[i].id, customers[i].allocated);
            printf("%d: needed resource allocation: %s\n", customers[i].id, customers[i].need);
            printf("%d: maximum resource allocation: %s\n", customers[i].id, customers[i].maximum);
        }

        printf("Number of customers: %d\n", num_customers);

        printf("Currently available resources: %s\n", availabe_resources);

        printf("Maximum resources from file:\n");

        for (int i = 0; i < num_customers; i++) {
            printf("%s\n", customers[i].maximum);
        }


    }



    return 0;
}


/**
 * Reads the given input file and passes each line as an array of ints to the customer_init function
 */
int read_file() {

    FILE *in_file = fopen(FILE_NAME, "r"); //open the file in read mode
    
    char line[BUFFER_SIZE];
    int customer_counter = 1; //helps to realloc the size of the pointer list properly
    
    customer *incoming_customers = NULL; //works similairly to the global list, collects each customer as it comes in

    incoming_customers = (customer *) malloc(sizeof(customer)); //intial setup so it can take a customer, and will resize to fit more
    //while we haven't reached the end of the file, read the current line into a char array and pass it to customer_init
    while ( !feof(in_file) ) {
        //printf("passed end of file check\n");
        char *curr_max[NUM_RESOURCES];

        

        for (int i = 0; fgets(line, BUFFER_SIZE, in_file) != NULL ; i++ ) { //this for loop takes each line in the file and sends it off to customer_init

            

            //printf("contents of line: %s", line);
            curr_max[i] = strtok(line, "\r\n");
            //printf("curr_max array contents: %s\n", *curr_max);
            //printf("Adding %s to curr_max array\n", curr_max[i]);
            strtok(NULL, "");

            num_customers++; //update the number of customers globally
            //printf("updated num customers: %d\n", num_customers);
            
            //customer_init(curr_max);
            incoming_customers[i] = *(customer_init(curr_max)); //current maximum resource allocation is set, initialize the customer object
            printf("current maximum resources value of incoming customers at position %d: %s\n", incoming_customers[i].id, incoming_customers[i].maximum);
            customer_counter++; //updating realloc counter
            
            
            incoming_customers = realloc( incoming_customers, (sizeof(customer) * customer_counter) ); //increasing the size of the list so it can fit the next customer
            
        }
        

    }

    for (int i = 0; i < customer_counter; i++) { //in current state, customer objects are created properly, but the list of objects has max and need properties of only the last entry
        printf("%d: local list maximum resources: %s\n", incoming_customers[i].id, incoming_customers[i].maximum);
    }


    customers = *(&incoming_customers); //we have received all incoming customers, pass the list to the global variable                              

    return 0;

}

/**
 * Helper function used in read_file(), takes the given array of ints and initializes a new customer object
 */
customer *customer_init(char **maximum_resources) {

    customer *new_customer = (customer *) malloc(sizeof(customer));
    
    new_customer->maximum = *maximum_resources;
    new_customer->need = *maximum_resources;
    new_customer->allocated = malloc(NUM_RESOURCES);
    
    for (int i = 0; i < NUM_RESOURCES; i++) {
        //strcpy(new_customer->allocated, "0");
        if (i == NUM_RESOURCES - 1) {
            strcat(new_customer->allocated, "0");
            break;
        }
        strcat(new_customer->allocated, "0,");
    }

    new_customer->id = num_customers - 1; //takes the non-updated num_customers so first customer id is zero and so on

    printf("%d: Setting max resources to: %s\n", new_customer->id, new_customer->maximum);
    printf("%d: Setting needed resources to: %s\n", new_customer->id, new_customer->need);
    printf("%d: Setting allocated resources to: %s\n", new_customer->id, new_customer->allocated);

    return new_customer;

}

/**
 * Function used to request resources from the banker
 */
int request_resources(customer *customer){

    char *customer_max = customer->maximum;
    char *customer_need = customer->need;
    char *customer_allocated = customer->allocated;
    int customer_id = customer->id;

    if (customer_need == "0,0,0,0" || customer_allocated == customer_max) { //if the customer does not need more or has reached the maximum allocated


    }




}