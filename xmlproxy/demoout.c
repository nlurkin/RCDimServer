#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TestNode.h"
#ifdef USE_XMLCONFIG
#include "xmlconfig_TestNode.h"
#else
#include "TestNodeProxy.h"
#endif

int main(){	
	
    TestNode node;
    
    
    node.uselessInt = 5;
    node.param = 10;
    node.sourceID = 0x11;
    node.frequency = 1000.2;
    strcpy(node.uselessString,"toto");

    node.version=TestNode_VERSION;
    
	#ifdef USE_XMLCONFIG
    OUTXMLFILE_TestNode(node, "ColdConfig.xml");  //Using xmlconfig
	#else
    xml_create_TestNode(&node, "ColdConfig.xml");  //Using XMLPreProcessor
	#endif

    node.param=15;
    node.frequency=2000.2;

	#ifdef USE_XMLCONFIG
    OUTXMLFILE_TestNode(node, "WarmConfig.xml");  //Using xmlconfig
	#else
    xml_create_TestNode(&node, "WarmConfig.xml");  //Using XMLPreProcessor
	#endif

	
    return 0;
}
