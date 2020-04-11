CXXFLAGS = -lpthread -Iinclude
DIRS = src
VPATH = src:bin:.
TARGET = server
SESSION = session.o
CGI_BIN = ./cgi-bin

FILES = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
OBJS = $(patsubst %.cpp, %.o, $(FILES))

$(TARGET):$(OBJS)
	$(CXX) -o $(TARGET) $(DIRS)/*.o $(CXXFLAGS)
	cp $(DIRS)/$(SESSION) $(CGI_BIN)
	$(MAKE) -C $(CGI_BIN)

$(OBJS):%.o:%.cpp 
	$(CXX) -o $@ -c $< $(CXXFLAGS)

test:
	echo $(OBJS)

clean:
	-$(RM) $(TARGET)
	-$(RM) $(DIRS)/*.o

