DESTDIR=

all:
	gcc -o switch_to_iap_mode switch_to_iap_mode.c usb.c `pkg-config --libs --cflags libusb-1.0`
	gcc -o quit_iap_mode quit_iap_mode.c usb.c `pkg-config --libs --cflags libusb-1.0`

clean:
	rm -f quit_iap_mode switch_to_iap_mode 
