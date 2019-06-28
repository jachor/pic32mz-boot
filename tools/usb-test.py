#!/usr/bin/env python
from __future__ import print_function
import usb, usb.core
import time

def main():
  dev = usb.core.find(idVendor=0x1234, idProduct=0xabcd)
  if dev is None:
    print("Device not found.")
    return
#  res = dev.ctrl_transfer(0x00, 100, data_or_wLength=[0xaa]*10)

  s_time = time.time()
  total_len = 0
  for i in xrange(100):
    res = dev.ctrl_transfer(0x80, 100, data_or_wLength=4000)
    print(len(res))
    total_len += len(res)
  e_time = time.time()
  time_delta = e_time - s_time
  print ("took {:.2f} s, {:.1f} bytes/sec".format(time_delta, total_len*1.0/time_delta))
  #print(type(res))
  #print(res)
  #print(res.tostring().decode('utf-8'))

if __name__ == '__main__':
  main()
