# To compile
# make TARGET_LINUX=1 (Linux)
# make TARGET_WINDOWS=1 (Windows)

CPP = g++

BIN = last-outpost.exe

CPPFLAGS = -std=c++23 -Wall -g -I./include -I./include/imgui -I../my-lib/include
LDFLAGS = -std=c++23

# ----------------------------------

ifdef TARGET_LINUX
	CPPFLAGS +=
	LDFLAGS += -lm

	CPPFLAGS += `pkg-config --cflags sdl2 SDL2_mixer SDL2_image SDL2_ttf SDL2_gfx`
	LDFLAGS += `pkg-config --libs sdl2 SDL2_mixer SDL2_image`

	BIN = last-outpost
endif

ifdef TARGET_WINDOWS
	CPPFLAGS += -I"C:/Program Files/msys64/ucrt64/include/SDL2"
	LDFLAGS += -L"C:/Program Files/msys64/ucrt64/lib" -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -lSDL2_gfx -mwindows
endif

# ----------------------------------

SRCS := $(wildcard src/*.cpp) $(wildcard src/imgui/*.cpp)

HEADERS := $(wildcard include/last-outpost/*.h)

OBJS := $(patsubst %.cpp,%.o,$(SRCS))

# ----------------------------------

%.o: %.cpp $(HEADERS)
	$(CPP) $(CPPFLAGS) -c -o $@ $<

all: $(BIN)
	@echo "Everything compiled! yes!"

$(BIN): $(OBJS)
	$(CPP) -o $(BIN) $(OBJS) $(LDFLAGS)

# ----------------------------------

clean:
	- rm -rf $(BIN) $(OBJS)
