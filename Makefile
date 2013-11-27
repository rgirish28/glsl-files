# This makefile is only for OSX (for now)

TARGET := shade

SOURCES := $(wildcard ./src/*.cpp)
HEADERS := $(wildcard ./src/*.h)

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	INCLUDE := -I./include/ -I/usr/X11/include -I./glm -DOSX
	LDFLAGS := -framework GLUT -framework OpenGL \
		-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
		-lGL -lGLU -lm -lstdc++ -lfreeimage -lGLEW
else
	INCLUDE := -I./include/ -I/usr/X11R6/include -I/sw/include \
		 -I/usr/sww/include -I/usr/sww/pkg/Mesa/include -I./glm
	LDFLAGS := -L./lib/nix -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib \
		-L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lGL -lglut -lGLU -lX11 \
		-lGLEW -lfreeimage
endif

CXX := g++
CXXFLAGS := -g -Wall -O3 -fmessage-length=0 -DGL_GLEXT_PROTOTYPES $(INCLUDE) $(MACROS)
#-----------------------------------------

OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS) $(HEADERS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	
default: $(TARGET)

clean:
	/bin/rm -f $(OBJECTS) $(TARGET)

