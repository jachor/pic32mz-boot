#ifndef _lowlevel_usb_ep0_h__
#define _lowlevel_usb_ep0_h__

#include <stddef.h>
#include <stdint.h>
#include "lowlevel/memory.h"

#define USB_EP0_MAX_PACKET (8*8)

PACKED_STRUCT
struct UsbSetupPacket {
  uint8_t bmRequestType;
  uint8_t bRequest;
  uint16_t wValue;
  uint16_t wIndex;
  uint16_t wLength;
};

enum UsbSetupRequest {
  USB_GET_STATUS = 0,
  USB_CLEAR_FEATURE = 1,
  USB_SET_FEATURE = 3,
  USB_SET_ADDRESS = 5,
  USB_GET_DESCRIPTOR = 6,
  USB_SET_DESCRIPTOR = 7,
  USB_GET_CONFIGURATION = 8,
  USB_SET_CONFIGURATION = 9,
  USB_GET_INTERFACE = 10,
  USB_SET_INTERFACE = 11,
  USB_SYNCH_FRAME = 12,
};

enum UsbDescriptorType {
  USB_DEVICE = 1,
  USB_CONFIGURATION = 2,
  USB_STRING = 3,
  USB_INTERFACE = 4,
  USB_ENDPOINT = 5,
};

enum UsbTransferType {
  USB_CONTROL = 0,
  USB_ISOCHRONOUS = 1,
  USB_BULK = 2,
  USB_INTERRUPT = 3,
};

PACKED_STRUCT
struct UsbDescriptor_Device {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint16_t bcdUsb;
  uint8_t bDeviceClass;
  uint8_t bDeviceSubClass;
  uint8_t bDeviceProtocol;
  uint8_t bMaxPacketSize0;
  uint16_t idVendor;
  uint16_t idProduct;
  uint16_t bcdDevice;
  uint8_t iManufacturer;
  uint8_t iProduct;
  uint8_t iSerialNumber;
  uint8_t bNumConfigurations;
};

PACKED_STRUCT
struct UsbDescriptor_Configuration {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint16_t wTotalLength;
  uint8_t bNumInterfaces;
  uint8_t bConfigurationValue;
  uint8_t iConfiguration;
  uint8_t bmAttributes;
  uint8_t bMaxPower;
};

PACKED_STRUCT
struct UsbDescriptor_Interface {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bInterfaceNumber;
  uint8_t bAlternateSetting;
  uint8_t bNumEndpoints;
  uint8_t bInterfaceClass;
  uint8_t bInterfaceSubClass;
  uint8_t bInterfaceProtocol;
  uint8_t iInterface;
};

PACKED_STRUCT
struct UsbDescriptor_Endpoint {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bEndpointAddress;
  uint8_t bmAttributes;
  uint16_t wMaxPacketSize;
  uint8_t bInterval;
};

PACKED_STRUCT
struct UsbDescriptor_String {
  uint8_t bLength;
  uint8_t bDescriptorType;
};

#define USB_LANGID_ENGLISH_US (0x0409)

extern void usb_ep0_get_descriptor(const struct UsbSetupPacket *setup, const void **out_ptr, int *len);

#endif

