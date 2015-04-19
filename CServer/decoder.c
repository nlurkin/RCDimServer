/*
 * decoder.c
 *
 *  Created on: 31 Jan 2014
 *      Author: ncl
 */

#include "decoder.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef USE_XMLCONFIG
#include "xmlconfig_TestNode.h"
#else
#include "TestNodeProxy.h"
#endif

int parseFile(char *content, struct configStruct_t *s){
	/*char lines[10][STRING_MAX_LENGTH];

	int linesNb = tokenize(lines, content, "\n");
	int i;
	for(i=0; i<linesNb; i++){
		decodeLine(lines[i]);
	}*/
	#ifdef USE_XMLCONFIG
	inxmlfile_TestNode(s->t,"toto", content);
	return 0;
	#else
	printf("%i\n", s->t->version);
	if(xml_read_file_TestNode(content)==-1) printf("Error: %s\n", xml_getLastFatalError_TestNode());
	else{
		int ret = xml_apply_TestNode(s->t);
		printf("%i\n", s->t->version);
		return ret>0;
	}
	return -1;
	#endif
}

int writeFile(char *content, struct configStruct_t *s){
	/*char lines[10][STRING_MAX_LENGTH];

	int linesNb = tokenize(lines, content, "\n");
	int i;
	for(i=0; i<linesNb; i++){
		decodeLine(lines[i]);
	}*/

	#ifdef USE_XMLCONFIG
	outxmlfile_TestNode((void*)s->t, "toto", content);
	#else
	xml_create_TestNode(s->t, content);
	#endif
	return 0;
}

/*void decodeLine(char *line){
	char tok[2][STRING_MAX_LENGTH];

	if(tokenize(tok, line, "=")!=2) return;
	if(strcmp(tok[0],"uselessInt")==0) uselessInt = atoi(tok[1]);
	if(strcmp(tok[0],"param")==0) param = atoi(tok[1]);
	if(strcmp(tok[0],"sourceID")==0) sourceID = strtol(tok[1], NULL, 16);
	if(strcmp(tok[0],"frequency")==0) frequency = atof(tok[1]);
	if(strcmp(tok[0],"uselessString")==0) strcpy(uselessString,tok[1]);
}*/
