#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/3rd/awtk/bin:$(pwd)/3rd/awtk-mvvm/bin
./bin/hls_player
