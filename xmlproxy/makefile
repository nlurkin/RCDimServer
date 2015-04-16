include ../makefile_common

xmlproxySRC = TestNodeProxy
xmlproxyOBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(xmlproxySRC)))

all: TestNodeProxy.h $(OBJDIR)/TestNodeProxy.o demo

TestNodeProxy.h:
	@echo "Doing XML"
	$(XMLPROXY)/xmlpp.py TestNode.h TestNode

$(OBJDIR)/%Proxy.o: %Proxy.cc %Proxy.h
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(HDRDIR)

demo: demoout.c TestNodeProxy.h $(OBJDIR)/TestNodeProxy.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBSDIR) $(LIBS) $(HDRDIR)

# Other Targets
clean:
	rm -rf demo
	rm -rf TestNodeProxy.cc
	rm -rf TestNodeProxy.h
	rm -rf TestNode.hpp
	rm -rf ColdConfig.xml
	rm -rf WarmConfig.xml

.PHONY: all clean dependents
.SECONDARY:
