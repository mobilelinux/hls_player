# HLS Player

Simple HLS player application. Play live hls stream, UI is based on AWTK-MVVM.
Written by Google Antigravity.
Licensed under the BSD 3-Clause License.

## Build

### Linux

Run the helper script to install dependencies and build:

```bash
./scripts/build_linux.sh
```

### macOS

### macOS

1.  **Clone the repository:**

    ```bash
    git clone --recursive https://github.com/your-username/hls_player.git
    cd hls_player
    ```

    *If you have already cloned without recursive:*

    ```bash
    git submodule update --init --recursive
    ```

2.  **Install Nested Dependencies (AWTK-MVVM / JerryScript):**

    The `awtk-mvvm` submodule requires `jerryscript` (releases/10.2) to be installed manually if the recursive update does not handle it correctly:

    ```bash
    git clone https://github.com/jerryscript-project/jerryscript.git 3rd/awtk-mvvm/3rd/jerryscript/jerryscript
    cd 3rd/awtk-mvvm/3rd/jerryscript/jerryscript
    git reset --hard 3737a28eafd580a2bee2794e4f5edd0c0471a0c6
    cd -
    ```

3.  **Build using SCons:**

    ```bash
    export WITH_AWTK_SO=true
    scons -j8
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
