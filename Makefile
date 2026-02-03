# Name of the executable
TARGET = ecg_program

# Compiler
CXX = g++
CXXFLAGS = -std=c++11 -I/usr/local/include/wfdb

# Source files (relative paths from Makefile location)
SRCS = \
    main/live.cpp \
    signal_acquisition/ecg_get.cpp \
    offline_process/ecg_filters.cpp \
    offline_process/ecg_ht.cpp \
	signal_export/ecg_export.cpp \
	feature_extraction/ecg_features.cpp \
	signal_export/feature_export.cpp

# Build rule: compile and link everything
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -lwfdb -o $(TARGET)

# Build and run offline program
run: $(TARGET)
	./$(TARGET)

# Clean rule
clean:
	rm -f $(TARGET)
