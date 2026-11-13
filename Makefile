CC := gcc
CFLAGS := -Wall -Wextra -g -O0 -MMD -MP 
CPPFLAGS := -Iinclude -Ivendor/glfw/include -Ivendor/vulkan/x86_64/include
LDFLAGS := -Lvendor/glfw/lib64 -Lvendor/vulkan/x86_64/lib/VulkanLoader/lib
LDLIBS := -lglfw3 -lvulkan -lm	

BUIDIR := build
TARGET := $(BUIDIR)/render

SRCDIR := src
SRCS := $(wildcard $(SRCDIR)/*.c)

OBJDIR := $(BUIDIR)/objdir
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

.PHONY: clean all

all: $(TARGET)
-include $(OBJS:.o=.d)

$(TARGET) : $(OBJS) | $(BUIDIR)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(OBJDIR)/%.o : $(SRCDIR)/%.c | $(OBJDIR)
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(OBJDIR):
	mkdir -p $@ 
$(BUIDIR):
	mkdir -p $@
clean:
	rm -rf $(OBJDIR) $(TARGET) $(BUIDIR)

