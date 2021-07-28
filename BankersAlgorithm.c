/*
-------------------------------------
File:   BankersAlgorithm.c           for submission change it to "190416070_190824620_a04.c"
Project: CP386 Assignment 4
Creates threads in a function and with the main and prints which is active, each runs 5 times

Link to GitHub Repository: 
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
 *      * "Status" -> Display the current state of available, maximum, allocation, and need arrays
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

    int *maximum;
    int *allocated;
    int *need;
    int id;
    bool finished;

} customer;


int main(int argc, char *args[]);
int read_file();
customer *customer_init(int *maximum_resources);
int request_resources(customer *customer, int *requested_resources);
int release_resources(customer customer);
int command_handler();




int num_customers = 0;
int available_resources[32];
customer *customers = NULL; //pointer to a list of customers



/* END DEFINITIONS */




int main (int argc, char *args[]) {

    //customers = (customer *) malloc(sizeof(customer) * 5);
    
    if (argc < 2) {

        printf("main -> Not enough available resources inputted, please try again!\n");
        return -1;
    }

    else {

        for (int i = 0; i < argc; i++) { //setting up available resources as defined with input

            //printf("Current argument: %s\n", args[i]);
            if ( i != 0 ) {
                //printf("current arg: %s\n", args[i]);
                int curr_arg = atoi(args[i]);
                //printf("integer version: %d\n", curr_arg);
                available_resources[i] = curr_arg;
                //printf("array element %d: %d\n", i, availabe_resources[i]);
            }
                

               
        }
        
        
        read_file(); //read in the contents of the file and initialize customer objects
        //printf("contents of the global customer list: %s\n", customers);

        printf("Number of customers: %d\n", num_customers);

        printf("Currently available resources:");

        for (int i = 1; i < NUM_RESOURCES + 1; i++){
            if (i == (NUM_RESOURCES + 1) - 1)
                printf(" %d\n", available_resources[i]);

            else    
                printf(" %d", available_resources[i]);
        }

        printf("Maximum resources from file:\n");

        for (int i = 1; i < num_customers + 1; i++) { //realloc adds empty space at the start of the list, so we need to start from second element and go 1 past num_customers to see all of them
            
            //printf("outside for loop iteration %d\n", i);

            for (int j = 0; j < NUM_RESOURCES; j++) {

                //if (customers[i].id != NULL) {
                     if (j == NUM_RESOURCES - 1) {
                    printf(" %d\n", customers[i].maximum[j]);
                }

                else if (j == 0) {
                    
                    printf("%d: %d", customers[i].id, customers[i].maximum[j]);
                }
                    

                else
                    printf(" %d", customers[i].maximum[j]);
                //}

               
            }

        }






    }


    command_handler();


    return 0;
}


/**
 * Reads the given input file and passes each line as an array of ints to the customer_init function
 */
int read_file() {

    FILE *in_file = fopen(FILE_NAME, "r"); //open the file in read mode
    
    char line[BUFFER_SIZE];
    
    
    customer *incoming_customers = NULL; //works similairly to the global list, collects each customer as it comes in

    
    if (in_file != NULL) {

        incoming_customers = (customer *) malloc(sizeof(customer) * 5); //intial setup so it can take a customer, and will resize to fit more
        int customer_counter = 1; //helps to realloc the size of the pointer list properly
        int k = 0;
        while (fgets(line, BUFFER_SIZE, in_file) != NULL) {
            char *line_token = line; //
            //printf("line_token value: %s\n", line_token);
            
            int curr_max[NUM_RESOURCES];
            char *char_token = strtok(line_token, ",");
            int i = 0;
            while (char_token != NULL) {
                
                curr_max[i] = atoi(char_token);
                //printf("curr_max[i] value: %d\n", curr_max[i]);
                char_token = strtok(NULL, ",");
                i++;


            }

            k++;
            num_customers++;
            customer_counter++;
            // printf("calling customer_init with:\n");

            // for (int i = 0; i < NUM_RESOURCES; i++) {

            //     if (i == NUM_RESOURCES - 1) {
            //         printf(" %d\n", curr_max[i]);

            //     }

            //     else
            //         printf(" %d", curr_max[i]);

            // }

            incoming_customers[k] = *customer_init(curr_max);
            customer_counter++;
            incoming_customers = realloc(incoming_customers, sizeof(customer) * customer_counter);

        }

        customers = incoming_customers;

    }

    return 0;

}
                          

/**
 * Helper function used in read_file(), takes the given array of ints and initializes a new customer object
 */
customer *customer_init(int *maximum_resources) {

    customer *new_customer = (customer *) malloc(sizeof(customer));
    
    new_customer->maximum = malloc(NUM_RESOURCES);
    new_customer->need = malloc(NUM_RESOURCES);
    new_customer->allocated = malloc(NUM_RESOURCES);
    new_customer->finished = false;

    for (int i = 0; i < NUM_RESOURCES; i++) {
        new_customer->maximum[i] = maximum_resources[i];
    }

    for (int i = 0; i < NUM_RESOURCES; i++) {
        new_customer->need[i] = maximum_resources[i];
    }
    
    for (int i = 0; i < NUM_RESOURCES; i++) {
        new_customer->allocated[i] = 0;
    }

    new_customer->id = num_customers - 1; //takes the non-updated num_customers so first customer id is zero and so on


    //Testing print statements
    // printf("%d: Setting max resources to:", new_customer->id);

    // for (int i = 0; i < NUM_RESOURCES; i++) {

    //     if (i == NUM_RESOURCES - 1) {
    //         printf(" %d\n", new_customer->maximum[i]);
    //     }
    //     else
    //         printf(" %d", new_customer->maximum[i]);
    // }

    // printf("%d: Setting needed resources to:", new_customer->id);

    // for (int i = 0; i < NUM_RESOURCES; i++) {

    //     if (i == NUM_RESOURCES - 1) {
    //         printf(" %d\n", new_customer->need[i]);
    //     }
    //     else
    //         printf(" %d", new_customer->need[i]);
    // }
    // printf("%d: Setting allocated resources to:", new_customer->id);

    // for (int i = 0; i < NUM_RESOURCES; i++) {

    //     if (i == NUM_RESOURCES - 1) {
    //         printf(" %d\n", new_customer->allocated[i]);
    //     }
    //     else
    //         printf(" %d", new_customer->allocated[i]);
    // }

    return new_customer;

}

/**
 * Function used to request resources from the banker
 */
int request_resources(customer *customer, int *request_resources){

    int *customer_max = customer->maximum;// read file 
    int *customer_need = customer->need; //customer_max - customer_allocated
    int *customer_allocated = customer->allocated;// user input -> RQ 11111
    int customer_id = customer->id;
    // available resources (original) = user input -> Currently Available resources:10 5 7 8
    //remaining resourcesa = available resources [i] - allocated resources[i]
    
    //fail 
    for (int i = 0; i < 4; i ++){
        if (available_resources[i] - customer_allocated[i] < 0){
            //failure print statement
        }
        else{
            available_resources[i] = available_resources[i] - customer_allocated[i];
            customer_need[i] = customer_max[i] - customer_allocated[i];
        }
    }





    //this way would've been a NIGHTMARE to compare each element in the string etc, and then making decisions and doing the appropriate math, which is why I 
    //changed the structure of the customer to be ints so we can easily compare resources and do math appropriately
    
    // if (customer_need == "0,0,0,0" || customer_allocated == customer_max) { //if the customer does not need more or has reached the maximum allocated

    //     printf("%d: Customer already has maximum allocation of resources!\n", customer->id);

    // }

    //printf("comparing strings %d and %d: strcmp() result: %d\n", availabe_resources[0], customer_need[0], strcmp(&availabe_resources[0], &customer_need[0]));


    //else if ()

    




}






/**
 * Function used to handle user inputted commands once customer object setup is complete
 */
int command_handler() {

    char command_line[BUFFER_SIZE];

    printf("Enter a command: ");
    fgets(command_line, BUFFER_SIZE, stdin); //gets the inputted command 

    char *input = strtok(command_line, "\r\n"); //remove the newline character so we can compare strings properly
    
    //printf("results of strncmp against %s and quit: %d\n", command, strncmp(command, "quit", BUFFER_SIZE));

    while (strcmp(input, "Exit") != 0) {

        char *command = strtok(input, " ");

        //if the first set of characters is RQ, then get the requested resources and send them to a thread that calls request_resources
        if (strcmp(command, "RQ") == 0) {

            int request[NUM_RESOURCES];
            int i = 0;
            command = strtok(NULL, " ");
            int customer_id = atoi(command);
            while(command != NULL) {
                
                command = strtok(NULL, " ");
                request[i] = atoi(command);
                printf("value of request[i]: %d\n", request[i]);
                i++;
            }
            

        }


        //if the first set of characters is RL, then get the released resources and send them to a thread that calls release_resources

        //if the first set of characters is Run, then create a thread that calls the safety algorithm

        //if the first set of characters is Status, then create a thread that calls the display_status function



    }

    printf(">>> Program Terminated.\n");

}