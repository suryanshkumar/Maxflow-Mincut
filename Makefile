TARGET	= maxflow_example

CC = g++
CFLAGS = -c -O3 -DNDEBUG
LDFLAGS = 

BASE_DIR := /usr

INCLUDE_DIRS =	-I$(BASE_DIR)/include \

LIB_DIRS = -L$(BASE_DIR)



SOURCES =	src/maxflow.cpp  \
			src/graph.cpp  \
			src/example_maxflow.cpp

OBJECTS = $(SOURCES:%.cpp=%.o)
DEPS = $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(TARGET)

-include $(DEPS)
	
$(TARGET): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ 

%.o : %.cpp
	$(CC) $(CFLAGS) -MM -MF $(patsubst %.o,%.d,$@) $(INCLUDE_DIRS) $(INCLUDES) $<
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(INCLUDES) $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(DEPS)
	rm -f $(TARGET)
