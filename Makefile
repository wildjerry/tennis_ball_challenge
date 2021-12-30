#version
MAJOR_VERSION=0
MINOR_VERSION=1
MICRO_VERSION=0
VERSION=$(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)

BUILD		:=	build
SOURCES		:=	source
INCLUDES	:=	include

#Target name
TARGET		:=	$(notdir $(CURDIR))

#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
CPPFILES	:=  $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))

# Object files
export OFILES	:=	 $(addprefix $(BUILD)/,$(CPPFILES:.cpp=.o))			


CPPFILES	:=   $(addprefix $(SOURCES)/,$(CPPFILES))


#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------


# Compiler
export LD	:=	$(CXX)

#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------
export INCLUDE	:=	$(foreach dir,$(INCLUDES), -I$(CURDIR)/$(dir)) \
					-I$(CURDIR)/$(BUILD)

# Flags
CPPFLAGS= -Wall -g -W -pedantic -ansi -std=c++0x -lutil

# Linking flags
INCLUDEPATH += -I/usr/include/opencv4

LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_calib3d -lopencv_imgcodecs -lopencv_videoio

LDFLAGS = $(LIBS)

$(BUILD)/%.o:$(SOURCES)/%.cpp
	@mkdir -p $(BUILD)
	@echo building $^ ...
	@$(LD) $(INCLUDEPATH) $(CPPFLAGS)  -c $^ -o $@ $(INCLUDE)

all: clean main

main:$(OFILES)
	@mkdir -p $(BUILD)
	@echo linking ...
	@$(LD) $(OFILES) $(CPPFLAGS) $(LDFLAGS) -o $(TARGET)
	@echo "$(TARGET)-$(VERSION)"

# Cleanup
clean:
	@echo clean ...
	@rm -rf $(BUILD)
	@rm -f $(TARGET)

run:
	@echo Running $(TARGET)
	@./$(TARGET)

install:
	@sudo mkdir -p /opt/$(TARGET)
	@sudo cp -f $(TARGET) /opt/$(TARGET)/$(TARGET)
	@sudo ln -sf /opt/$(TARGET)/$(TARGET) /usr/local/bin/$(TARGET)

uninstall:
	@sudo rm -rf /opt/$(TARGET)
	@sudo rm -f /usr/local/bin/$(TARGET)
