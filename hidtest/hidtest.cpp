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
    struct hid_path_info *devices, *device;
    hid_device *handle;
    unsigned char buf[256];
    int i, j, k, res = 0;

#ifdef WIN32
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);
#endif
    
    if (hid_init())
        return -1;

    devices = hid_list_devices(0x04d8, 0xfcf0);

    for (i = 0; i < 5; i++) {
        printf("=================================================================\n");
        device = devices;

        while (device) {
            printf("Device: %s\n", device->path);

            handle = hid_open_path(device->path);

            for (j = 0; j < 5; j++) {
                buf[0] = 0;
                buf[1] = 0;
                buf[2] = j;
                buf[3] = 0;
                buf[4] = 1;
                buf[5] = 1;
                buf[6] = 3;

                hid_write(handle, buf, 7);

                res = hid_read(handle, buf, sizeof(buf));

                printf("Data read:\n   ");
                for (k = 0; k < res; k++)
                    printf("%02hhx ", buf[k]);
                printf("\n");
            }

            hid_close(handle);

            device = device->next;
            printf("\n");
        }
    }

    hid_free_list(devices);

//    devs = hid_enumerate(0x0, 0x0);
//    cur_dev = devs;	
//    while (cur_dev) {
//        printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
//        printf("\n");
//        printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
//        printf("  Product:      %ls\n", cur_dev->product_string);
//        printf("  Release:      %hx\n", cur_dev->release_number);
//        printf("  Interface:    %d\n",  cur_dev->interface_number);
//        printf("\n");
//        cur_dev = cur_dev->next;
//    }
//    hid_free_enumeration(devs);
//    
//    for (i = 0; i < 5; i++) {
//        printf("\n\n\n\nIteration: %d\n", i);
//        
//        // Open the device using the VID, PID
//        handle = hid_open(0x04d8, 0xfcf0, NULL);
//        if (!handle) {
//            printf("unable to open device\n");
//            return 1;
//        }
//        else {
//            printf("device opened!\n");
//        }
//
//        // Read the Manufacturer String
//        wstr[0] = 0x0000;
//        res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
//        if (res < 0)
//            printf("Unable to read manufacturer string\n");
//        printf("Manufacturer String: %ls\n", wstr);
//
//        // Read the Product String
//        wstr[0] = 0x0000;
//        res = hid_get_product_string(handle, wstr, MAX_STR);
//        if (res < 0)
//            printf("Unable to read product string\n");
//        printf("Product String: %ls\n", wstr);
//
//        hid_set_nonblocking(handle, 1);
//
//        // clear buffer
//        memset(buf, 0x00, sizeof(buf));
//
//        // Read Serial
//        // # USB Message Overhead
//        // - Report ID
//        buf[0] = REPORT_ID;
//        // - Message ID
//        buf[1] = 0x00;
//        buf[2] = 0x00;
//        // - packet index
//        buf[3] = 0x00;
//        // - packet count
//        buf[4] = 0x01;
//        // - data length
//        buf[5] = 0x01;
//        // # Data
//        // - read serial command
//        buf[6] = 0x03;
//
//        res = hid_write(handle, buf, 7);
//        if (res < 0) {
//            printf("Unable to write()\n");
//            printf("Error: %ls\n", hid_error(handle));
//        }
//
//        // clear buffer
//        memset(buf, 0x00, sizeof(buf));
//
//        res = 0;
//        while (res == 0) {
//            res = hid_read(handle, buf, sizeof(buf));
//            if (res == 0)
//                printf("waiting...\n");
//            if (res < 0)
//                printf("Unable to read()\n");
//#ifdef WIN32
//            Sleep(500);
//#else
//            usleep(500 * 1000);
//#endif
//        }         
//
//        printf("Data read:\n");
//        // Print out the returned buffer.
//        for (i = 0; i < res; i++)
//            printf("%02hhx ", buf[i]);
//        printf("\n");
//
//        // TODO: blocking read
//
//        hid_close(handle);
//        handle = NULL;
//    }

    /* Free static HIDAPI objects. */
    hid_exit();

#ifdef WIN32
    system("pause");
#endif

    return 0;
}
