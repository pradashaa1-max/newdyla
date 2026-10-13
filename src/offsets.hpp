
---

**Makefile**

```makefile
TARGET   = Astral
OUT      = out
SRC      = src/entry.mm src/player.cpp src/aimbot.cpp src/esp.cpp src/menu.cpp src/hooks.cpp
OBJ      = $(SRC:.mm=.o)
OBJ     := $(OBJ:.cpp=.o)

SDK      = $(shell xcrun --sdk iphoneos --show-sdk-path)
CC       = xcrun -sdk iphoneos clang++
CFLAGS   = -std=c++17 -fPIC -fobjc-arc -arch arm64 -isysroot $(SDK) \
           -I./src -O2 -fvisibility=hidden
LDFLAGS  = -shared -arch arm64 -isysroot $(SDK) \
           -framework UIKit -framework Foundation -framework Metal \
           -Wl,-undefined,dynamic_lookup

.PHONY: all clean

all: $(OUT)/$(TARGET).dylib

$(OUT)/$(TARGET).dylib: $(OBJ)
	@mkdir -p $(OUT)
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "built $@"

%.o: %.mm
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(OUT)/$(TARGET).dylib
