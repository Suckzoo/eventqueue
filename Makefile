CXX+= -std=c++11
CXXFLAGS= -g -O0 -Wall
CXXFLAGS+= $(EXTRA_CXXFLAGS)

SRCS= $(wildcard *.cpp)
HEADERS= $(wildcard *.hpp)
OBJS= $(SRCS:.cpp=.o)

TARGET= eventqueue

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f eventqueue *.o
	
.PHONY: all clean