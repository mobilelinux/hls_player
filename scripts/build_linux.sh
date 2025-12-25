#!/bin/bash

# Update package lists
sudo apt-get update

# Install dependencies
sudo apt-get install -y scons libsdl2-dev libsndio-dev libasound2-dev libgtk-3-dev libglib2.0-dev libxext-dev libx11-dev libavformat-dev libavcodec-dev libavutil-dev libswscale-dev libswresample-dev libreadline-dev

# Build AWTK
echo "Building AWTK..."
export WITH_AWTK_SO=true
scons -C 3rd/awtk

# Build AWTK-MVVM
echo "Building AWTK-MVVM..."
scons -C 3rd/awtk-mvvm

# Build HLS Player
echo "Building HLS Player..."
scons
