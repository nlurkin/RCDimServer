include ../makefile_common

CLibSRC  = server commands helper
CLibOBJ  = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(CLibSRC)))
CServerSRC = mainCServer decoder
CServerOBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(CServerSRC)))

ifeq ($(WHICHXML),XMLPreProcessor)
	XMLHEADER = TestNodeProxy.h
else
	XMLHEADER = xmlconfig_TestNode.h
endif

all: RCDimC libcserver.so

RCDimC: libcserver.so ../RCDimC

libcserver.so: ../libcserver.so

../RCDimC: $(CServerOBJ) $(xmlproxyDIR)/$(XMLHEADER) $(xmlproxyOBJ)
	$(CC) -o $@ $(CServerOBJ) $(xmlproxyOBJ) $(CFLAGS) $(LIBSDIR) $(LIBS) -lcserver

../libcserver.so: $(CLibOBJ)
	$(CCC) $(SOFLAGS) -Wl,-soname,libcserver.so -o $@ $^ $(CFLAGS)

$(OBJDIR)/%.o: %.c %.h ../Version.h
	@mkdir -p $(OBJDIR)
	$(CCC) -o $@ -c $< $(CFLAGS) $(HDRDIR)

$(OBJDIR)/%CServer.o: main.c
	@mkdir -p $(OBJDIR)
	$(CCC) -o $@ -c $< $(CFLAGS) $(HDRDIR)
	
# Other Targets
clean:
	rm -f ../RCDimC
	rm -f ../libcserver.so

.PHONY: all clean dependents
.SECONDARY:
