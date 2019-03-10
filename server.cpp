/*
 [Title]: server.cpp -- calculate the one way delays of the other VMs
 [Author]:Ioannis Tsiakkas (iantsiakkas@ics.forth.gr) -- @inspire_forth
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

#define exit_err(FILE, LINE, ERR) \
{ \
    fprintf(stderr, a_c_r "ERROR: %-70s" a_c_re " | [%-50s, %-5d]\n", ERR, FILE, LINE); \
    exit(EXIT_FAILURE); \
}

#define prnt_scs(FILE, LINE, MSG) \
{ \
    fprintf(stdout, "SUCCS: " a_c_g "%-70s" a_c_re " | [%-50s, %-5d]\n", MSG, FILE, LINE); \
}

#define prnt_inf(FILE, LINE, MSG) \
{ \
    fprintf(stdout, "MESSG: " a_c_y "%-70s" a_c_re " | [%-50s, %-5d]\n", MSG, FILE, LINE); \
}

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

    bool
    create_socket()
    {

    }
    
    bool
    file_to_server(char* flnm, SOCKET sckt)
    {
        Packet p;
        p.header = 1;
        p.footer = 0;
        int test;

        // Open
        cout << "Opening the file for Reading...\n";
        ifstream is (flnm, ios::in | ios::binary);

        // Check if error
        if ( !is )  {
            cout << "Error opening the file to read :/\n";
            return 0;
        }

        // Read and Write entire contents
        while ( is ) {
            // Read
            is.read(p.buffer, BUFFER_SIZE);
            //cout << "\nInput file pointer at: " << is.tellg() << "\nAmount read: " << is.gcount();
            
            // eof + last packet
            if (is.gcount() < BUFFER_SIZE)
                p.footer = is.gcount(); 
            
            test = send(sckt,(char*)&p,sizeof(Packet),0);
            //cout << test << endl;
            
        }

        // Close stuff
        cout << "closing read file.\n";
        is.close();
        return 1;
    }
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