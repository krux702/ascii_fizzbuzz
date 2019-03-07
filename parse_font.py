#!/usr/bin/python3

import cv2
import numpy as np

img = cv2.imread('font2.png')
height, width, channels = img.shape
print ("  height: ", height)
print ("   width: ", width)
print ("channels: ", channels)

for char in range(0,55):
  print("  // font - '%d'" % char)
  for y in range(0,20):
    print("  ", end = "")
    for x in range(0,23):
      x_pos = (char * 24) + x
      px = img[y][x_pos]
      print('0x%02X' % px[0], end = ", ")
    print("")
  print("")
