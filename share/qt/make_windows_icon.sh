#!/bin/bash
# create multiresolution windows icon
ICON_DST=../../src/qt/res/icons/mojocoin.ico

convert ../../src/qt/res/icons/mojocoin-16.png ../../src/qt/res/icons/mojocoin-32.png ../../src/qt/res/icons/mojocoin-48.png ${ICON_DST}
