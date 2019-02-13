/*
 [Title]: server.c -- calculate the one way delays of the other VMs
 [Author]: Dimitris Mavrommatis (mavromat@ics.forth.gr) -- @inspire_forth
-------------------------------------------------------------------------------------------------------------
 [Details]: 
 A C program that connects to other VMs and requests for timestamps in order to calculate the one way delay.
 The results are then send to the CXP Controller which is going to save them and use them in order to path
 stich the lowest latency path when needed.
-------------------------------------------------------------------------------------------------------------
 [Warning]:
 This script comes as-is with no promise of functionality or accuracy. I did not write it to be efficient nor 
 secured. Feel free to change or improve it any way you see fit.
-------------------------------------------------------------------------------------------------------------   
 [Modification, Distribution, and Attribution]:
 You are free to modify and/or distribute this script as you wish.  I only ask that you maintain original
 author attribution.
*/

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

char *serverName;
double *delays;

typedef double elem_type ;

typedef struct ip_name
{
    char *name;
    char *ip;
    int id;
} ipname;

#define ELEM_SWAP(a,b) { register elem_type t=(a);(a)=(b);(b)=t; }

// server class for sending data
class server
{
    private:
        int socket;
        const void *buffer; 
        size_t length;
        int flags;
        
    public:
        int get_socket() { return socket; }
        const void *get_buffer() { return buffer; } 
        size_t get_length() { return length; }
        int get_flags() { return flags; }

}

// client class for recieving data
class client
{
    private:
        int socket;
        const void *buffer; 
        size_t length;
        int flags;
        
    public:
        int get_socket() { return socket; }
        const void *get_buffer() { return buffer; } 
        size_t get_length() { return length; }
        int get_flags() { return flags; }

}