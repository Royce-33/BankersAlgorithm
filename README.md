# Banker's Algorithm
Multithreaded code that implements the Banker's Algorithm. Customers request and release resources from the bank, and the banker keeps track of these resources. The banker will grant the request if it satisfies the safety algorithm.
## Motivation

## Installation

## Screenshots

## Contributions
### Riley's Contribution
* Implemented main function
* Implemented read_file function
* Implemented customer_init function
* Implemented request_resources function
* Implemented run_safety_algorithm function
### Torin's Contribution
* Implemented main function
* Implemented release_resources function
* Implemented request_resources_safety function
* Implemented release_resources_safety function

## Features
* Handles manual resource request and release commands
    * Command structure follows the pattern of:
        * "RQ x x x x ... x" -> where the number of x's is the number of resources
        * "RL x x x x ... x" -> where the number of x's is the number of resources
* "Status" command displays maximum, allocated and needed resources for each customer as well as the resources available to all of them
* "Run" command runs the safe sequence algorithm with the system in its current state 


## Tests


## Code Example


## Authors
Riley Adams - adam6070@mylaurier.ca

Torin Borton-McCallum - bort4620@mylaurier.ca

## Credits


## License
MIT License -> See the LICENSE file for details.
