#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <string.h>
#include <libusb.h>
#include "errorcode.h"
#include "usb.h"

//4348:55e0 WinChipHead
static const uint16_t iap_vids[] = {0x4348, 0};
static const uint16_t iap_pids[] = {0x55e0, 0};
struct libusb_device_handle *iap_handle = NULL;

int main(void)
{
  unsigned char txbuf[6];
  unsigned char rxbuf[20];
  unsigned int len = 5;
  int ret;

  if (jtag_libusb_open(iap_vids, iap_pids, &iap_handle) != ERROR_OK)
  {
    printf("open iap device failed\n");
    goto end;
  }

  if(libusb_kernel_driver_active(iap_handle, 0) == 1)
  {
    if(libusb_detach_kernel_driver(iap_handle, 0) != 0)
      printf("Couldn't detach kernel driver!\n");
  }

  ret = libusb_claim_interface(iap_handle, 0);
  if (ret !=  LIBUSB_SUCCESS) {
    printf("libusb_claim_interface error: %s", libusb_error_name(ret));
    goto end;
  }

  sleep(1);

  // Quit IAP mode 
  txbuf[0] = 0x83;
  txbuf[1] = 0x02;
  txbuf[2] = 0x00;
  txbuf[3] = 0x00;
  len = 4;
  ret = pWriteData(iap_handle, 2, txbuf, &len);
  if(ret != ERROR_OK) {
    printf("write something failed\n");
    goto end;
  }

  printf("Quit IAP mode successfully\n");

 end:
  if(iap_handle) 
    jtag_libusb_close(iap_handle); 
  return 0;
}

