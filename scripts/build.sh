#!/bin/bash
echo "Starting build..."
echo "Staging firmware files..."
python stage_firmware.py
echo "Done staging firmware..."
echo "Building examples..."
mkdir ../build
(cd ../build; cmake ..; make -j )
