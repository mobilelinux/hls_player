# HLS Player

Simple HLS player application. Play live hls stream, UI is based on AWTK-MVVM.
Written by Google Antigravity.

## Build

### Linux

Run the helper script to install dependencies and build:

```bash
./scripts/build_linux.sh
```

### macOS

Build using SCons:

```bash
scons
```

## Run

### Linux

Use the run script to ensure library paths are set correctly:

```bash
./scripts/run_linux.sh
```

### macOS

Run the binary directly:

```bash
./bin/hls_player
```
