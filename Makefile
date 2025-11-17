# Name of the executable
TARGET = ecg_program

# Compiler
CXX = g++
CXXFLAGS = -std=c++11 -I/usr/local/include/wfdb

# Source files (relative paths from Makefile location)
SRCS = \
    main/main.cpp \
    signal_acquisition/ecg_get.cpp \
    signal_acquisition/ecg_filters.cpp \
    signal_segmentation/ecg_segment.cpp \
	signal_export/ecg_export.cpp

# Build rule: compile and link everything
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -lwfdb -o $(TARGET)

# Clean rule
clean:
	rm -f $(TARGET)
