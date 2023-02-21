CC := clang 
CFLAGS := -Iassets/
CFILES := src/*.c lib/*.c -Ilib/ -Isrc/
COUT := bin/smol
SHDCFLAGS := glsl100

ifeq ($(OS),Windows_NT)
#	Clang has some automatic stuff for this, 
#	Might check it out later

	CC = x86_64-w64-mingw32-gcc
	CFLAGS += -mwin32 -lkernel32 -luser32 -lshell32 -lole32 -lgdi32 -DSMOL_OS='"Windows"'
	COUT := $(COUT).exe
	ifdef WINDOWS_FORCE_GL
		SHDCFLAGS := $(SHDCFLAGS):glsl330
		CFLAGS += -DSOKOL_GLCORE33
	else
		SHDCFLAGS := $(SHDCFLAGS):hlsl5
		CFLAGS += -ld3d11 -ldxgi -DSOKOL_D3D11 
	endif

else
	UNAME_S := $(shell uname -s)
	CFLAGS += -DLOG_USE_COLOR -lGL -ldl -lm -pthread -DSOKOL_GLES2 -DSOKOL_FORCE_EGL

	ifeq ($(UNAME_S),Linux)
		CFLAGS += -DSMOL_OS='"Linux"' -lEGL -lasound -lxkbcommon -lX11 -lXi -lXcursor 

	endif

#	No Apple yet, I am sorry
	ifeq ($(UNAME_S),Darwin) 
		SHDCFLAGS := $(SHDCFLAGS):metal_macos
		CFLAGS += -DSOKOL_METAL -DSMOL_OS='"MacOS/iOS"'
	endif
endif

ifdef NO_SSE 
	CFLAGS += -DHANDMADE_MATH_NO_SSE -DSTBI_NO_SIMD
else
	CFLAGS += -ftree-vectorize -ffast-math -flto
endif

# TODO: Handle SMOL_OS from C.

shaders:
	sokol-shdc --input src/shader.glsl --output src/shader.h --slang $(SHDCFLAGS)

pack-demo:
	tar -czvf demo.smol demo/* 

demo: shaders
	tcc $(CFILES) $(CFLAGS) -DSMOL_DEBUG -DMEMBRO_ACTIVATE -DSTBI_NO_SIMD -run demo/

debug: shaders
	$(CC) $(CFILES) $(CFLAGS) -DSMOL_DEBUG -DMEMBRO_ACTIVATE -ggdb -o $(COUT)

build: shaders
	$(CC) $(CFILES) $(CFLAGS) -Os -o $(COUT)
	strip -s $(COUT)
	upx $(COUT)

build-wasm: shaders
	rm -rf bin/web
	cp -r web/ bin/
	emcc $(CFILES) -Ilib/ -Isrc/ -Iassets/ -ffast-math -DSOKOL_GLES2 -DSMOL_OS='"Web"' -O3 -o bin/web/smol.js

serve-web: build-wasm
	python3 -m http.server -d bin/web
