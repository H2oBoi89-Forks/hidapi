/*******************************************************
 Windows HID simplification

 Alan Ott
 Signal 11 Software

 8/22/2009

 Copyright 2009
 
 This contents of this file may be used by anyone
 for any reason without any conditions and may be
 used as a starting point for your own applications
 which use HIDAPI.
********************************************************/

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include "hidapi.h"

// Headers needed for sleeping.
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

int main(int argc, char* argv[])
{
    int res;
    unsigned char buf[256];
    #define MAX_STR 255
    wchar_t wstr[MAX_STR];
    hid_device *handle;
    const byte REPORT_ID = 0;
    int i, j;

#ifdef WIN32
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);
#endif

    struct hid_device_info *devs, *cur_dev;
    
    if (hid_init())
        return -1;

    devs = hid_enumerate(0x0, 0x0);
    cur_dev = devs;	
    while (cur_dev) {
        printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
        printf("\n");
        printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
        printf("  Product:      %ls\n", cur_dev->product_string);
        printf("  Release:      %hx\n", cur_dev->release_number);
        printf("  Interface:    %d\n",  cur_dev->interface_number);
        printf("\n");
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);
    
    for (i = 0; i < 5; i++) {
        printf("\n\n\n\nIteration: %d\n", i);
        
        // Open the device using the VID, PID
        handle = hid_open(0x04d8, 0xfcf0, NULL);
        if (!handle) {
            printf("unable to open device\n");
            return 1;
        }
        else {
            printf("device opened!\n");
        }

        // Read the Manufacturer String
        wstr[0] = 0x0000;
        res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
        if (res < 0)
            printf("Unable to read manufacturer string\n");
        printf("Manufacturer String: %ls\n", wstr);

        // Read the Product String
        wstr[0] = 0x0000;
        res = hid_get_product_string(handle, wstr, MAX_STR);
        if (res < 0)
            printf("Unable to read product string\n");
        printf("Product String: %ls\n", wstr);

        // clear buffer
        memset(buf, 0x00, sizeof(buf));

        // Read Serial
        // # USB Message Overhead
        // - Report ID
        buf[0] = REPORT_ID;
        // - Message ID
        buf[1] = 0x00;
        buf[2] = 0x00;
        // - packet index
        buf[3] = 0x00;
        // - packet count
        buf[4] = 0x01;
        // - data length
        buf[5] = 0x01;
        // # Data
        // - read serial command
        buf[6] = 0x03;

        res = hid_write(handle, buf, 7);
        if (res < 0) {
            printf("Unable to write()\n");
            printf("Error: %ls\n", hid_error(handle));
        }

        // clear buffer
        memset(buf, 0x00, sizeof(buf));
                
        res = hid_read(handle, buf, sizeof(buf));
        if (res < 0) {
            printf("Unable to read()\n");
        }
        else {
            printf("Data read:\n");
            // Print out the returned buffer.
            for (j = 0; j < res; j++)
                printf("%02hhx ", buf[j]);
            printf("\n");
        }

        hid_close(handle);
        handle = NULL;
    }

    /* Free static HIDAPI objects. */
    hid_exit();

#ifdef WIN32
    system("pause");
#endif

    return 0;
}
