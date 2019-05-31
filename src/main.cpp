#include <iostream>
#include <getopt.h>
#include "QuickSort.h"
#include "MergeSort.h"
#include "InsertionSort.h"
#include <assert.h>
#include <sched.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

bool getVerboseOption(int argc, char *argv[]);

std::string toLower(string algorithm);

void print_affinity() {
    cpu_set_t mask;
    long nproc, i;

    if (sched_getaffinity(0, sizeof(cpu_set_t), &mask) == -1) {
        perror("sched_getaffinity");
        assert(false);
    } else {
        nproc = sysconf(_SC_NPROCESSORS_ONLN);
        printf("sched_getaffinity = ");
        for (i = 0; i < nproc; i++) {
            printf("%d ", CPU_ISSET(i, &mask));
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {

    int cpu_aff = atoi(argv[5]);

    if (cpu_aff >= 0)
    {
        cpu_set_t mask;

        CPU_ZERO(&mask);
        CPU_SET(cpu_aff, &mask);
        if (sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1) {
            perror("sched_setaffinity");
            assert(false);
        }
    }

    if (argc >= 6) {

        bool isVerbose = getVerboseOption(argc, argv);
        if (isVerbose) { argv++; }

        int arraySize = atoi(argv[1]);
        char *inputFile = argv[2];
        char *outputFile = argv[3];
        std::string algorithm = toLower(argv[4]);

        if (algorithm == QUICK_SORT) {
            QuickSort qs(arraySize, inputFile, outputFile, isVerbose);
            qs.sort();
        } else if (algorithm == MERGE_SORT) {
            MergeSort ms(arraySize, inputFile, outputFile, isVerbose);
            ms.sort();
        } else if (algorithm == INSERTION_SORT) {
            InsertionSort is(arraySize, inputFile, outputFile, isVerbose);
            is.sort();
        }
    } else {
        cerr << "Error occured." << endl;
        return -1;
    }

    return 0;
}

/**
 * Parse the input arguments and check if the '-v' option is set.
 * @param argc
 * @param argv
 * @return
 */
bool getVerboseOption(int argc, char *argv[]) {
    bool isVerbose = false;
    int option = 0;
    while ((option = getopt(argc, argv, "v")) != -1) {
        switch (option) {
            case 'v' :
                isVerbose = true;
            default:
                break;
        }
    }

    return isVerbose;
}

/**
 * Convert the given string to lowercase.
 * @param algorithm
 * @return
 */
std::string toLower(string algorithm) {
    std::transform(algorithm.begin(), algorithm.end(), algorithm.begin(), ::tolower);
    return algorithm;
}
