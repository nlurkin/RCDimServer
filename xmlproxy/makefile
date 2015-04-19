include ../makefile_common

xmlproxySRC = TestNodeProxy
xmlproxyOBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(xmlproxySRC)))

ifeq ($(WHICHXML),XMLPreProcessor)
	XMLHEADER = TestNodeProxy.h
	XMLEXT = cc
	XMLHEADER_EXPR = %Proxy.h
else
	XMLHEADER = xmlconfig_TestNode.h
	XMLEXT = cpp
	XMLHEADER_EXPR = xmlconfig_%.h
endif

 
all: $(XMLHEADER) $(OBJDIR)/TestNodeProxy.o demo

$(XMLHEADER):
	@echo "Doing XML"
	$(XMLPROXY)/$(XMLSCRIPT) TestNode.h $(XMLOPTION)

$(OBJDIR)/%Proxy.o: %Proxy.$(XMLEXT) $(XMLHEADER_EXPR)
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CPPFLAGS) $(HDRDIR)

demo: demoout.c $(XMLHEADER) $(OBJDIR)/TestNodeProxy.o
	$(CC) -o $@ $^ $(CPPFLAGS) $(LIBSDIR) $(LIBS) $(HDRDIR)

# Other Targets
clean:
	rm -rf demo
	rm -rf TestNodeProxy.cc
	rm -rf TestNodeProxy.h
	rm -rf TestNodeProxy.cpp
	rm -rf xmlconfig_TestNode.h
	rm -rf TestNode.hpp
	rm -rf ColdConfig.xml
	rm -rf WarmConfig.xml

.PHONY: all clean dependents
.SECONDARY:
