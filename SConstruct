import os
import sys

APP_NAME = 'hls_player'

# AWTK Root
AWTK_ROOT = os.path.join(os.getcwd(), '3rd/awtk')
AWTK_MVVM_ROOT = os.path.join(os.getcwd(), '3rd/awtk-mvvm')

sys.path.insert(0, AWTK_ROOT)
import awtk_config as awtk

# Add AWTK-MVVM include path
awtk.CPPPATH.append(os.path.join(AWTK_MVVM_ROOT, 'src'))
awtk.CPPPATH.append(os.path.join(AWTK_MVVM_ROOT, 'include')) # If exists

# Add FFmpeg and SDL2
def pkg_config(pkg):
    if os.system(f'pkg-config --exists {pkg}') == 0:
        return {
            'CPPPATH': os.popen(f'pkg-config --cflags-only-I {pkg}').read().strip().replace('-I', '').split(),
            'LIBPATH': os.popen(f'pkg-config --libs-only-L {pkg}').read().strip().replace('-L', '').split(),
            'LIBS': os.popen(f'pkg-config --libs-only-l {pkg}').read().strip().replace('-l', '').split()
        }
    return {}

ffmpeg_info = pkg_config('libavformat libavcodec libavutil libswscale libswresample')
sdl2_info = pkg_config('sdl2')

def merge_info(info):
    if 'CPPPATH' in info:
        awtk.CPPPATH += info['CPPPATH']
    if 'LIBPATH' in info:
        awtk.LIBPATH += info['LIBPATH']
    if 'LIBS' in info:
        awtk.LIBS += info['LIBS']

merge_info(ffmpeg_info)
# SDL2 is handled by AWTK usually, but we add it if needed.

# Add AWTK and MVVM libs
awtk.LIBPATH.append(os.path.join(AWTK_ROOT, 'bin'))
awtk.LIBPATH.append(os.path.join(AWTK_MVVM_ROOT, 'bin'))

# Link against shared libs if available, or static.
# AWTK build produced libawtk.dylib and libmvvm.dylib
awtk.LIBS = ['awtk', 'mvvm'] + awtk.LIBS

# Sources
SOURCES = Glob('src/*.c') + Glob('src/model/*.c') + Glob('src/view_model/*.c') + Glob('src/view/*.c')

os.environ['BIN_DIR'] = 'bin'

# We need to define AWTK_MVVM to enable MVVM features if needed in headers
awtk.CCFLAGS += ' -DAWTK_MVVM'

print("CPPPATH:", awtk.CPPPATH)
if os.path.join(AWTK_ROOT, 'src') not in awtk.CPPPATH:
    awtk.CPPPATH.append(os.path.join(AWTK_ROOT, 'src'))
awtk.CPPPATH.append('res')

env = Environment(
    CCFLAGS = awtk.CCFLAGS,
    CPPPATH = awtk.CPPPATH,
    LIBPATH = awtk.LIBPATH,
    LIBS = awtk.LIBS,
    LINKFLAGS = awtk.LINKFLAGS,
)

# Build
env.Program(os.path.join('bin', APP_NAME), SOURCES)
