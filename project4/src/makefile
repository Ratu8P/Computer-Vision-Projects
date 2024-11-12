# Haobo Zhang
# 11/01/2024
# CS 5330
# Project 4

# MacOS compiler
CC = g++

# Make it the C++ compiler too
CXX = $(CC)

# OSX include paths for MacPorts and Homebrew
CFLAGS = -Wc++11-extensions -std=c++17 `pkg-config --cflags opencv4` -I../include -I/opt/homebrew/include -DENABLE_PRECOMPILED_HEADERS=OFF

# Make these the C++ flags too
CXXFLAGS = $(CFLAGS)

# Library paths, update to where your libraries are stored
LDFLAGS = -L/opt/homebrew/opt/opencv/lib -L/opt/homebrew/lib  # opencv and glew libraries

# opencv libraries, GLEW, GLFW, and MacOS frameworks
LDLIBS = -lopencv_core -lopencv_highgui -lopencv_video -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc -lopencv_objdetect -lopencv_calib3d -lGLEW -lglfw -framework OpenGL -framework AVFoundation -framework CoreMedia -framework CoreVideo -framework CoreServices -framework CoreGraphics -framework AppKit -framework OpenCL

# Location to write the executables
BINDIR = ../bin

# rules for making various executables
ar: ar.o methods.o
	$(CC) $^ -o $(BINDIR)/$@ $(LDFLAGS) $(LDLIBS)

harris: harris_corners.o
	$(CC) $^ -o $(BINDIR)/$@ $(LDFLAGS) $(LDLIBS)

ar_video: ar_video.o methods.o
	$(CC) $^ -o $(BINDIR)/$@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f *.o *~
