#include "lowlevel/usb_ep0.h"
#include "lowlevel/printk.h"

static const struct UsbDescriptor_Device _device_descriptor = {
  .bLength = sizeof(struct UsbDescriptor_Device),
  .bDescriptorType = USB_DEVICE,
  .bcdUsb = 0x110,
  .bDeviceClass = 0xff,
  .bMaxPacketSize0 = USB_EP0_MAX_PACKET,
  .idVendor = 0x1234,
  .idProduct = 0xabcd,
  .iManufacturer = 51,
  .bNumConfigurations = 1,
};

static const struct UsbDescriptor_Configuration _config1_descriptor = {
  .bLength = sizeof(struct UsbDescriptor_Configuration),
  .bDescriptorType = USB_CONFIGURATION,
  .wTotalLength = sizeof(struct UsbDescriptor_Configuration),
  .bNumInterfaces = 0,
  .bConfigurationValue = 1,
  .bmAttributes = 0x80, // reserved
  .bMaxPower = 100/2,
};

#define _NUMARGS(...)  (sizeof((int[]){__VA_ARGS__})/sizeof(int))
#define _STRING_DESCRIPTOR(name, ...) \
    static const struct _descriptor_##name { \
      struct UsbDescriptor_String header; \
      uint16_t data[_NUMARGS(__VA_ARGS__)]; \
    } name = { \
      .header = {.bLength = sizeof(struct _descriptor_##name), .bDescriptorType = USB_STRING}, \
      .data = __VA_ARGS__, \
    };

_STRING_DESCRIPTOR(_string_langids, USB_LANGID_ENGLISH_US);
_STRING_DESCRIPTOR(_string_manufacturer, 'J', 'a', 'c', 'h', 'o', 'r');

#define _RETURN_DESCRIPTOR(descriptor) ({*out_ptr=&(descriptor); *len=sizeof((descriptor)); return;})
void usb_ep0_get_descriptor(const struct UsbSetupPacket *setup, const void **out_ptr, int *len) {
  uint8_t descriptor_type = setup->wValue >> 8;
  uint8_t descriptor_index = setup->wValue & 0xff;
  printk("usb: ep0: get_descriptor: type=%d, index=%d, len=%d\r\n", descriptor_type, descriptor_index, setup->wLength);

  switch (descriptor_type) {
    case USB_DEVICE:
      switch (descriptor_index) {
        case 0: _RETURN_DESCRIPTOR(_device_descriptor);
        default: break;
      }
      break;

    case USB_CONFIGURATION:
      switch (descriptor_index) {
        case 0:  _RETURN_DESCRIPTOR(_config1_descriptor);
        default: break;
      }
      break;

    case USB_STRING:
      switch (descriptor_index) {
        case 0:  _RETURN_DESCRIPTOR(_string_langids);
        case 51:  _RETURN_DESCRIPTOR(_string_manufacturer);
        default: break;
      }
      break;
  }

  *out_ptr = NULL;
  *len = 0;
}
