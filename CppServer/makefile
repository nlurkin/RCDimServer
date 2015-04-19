include ../makefile_common

CppLibSRC    = NA62DimCommands NA62DimServer
CppLibOBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(CppLibSRC)))
CppServerSRC = ConfigDecoder TestServer mainCppServer
CppServerOBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(CppServerSRC)))


ifeq ($(WHICHXML),XMLPreProcessor)
	XMLHEADER = TestNodeProxy.h
	XMLEXT = cc
	XMLHEADER_EXPR = %Proxy.h
else
	XMLHEADER = xmlconfig_TestNode.h
	XMLEXT = cpp
	XMLHEADER_EXPR = xmlconfig_%.h
endif


all: libcppserver.so RCDimCpp

RCDimCpp: ../RCDimCpp
libcppserver.so: ../libcppserver.so

# Tool invocations
../RCDimCpp: $(CppServerOBJ) $(xmlproxyDIR)/$(XMLHEADER) $(xmlproxyOBJ)
	$(CC) -o $@ $(CppServerOBJ) $(xmlproxyOBJ) $(CPPFLAGS) $(LIBSDIR) $(LIBS) -lcppserver 

$(OBJDIR)/%.o: %.cpp %.h ../Version.h
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CPPFLAGS) $(HDRDIR)

$(OBJDIR)/%Proxy.o: $(xmlproxyDIR)/%Proxy.$(XMLEXT) $(xmlproxyDIR)/$(XMLHEADER_EXPR)
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CPPFLAGS) $(HDRDIR)

../libcppserver.so: $(CppLibOBJ)
	$(CC) $(SOFLAGS) -Wl,-soname,libcppserver.so -o $@ $^ $(CPPFLAGS)

$(OBJDIR)/%CppServer.o: main.cpp
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CPPFLAGS) $(HDRDIR)


# Other Targets
clean:
	rm -f ../RCDimCpp
	rm -rf ../*.so

.PHONY: all clean dependents
.SECONDARY:
