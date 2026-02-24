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
-lmingw32 -lSDL2main -lSDL2 -lSDL2_image \
-o rynth.exe
```
> Run in the same shell (MinGW64)

#### IMPORTANT THING (A LOT OF PEOPLE GET WRONG HERE)
Don't compile in MSYS shell\
Compile in MinGW64 shell

If wrong shell -> linker error.