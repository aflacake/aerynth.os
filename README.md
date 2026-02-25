# Aerynth.os

## Contribution
### Install Toolchain
If there is no compiler yet:
```
pacman -S mingw-w64-x86_64-toolchain
```

### Install SDL2
In the MinGW64 terminal:
```
pacman -S mingw-w64-x86_64-SDL2
```

Install SDL2_image (PNG support):
```
pacman -S mingw-w64-x86_64-SDL2_image
```

Make sure the mingw-w64-x86_64-SDL2_ttf package is installed in MinGW-w64:
```
pacman -S mingw-w64-x86_64-SDL2_ttf
```
> Make sure the compile command has `-IC:/msys64/mingw64/include/SDL2`


### Check Installation
Run:
```
pkg-config --cflags --libs sdl2
```
If the flags (`-I`, `-L`, `-lSDL2`) appear -> SUCCESSFUL.

### Compile
```
g++ -std=c++17 \
src/main.cpp \
src/app/Application.cpp \
src/ui/SpeechBubble.cpp \
-IC:/msys64/mingw64/include/SDL2 \
-LC:/msys64/mingw64/lib \
-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf \
-o rynth.exe
```
> Run in the same shell (MinGW64)

#### IMPORTANT THING (A LOT OF PEOPLE GET WRONG HERE)
Don't compile in MSYS shell\
Compile in MinGW64 shell

If wrong shell -> linker error.