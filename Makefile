TARGET := bin/app

SOURCES := $(shell find src -type f)
# SOURCES_EXTERN := $(shell find external -type f -name *.cpp)

OBJECTS := $(patsubst src/%, build/%, $(SOURCES:.cpp=.o))
# OBJECTS_EXTERN := $(patsubst external/%, build/%, $(SOURCES_EXTERN:.cpp=.o))
# OBJECTS += $(OBJECTS_EXTERN)

DEPS := $(OBJECTS:.o=.d)

INC := -I external -I include

$(TARGET): $(OBJECTS)
	@mkdir -p bin
	g++ $^ -o $(TARGET) -pthread -lssl -lcrypto

build/%.o: src/%.cpp
	@mkdir -p $(shell dirname $@)
	g++ -MMD -c $< -o $@ $(INC)

# build/%.o: external/%.cpp
# 	@mkdir -p $(shell dirname $@)
# 	g++ -MMD -c $< -o $@

clean:
	rm -rf build bin .cache

.PHONY: clean

-include $(DEPS)
