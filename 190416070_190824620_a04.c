/*
-------------------------------------
File:   Question1.c
Project: CP386 Assignment 2
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