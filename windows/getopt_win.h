/**
 * Windows-compatible getopt implementation
 * 
 * This header provides POSIX getopt and getopt_long functionality on Windows
 * where these functions are not available natively.
 * 
 * Usage: Include this header instead of <getopt.h> on Windows systems.
 */

#pragma once

#ifdef _WIN32

#include <string>
#include <cstring>
#include <iostream>

// Global variables for getopt (POSIX compatibility)
extern char* optarg;
extern int optind, opterr, optopt;

// Option structure for getopt_long
struct option {
    const char* name;     // Long option name
    int has_arg;         // Argument requirement
    int* flag;           // Flag to set (if not NULL)
    int val;             // Value to return or flag value
};

// Argument requirements constants
#define no_argument       0
#define required_argument 1
#define optional_argument 2

// Function declarations
int getopt(int argc, char* const argv[], const char* optstring);
int getopt_long(int argc, char* const argv[], const char* optstring,
                const struct option* longopts, int* longindex);

// Global variables definition
#ifndef GETOPT_IMPLEMENTATION
char* optarg = nullptr;
int optind = 1;
int opterr = 1;
int optopt = 0;

// Internal state variables
namespace {
    char* nextchar = nullptr;
    bool posixly_correct = false;
}

/**
 * Parse short options from command line arguments
 * 
 * @param argc Number of arguments
 * @param argv Array of argument strings
 * @param optstring String defining valid options
 * @return Option character, -1 if done, '?' if error
 */
int getopt(int argc, char* const argv[], const char* optstring) {
    // Check for end conditions
    if (optind >= argc || argv[optind] == nullptr) {
        return -1;
    }
    
    // Handle non-option arguments
    if (argv[optind][0] != '-' || argv[optind][1] == '\0') {
        return -1;
    }
    
    // Handle "--" (end of options)
    if (std::strcmp(argv[optind], "--") == 0) {
        optind++;
        return -1;
    }
    
    // Initialize nextchar if needed
    if (nextchar == nullptr || *nextchar == '\0') {
        nextchar = argv[optind] + 1;
    }
    
    // Get current option character
    char c = *nextchar++;
    
    // Look for option in optstring
    const char* cp = std::strchr(optstring, c);
    
    if (cp == nullptr || c == ':') {
        // Invalid option
        if (opterr && *optstring != ':') {
            std::cerr << argv[0] << ": invalid option -- '" << c << "'" << std::endl;
        }
        optopt = c;
        
        // Move to next argument if at end of current one
        if (*nextchar == '\0') {
            optind++;
            nextchar = nullptr;
        }
        return '?';
    }
    
    // Check if option requires an argument
    if (cp[1] == ':') {
        if (*nextchar != '\0') {
            // Argument immediately follows option
            optarg = nextchar;
            optind++;
            nextchar = nullptr;
        } else if (++optind >= argc) {
            // Missing required argument
            if (opterr && *optstring != ':') {
                std::cerr << argv[0] << ": option requires an argument -- '" << c << "'" << std::endl;
            }
            optopt = c;
            return (*optstring == ':') ? ':' : '?';
        } else {
            // Argument is next parameter
            optarg = argv[optind++];
        }
    } else {
        // Option doesn't take an argument
        optarg = nullptr;
        if (*nextchar == '\0') {
            optind++;
            nextchar = nullptr;
        }
    }
    
    return c;
}

/**
 * Parse long options from command line arguments
 * 
 * @param argc Number of arguments
 * @param argv Array of argument strings
 * @param optstring String defining valid short options
 * @param longopts Array of long option structures
 * @param longindex Pointer to store index of matched long option
 * @return Option value, -1 if done, '?' if error
 */
int getopt_long(int argc, char* const argv[], const char* optstring,
                const struct option* longopts, int* longindex) {
    
    // Check for end conditions
    if (optind >= argc || argv[optind] == nullptr) {
        return -1;
    }
    
    // Handle non-option arguments
    if (argv[optind][0] != '-') {
        return -1;
    }
    
    // Handle "--" (end of options)
    if (std::strcmp(argv[optind], "--") == 0) {
        optind++;
        return -1;
    }
    
    // Check if this is a long option (starts with "--")
    if (argv[optind][1] == '-') {
        // Long option processing
        const char* name = argv[optind] + 2;
        const char* eq = std::strchr(name, '=');
        size_t name_len = eq ? static_cast<size_t>(eq - name) : std::strlen(name);
        
        // Search for matching long option
        for (int i = 0; longopts[i].name != nullptr; i++) {
            if (std::strncmp(longopts[i].name, name, name_len) == 0 &&
                std::strlen(longopts[i].name) == name_len) {
                
                // Found a match
                if (longindex) {
                    *longindex = i;
                }
                
                optind++;
                
                // Handle option argument
                if (longopts[i].has_arg == required_argument) {
                    if (eq) {
                        // Argument provided with '='
                        optarg = const_cast<char*>(eq + 1);
                    } else if (optind < argc) {
                        // Argument is next parameter
                        optarg = argv[optind++];
                    } else {
                        // Missing required argument
                        if (opterr) {
                            std::cerr << argv[0] << ": option '--" << longopts[i].name 
                                     << "' requires an argument" << std::endl;
                        }
                        optopt = longopts[i].val;
                        return '?';
                    }
                } else if (longopts[i].has_arg == optional_argument) {
                    // Optional argument
                    optarg = eq ? const_cast<char*>(eq + 1) : nullptr;
                } else {
                    // No argument
                    optarg = nullptr;
                    if (eq) {
                        // Error: argument provided for no-argument option
                        if (opterr) {
                            std::cerr << argv[0] << ": option '--" << longopts[i].name 
                                     << "' doesn't allow an argument" << std::endl;
                        }
                        return '?';
                    }
                }
                
                // Handle flag setting
                if (longopts[i].flag) {
                    *(longopts[i].flag) = longopts[i].val;
                    return 0;
                } else {
                    return longopts[i].val;
                }
            }
        }
        
        // No matching long option found
        if (opterr) {
            std::cerr << argv[0] << ": unrecognized option '--" << name << "'" << std::endl;
        }
        optopt = 0;
        return '?';
    } else {
        // Short option - delegate to getopt()
        return getopt(argc, argv, optstring);
    }
}

#endif // GETOPT_IMPLEMENTATION

#else
// On non-Windows systems, use system getopt
#include <getopt.h>
#endif // _WIN32