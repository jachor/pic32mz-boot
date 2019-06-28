#include "lowlevel/usb_ep0.h"
#include <stddef.h>
#include "lowlevel/usb.h"
#include "lowlevel/ports.h"
#include "lowlevel/memory.h"
#include "lowlevel/printk.h"
#include "lowlevel/clock.h"

#define USBEP0_CSR0_RX_READY (1<<16)
#define USBEP0_CSR0_TX_READY (1<<17)
#define USBEP0_CSR0_STALL_SENT (1<<18)
#define USBEP0_CSR0_DATA_END (1<<19)
#define USBEP0_CSR0_SETUP_END (1<<20)
#define USBEP0_CSR0_SEND_STALL (1<<21)
#define USBEP0_CSR0_RX_READY_SERVICED (1<<22)
#define USBEP0_CSR0_SETUP_END_SERVICED (1<<23)
#define USBEP0_CSR0_FLUSH_FIFO (1<<24)

void usb_ep_read(void *dest, int ep, int len) {
  uint8_t *dest_ptr = (uint8_t *)dest;
  while(len-->0) {
    *dest_ptr = UsbController->USBEP_FIFO[ep].r8;
    dest_ptr ++;
  }
}

void usb_ep_write(const void *src, int ep, int len) {
  const uint8_t *src_ptr = (uint8_t *)src;
  for (int i=0; i<len; i++) {
    UsbController->USBEP_FIFO[ep].indexed8[i%4] = src_ptr[i];
  }
}

const char test_text[] =
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  "Co zozole w sobie maja ze je wszyscy zajadaja\r\n"
  ;

static struct {
  enum {
    EP0_WAITING_FOR_SETUP,
    EP0_RECEIVING_DATA,
    EP0_RECEIVING_ZLP,
    EP0_SENDING_DATA,
    EP0_SENDING_ZLP,
  } state;
  uint8_t *ptr;
  int bytes_left;
} _ep0_state;

void _ep0_reset() {
  _ep0_state.state = EP0_WAITING_FOR_SETUP;
  _ep0_state.bytes_left = 0;
  _ep0_state.ptr = NULL;

  if (UsbController->USBEP_CSR[0].CSR0 & (USBEP0_CSR0_RX_READY | USBEP0_CSR0_TX_READY)) {
    printk("usb: ep0: reset, fifo non-empty, flushing.\r\n");
    UsbController->USBEP_CSR[0].CSR0 = USBEP0_CSR0_FLUSH_FIFO;
  }
}

void _ep0_send_data(const void *ptr, int len) {
  _ep0_state.state = EP0_SENDING_DATA;
  _ep0_state.ptr = (void*) ptr;
  _ep0_state.bytes_left = len;

  UsbController->USBEP_CSR[0].CSR0 = 0
      | USBEP0_CSR0_RX_READY_SERVICED
      | 0;
}

void _usb_ep0_no_data() {
  _ep0_state.state = EP0_WAITING_FOR_SETUP;
  UsbController->USBEP_CSR[0].CSR0 = 0
      | USBEP0_CSR0_RX_READY_SERVICED
      | USBEP0_CSR0_DATA_END
      | 0;
}

void usb_ep0_reset() {
  printk("usb: device reset\r\n");
  usb_set_address(0);
  _ep0_reset();
}

static void _usb_ep0_send_stall() {
  UsbController->USBEP_CSR[0].CSR0 = USBEP0_CSR0_SEND_STALL;
}

static void _usb_ep0_read_setup(struct UsbSetupPacket *setup) {
  uint32_t *dest = (uint32_t*)setup;
  dest[0] = UsbController->USBEP_FIFO[0].r32;
  dest[1] = UsbController->USBEP_FIFO[0].r32;
}


static void _usb_handle_ep0_setup() {
  struct UsbSetupPacket setup;
  _usb_ep0_read_setup(&setup);

  printk("usb: ep0: got setup: bmRequestType=0x%02x, bRequest=%d\r\n", setup.bmRequestType, setup.bRequest);
  int is_device_to_host = !!(setup.bmRequestType & 0x80);
  int length = setup.wLength;

  switch(setup.bRequest) {
    case USB_SET_ADDRESS:
      //UsbController->USBEP_CSR[0].CSR0 = (1<<17) | (1<<19);
      _usb_ep0_no_data();
      printk("usb: got new address %d\r\n", setup.wValue);
      usb_set_address(setup.wValue);
      break;

    case USB_GET_DESCRIPTOR:
      // is_device_to_host -> true
      {
        int len; const void *ptr=NULL;
        usb_ep0_get_descriptor(&setup, &ptr, &len);
        if (setup.wLength < len) {
          len = setup.wLength;
        }

        if (ptr != NULL) {
          _ep0_send_data(ptr, len);
        } else {
          _usb_ep0_send_stall();
        }
      }
      break;

    case USB_SET_CONFIGURATION:
      {
        uint8_t configuration = (uint8_t) setup.wValue;
        printk("usb: ep0: set configuration %d\r\n", configuration);
        _usb_ep0_no_data();
      }
      break;

    case 100: // test
      {
        int len = sizeof(test_text);
        if (setup.wLength < len) {
          len = setup.wLength;
        }
        _ep0_send_data(test_text, len);
      }
      break;

    default:
      _usb_ep0_send_stall();
      break;
  }
}

int usb_ep0_interrupt() {
  uint32_t epcsr0 = UsbController->USBEP_CSR[0].CSR0;
  //printk("usb: ep0 csr0=%08x\r\n", epcsr0);

  if (epcsr0 & USBEP0_CSR0_STALL_SENT) {
    // Stall sent
    // TODO: is it needed?
    UsbController->USBEP_CSR[0].CSR0 = 0;
  }

  if (epcsr0 & USBEP0_CSR0_SETUP_END) {
    // Setup ended, reset state.
    printk("usb: ep0: SETUP ENDED\r\n");
    _ep0_reset();
    UsbController->USBEP_CSR[0].CSR0 = USBEP0_CSR0_SETUP_END_SERVICED;
  }
/*
  if (epcsr0 & (1<<17)) {
    // tx packet ready
    _ep0_push_data();
  }*/

  if (epcsr0 & USBEP0_CSR0_RX_READY) {
    // rx packet ready
    int rx_count = UsbController->USBEP_CSR[0].CSR2 & 0x7f;
    printk("usb: ep0: got %d bytes (state=%d)\r\n", rx_count, _ep0_state.state);

    if (_ep0_state.state == EP0_RECEIVING_ZLP && rx_count>0) {
      printk("usb: ep0: got non-zero length packet, assuming SETUP.\r\n");
      _ep0_state.state = EP0_WAITING_FOR_SETUP;
    }

    switch (_ep0_state.state) {
      case EP0_SENDING_DATA:
      case EP0_SENDING_ZLP:
        printk("usb: ep0: unexpected packet, resetting\r\n");
        _ep0_state.state = EP0_WAITING_FOR_SETUP;
      case EP0_WAITING_FOR_SETUP:
        _usb_handle_ep0_setup();
        break;

      case EP0_RECEIVING_ZLP: // XXX: not used?
        printk("usb: ep0: receiving ZLP\r\n");
        // receive ZLP.
        UsbController->USBEP_CSR[0].CSR0 = 0
            //| USBEP0_CSR0_DATA_END
            | USBEP0_CSR0_RX_READY_SERVICED
            | 0;
        _ep0_state.state = EP0_WAITING_FOR_SETUP;
        break;
      case EP0_RECEIVING_DATA:
        printk("usb: ep0: receiving data (unimplemented)\r\n");
        // receive data.
        break;
    }
  }

  while (1) {
    epcsr0 = UsbController->USBEP_CSR[0].CSR0;
    //printk("usb: ep0: loop csr0=%08x\r\n", epcsr0);
    if (epcsr0 & USBEP0_CSR0_TX_READY) {
//      continue;
      return 1;
    }

    // got space in buffer.
    switch (_ep0_state.state) {
      case EP0_SENDING_DATA:
        {
          int len = _ep0_state.bytes_left;
          if (len > USB_EP0_MAX_PACKET) len = USB_EP0_MAX_PACKET;
          usb_ep_write(_ep0_state.ptr, 0, len);
          _ep0_state.ptr += len;
          _ep0_state.bytes_left -= len;

          char is_last = len < USB_EP0_MAX_PACKET;

          UsbController->USBEP_CSR[0].CSR0 = 0
              | (is_last ? USBEP0_CSR0_DATA_END : 0)
              | USBEP0_CSR0_TX_READY;

          printk("usb: ep0: sent %d bytes, is_last=%d, bytes_left=%d\r\n", len, is_last, _ep0_state.bytes_left);

          if (is_last) {
            _ep0_state.state = EP0_RECEIVING_ZLP;
          }
        }
        break;

      case EP0_SENDING_ZLP:
        printk("usb: ep0: sent ZLP\r\n");
        UsbController->USBEP_CSR[0].CSR0 = USBEP0_CSR0_DATA_END | USBEP0_CSR0_TX_READY;
        _ep0_state.state = EP0_WAITING_FOR_SETUP;
        break;

      default:
        return 0; // done.
    }
  }
}
