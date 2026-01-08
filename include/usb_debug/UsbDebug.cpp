//
// Created by Dhruv Sirohi on 9/23/25.
//
#include <iostream>
#include <libusb.h>

// Define Vendor ID, Product ID, endpoint address, and interface number
#define VENDOR_ID 0x1234
#define PRODUCT_ID 0x1234

#define ENDPOINT 0x81
#define INTERFACE 0

int main() {
    // 1. Initialize libusb context
    libusb_context *ctx = nullptr;
    libusb_device_handle *dev_handle = nullptr;
    int r = libusb_init(&ctx);
    if (r < 0) {
        std::cerr << "Failed to initialize libusb" << std::endl;
        return -1;
    }

    // 2. Open the USB device using Vendor ID and Product ID
    dev_handle = libusb_open_device_with_vid_pid(ctx, VENDOR_ID, PRODUCT_ID);
    if (!dev_handle) {
        std::cerr << "Failed to open USB device" << std::endl;
        libusb_exit(ctx);
        return -1;
    }

    // 3. Claim the device interface
    if (libusb_claim_interface(dev_handle, INTERFACE) < 0) {
        std::cerr << "Failed to claim interface" << std::endl;
        libusb_close(dev_handle);
        libusb_exit(ctx);
        return -1;
    }

    // 4. Create a buffer for incoming data

    // 5. Enter a loop:
    //    - Perform a bulk or isochronous transfer from the endpoint
    //    - Check how many bytes were transferred
    //    - If data received, process or log it
    //    - Handle timeouts or errors

    // 6. Release the interface

    // 7. Close the device handle

    // 8. Exit libusb and clean up

    return 0;
}
