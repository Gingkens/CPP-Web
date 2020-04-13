CXXFLAGS = -I../include -lmysqlpp
CONTROL = ../control 
DIRS = .
EXTRAS = ../src/HttpCookie.o ../src/Response.o

FILES = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
TARGET = $(patsubst %.cpp, %, $(FILES))
CONTROL_FILES = $(foreach dir, $(CONTROL), $(wildcard $(dir)/*.cpp))
OBJS = $(patsubst %.cpp, %.o, $(CONTROL_FILES))

all:$(TARGET)

$(TARGET):%:%.cpp $(OBJS)
	$(CXX)  $< $(OBJS) $(EXTRAS) $(CXXFLAGS) -o $@

$(OBJS):%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

test:
	echo $(TARGET)

clean:
	-$(RM) $(TARGET)
	-$(RM) $(OBJS)

