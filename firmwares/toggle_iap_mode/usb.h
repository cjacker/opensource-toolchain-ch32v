#ifndef USB_H
#define USB_H
int jtag_libusb_open(const uint16_t vids[], const uint16_t pids[],
		                     struct libusb_device_handle **out);
int jtag_libusb_set_configuration(struct libusb_device_handle *devh,
		                                  int configuration);
void jtag_libusb_close(struct libusb_device_handle *dev);
int pWriteData(libusb_device_handle *handle, int endpoint, unsigned char *buf, unsigned int *length);
int pReadData(libusb_device_handle *handle, int endpoint, unsigned char *buf, unsigned int *length);
#endif
