#!/bin/sh
wlink erase --method pin-rst --speed low --chip CH32V30X
wlink flash WCH-LinkE-APP-IAP.bin

