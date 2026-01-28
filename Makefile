CXX = clang++
CXXFLAGS = -std=c++17 `wx-config --cxxflags`
LDFLAGS = `wx-config --libs`

TARGET = filemanager
OBJS = main.o MainFrame.o FileOp.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

main.o: main.cpp MainFrame.h
	$(CXX) $(CXXFLAGS) -c main.cpp

MainFrame.o: MainFrame.cpp MainFrame.h
	$(CXX) $(CXXFLAGS) -c MainFrame.cpp

clean:
	rm -f $(TARGET) *.o
