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

static const uint16_t wlink_vids[] = {0x1a86, 0};
static const uint16_t wlink_pids[] = {0x8010, 0};
struct libusb_device_handle *wlink_handle = NULL;

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

  if (jtag_libusb_open(wlink_vids, wlink_pids, &wlink_handle) != ERROR_OK)
  {
    printf("open wlink device failed\n");
    goto end;
  }

  jtag_libusb_set_configuration(wlink_handle, 0);

  if (libusb_claim_interface(wlink_handle, 0) != ERROR_OK)
  {
    printf("claim wlink interface failed\n");
    goto end;
  }

  // read adatper type and firmware version
  txbuf[0] = 0x81;
  txbuf[1] = 0x0d;
  txbuf[2] = 0x01;
  txbuf[3] = 0x01;
  len = 4;
  pWriteData(wlink_handle, 1, txbuf, &len);
  len = 7;
  pReadData(wlink_handle, 1, rxbuf, &len);
  printf("adapter type:%x, version:%d.%d\n", rxbuf[5], rxbuf[3], rxbuf[4]);

  // according to adapter type, find correct firmware
  // and compare version info with wchlink.wcfg
  // for adapter type 0x02 and 0x12, it's WCH-LinkE: FIRMWARE_CH32V203.BIN  
  
  // get IAP type:0x02010f81 len 4
  txbuf[0] = 0x81;
  txbuf[1] = 0x0f;
  txbuf[2] = 0x01;
  txbuf[3] = 0x02;
  len = 4;
  ret = pWriteData(wlink_handle, 1, txbuf, &len);
  if(ret != ERROR_OK) {
    printf("get IAP type failed\n");
    goto end;
  }
  len = 5;
  ret = pReadData(wlink_handle, 1, rxbuf, &len);
  if(ret != ERROR_OK) {
     printf("read IAP type failed\n");
     goto end;
  }
  // read IAP type: 82, f, 1, 2, f 
  // printf("read IAP type: %x, %x, %x, %x, %x\n",rxbuf[0], rxbuf[1], rxbuf[2], rxbuf[3], rxbuf[4]);
  
  // set it to IAP mode: 0x01010f81 len 4
  txbuf[0] = 0x81;
  txbuf[1] = 0x0f;
  txbuf[2] = 0x01;
  txbuf[3] = 0x01;
  len = 4;
  ret = pWriteData(wlink_handle, 1, txbuf, &len);

  if(ret != ERROR_OK) {
    printf("set IAP mode failed\n");
    goto end;
  } 

  sleep(1);

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
  
  printf("Enter IAP mode successfully!\n");


#if 0 
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

  printf("quit IAP mode successfully\n");
#endif

 end:
  //if(wlink_handle) 
  //  jtag_libusb_close(wlink_handle); 
  if(iap_handle) 
    jtag_libusb_close(iap_handle); 
  return 0;
}
