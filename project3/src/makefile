# MacOS compiler
CC = g++

# Make it the C++ compiler too
CXX = $(CC)

# OSX include paths for MacPorts
CFLAGS = -Wc++11-extensions -std=c++17 `pkg-config --cflags opencv4` -I../include -DENABLE_PRECOMPILED_HEADERS=OFF


# Make these the C++ flags too
CXXFLAGS = $(CFLAGS)

# Library paths, update to wwhere your openCV libraries are stored
LDFLAGS = -L/opt/homebrew/opt/opencv/lib  # opencv libraries are here

# opencv libraries and MacOS frameworks
LDLIBS = -lopencv_core -lopencv_highgui -lopencv_video -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc -lopencv_objdetect -framework AVFoundation -framework CoreMedia -framework CoreVideo -framework CoreServices -framework CoreGraphics -framework AppKit -framework OpenCL


# Location to write the executables
BINDIR = ../bin

# rules for making various executables
recognition: 2D_object_recognition.o methods.o csv_util.o
	$(CC) $^ -o $(BINDIR)/$@ $(LDFLAGS) $(LDLIBS)

test: test_image.o filters.o
	$(CC) $^ -o $(BINDIR)/$@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f *.o *~ 