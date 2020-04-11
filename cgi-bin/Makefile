CXXFLAGS = -I../include
DIRS = .

FILES = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
TARGET = $(patsubst %.cpp, %, $(FILES))
SESSION = session.o

$(TARGET):%:%.cpp
	$(CXX) -o $@ $< $(SESSION) $(CXXFLAGS)

test:
	echo $(TARGET)
