#include "json.h"

cJSON* parseJSON(char *filename) {
	FILE *fp;
	size_t fsize;
	char *fbuf;
	cJSON* json;

	
	if( (fp=fopen(filename, "rb")) == NULL ) {
		fprintf(stderr, "Error0: Couldn't open '%s' for reading\n", filename);
		return NULL;
	}
	if( fseek(fp,0,SEEK_END) == -1) {
		fprintf(stderr, "Error1: Couldn't determain filesize of '%s'\n", filename);
		return NULL;
	} else {
		/* Allocate memory for file */
		if( (fsize=ftell(fp)) != 255 )
			if( (fbuf = malloc(sizeof(char)*fsize)) == NULL ) {
				fprintf(stderr, "Error2: Couldn't allocate memory for '%s'\n", filename);
				return NULL;
			}

		rewind(fp);
	}
	if( fread(fbuf, sizeof(char), fsize, fp) != fsize ) {
		fprintf(stderr, "Error3: Couldn't copy '%s' to buffer\n", filename);
		return NULL;
	}

	fclose(fp);

	json=cJSON_Parse(fbuf);

	free(fbuf);

	return json;
}

unit *initUnit( cJSON* type, unitScope scope ){
	/* type is an array */
	unit *current = NULL;
	cJSON *tmp;
	tmp = type->child;
	if(strcmp(tmp->valuestring,"osc") ==0 ){
		current = addOsc(scope);
		tmp = tmp->next;
		if(strcmp(tmp->valuestring, "sin") ==0 )
			setOscType(current, otSIN);
		if(strcmp(tmp->valuestring, "rect") ==0 )
			setOscType(current, otREC);
		if(strcmp(tmp->valuestring, "tri") ==0 )
			setOscType(current, otTRI);
	}
	else if(strcmp(tmp->valuestring, "mixer") == 0){
		current = addMixer2ch(scope);
		if((tmp = tmp->next) !=NULL) 
			if(tmp->type == cJSON_Number)
				if(tmp->valueint != 2){
					printf("Right now we only have a 2ch Mixer implemented, sorry.\n");
					exit(-1);
				}
					
	}
	else if(strcmp(tmp->valuestring, "lowpass")==0){
		current = addFxLowpass(scope);
	}
	else if(strcmp(tmp->valuestring, "highpass")==0){
		current = addFxHighpass(scope);
	}
	else if(strcmp(tmp->valuestring, "bandpass") == 0){
		current = addFxBandpass(scope);
	}

	return current;
}

OptionSet** getAttrArray(){
	int i;
	OptionSet** os;
	if( (os = malloc(30 * sizeof( OptionSet *))) ==NULL ) return NULL;
	

	for( i=0 ;i<30; i++) if ( ( os[i] = malloc(sizeof(OptionSet))  ) ==NULL ) printf("malloc error in getAttrArray\n"), exit(-2) ;
	os[i =  0]->name=strdup("param1_mod"); os[i]->typ=_ANALOG; os[i]->pt=ptPARAM1; os[i]->po=poMOD; 
	os[i =  1]->name=strdup("param1_value"); os[i]->typ=_ANALOG; os[i]->pt=ptPARAM1; os[i]->po=poVAL; 
	os[i =  2]->name=strdup("param1_range");	os[i]->typ=_ANALOG; os[i]->pt=ptPARAM1; os[i]->po=poRANGE; 

	os[i =  3]->name=strdup("freq_mod"); os[i]->typ=_ANALOG; os[i]->pt=ptFREQ; os[i]->po=poMOD; 
	os[i =  4]->name=strdup("freq_value"); os[i]->typ=_ANALOG; os[i]->pt=ptFREQ; os[i]->po=poVAL; 
	os[i =  5]->name=strdup("freq_range"); os[i]->typ=_ANALOG; os[i]->pt=ptFREQ; os[i]->po=poRANGE; 

	os[i =  6]->name=strdup("vol_mod"); os[i]->typ=_ANALOG; os[i]->pt=ptVOL; os[i]->po=poMOD; 
	os[i =  7]->name=strdup("vol_value"); os[i]->typ=_ANALOG; os[i]->pt=ptVOL; os[i]->po=poVAL; 
	os[i =  8]->name=strdup("vol_range"); os[i]->typ=_ANALOG; os[i]->pt=ptVOL; os[i]->po=poRANGE; 

	os[i =  9]->name=strdup("input1_mod"); os[i]->typ=_ANALOG; os[i]->pt=ptINPUT1; os[i]->po=poMOD; 
	os[i = 10]->name=strdup("input1_value"); os[i]->typ=_ANALOG; os[i]->pt=ptINPUT1; os[i]->po=poVAL; 
	os[i = 11]->name=strdup("input1_range"); os[i]->typ=_ANALOG; os[i]->pt=ptINPUT1; os[i]->po=poRANGE; 

	os[i = 12]->name=strdup("input2_mod"); os[i]->typ=_ANALOG; os[i]->pt=ptINPUT2; os[i]->po=poMOD; 
	os[i = 13]->name=strdup("input2_value"); os[i]->typ=_ANALOG; os[i]->pt=ptINPUT2; os[i]->po=poVAL; 
	os[i = 14]->name=strdup("input2_range"); os[i]->typ=_ANALOG; os[i]->pt=ptINPUT2; os[i]->po=poRANGE; 

	os[i = 15]->name=strdup("vol1_mod"); os[i]->typ=_ANALOG; os[i]->pt=ptVOL1; os[i]->po=poMOD; 
	os[i = 16]->name=strdup("vol1_value"); os[i]->typ=_ANALOG; os[i]->pt=ptVOL1; os[i]->po=poVAL; 
	os[i = 17]->name=strdup("vol1_range"); os[i]->typ=_ANALOG; os[i]->pt=ptVOL1; os[i]->po=poRANGE; 

	os[i = 18]->name=strdup("vol2_mod"); os[i]->typ=_ANALOG; os[i]->pt=ptVOL2; os[i]->po=poMOD; 
	os[i = 19]->name=strdup("vol2_value"); os[i]->typ=_ANALOG; os[i]->pt=ptVOL2; os[i]->po=poVAL; 
	os[i = 20]->name=strdup("vol2_range"); os[i]->typ=_ANALOG; os[i]->pt=ptVOL2; os[i]->po=poRANGE; 

	os[i = 21]->name=strdup("cutoff_mod"); os[i]->typ=_ANALOG; os[i]->pt=ptCUTOFF; os[i]->po=poMOD; 
	os[i = 22]->name=strdup("cutoff_value"); os[i]->typ=_ANALOG; os[i]->pt=ptCUTOFF; os[i]->po=poVAL; 
	os[i = 23]->name=strdup("cutoff_range"); os[i]->typ=_ANALOG; os[i]->pt=ptCUTOFF; os[i]->po=poRANGE; 

	os[i = 24]->name=strdup("bandwidth_mod"); os[i]->typ=_ANALOG; os[i]->pt=ptBANDWIDTH; os[i]->po=poMOD; 
	os[i = 25]->name=strdup("bandwidth_value"); os[i]->typ=_ANALOG; os[i]->pt=ptBANDWIDTH; os[i]->po=poVAL; 
	os[i = 26]->name=strdup("bandwidth_range"); os[i]->typ=_ANALOG; os[i]->pt=ptBANDWIDTH; os[i]->po=poRANGE; 

	os[i = 27]->name=strdup("activity"); os[i]->typ=_DIGITAL; os[i]->bt=btACT; 
	os[i = 28]->name=strdup("activity1"); os[i]->typ=_DIGITAL; os[i]->bt=btACT1; 
	os[i = 29]->name=strdup("activity2"); os[i]->typ=_DIGITAL; os[i]->bt=btACT2; 

	return os;
}

int routing(char *conffile){
	cJSON *root, *config, *locals,*globals,*outputs, *tmp,*scndTmp;

	cJSON *type, *name, *parameters;
	int i, voices_count;
	
	ListElement *hashList[256];
	ListElement *attrList[256];
	ListElement *currentLE;

	unit *current;
	
	OptionSet** os;
	OptionSet* tmpOS;
	
	if( (os= getAttrArray()) ==NULL) printf("malloc error in getAttrArray\n"), exit(-2);
	for(i=0; i<256;  attrList[i] = hashList[i++] = NULL);
	for(i=0; i<30; i++) 
		 addToHashlist(os[i]->name,os[i],  attrList) ;
	
	root=config=locals=globals=NULL;

	root=parseJSON(conffile);
	if(root == NULL) printf(":(\n"), exit(-1);
	
	addToHashlist("voiceFreq", voiceFreq, hashList);
	addToHashlist("voiceVelocity", voiceVelocity, hashList);
	addToHashlist("voiceActive", voiceActive, hashList);

	addToHashlist("voicesOut", voicesOut, hashList);

	
	

	/* variablen fuer sections setzen */
	tmp=root->child;
	while(tmp!=NULL){
		if(strcmp(tmp->string, "config") ==0) config=tmp;
		if(strcmp(tmp->string, "locals") ==0) locals=tmp;
		if(strcmp(tmp->string, "globals") ==0) globals=tmp;
		if(strcmp(tmp->string, "outputs") ==0) outputs=tmp;
		tmp = tmp->next;
	}
	setupRouting();	
	tmp = config->child;
	while(tmp!=NULL){
		if(strcmp(tmp->string, "voices") ==0) break;
		tmp=tmp->next;
	}
		

	if(tmp==NULL) return -1;
	
	/** Anzahl der Voices setzen */
	voices_count = tmp->valueint;	
	setVoiceCount(voices_count);
	printf("Anzahl voices: %d\n", voices_count);

	/* alle lokalen units initialisieren.
	Ein durchgang dieser schleife entspricht einem Unit.*/
	tmp=locals->child;
	while(  tmp != NULL ){
		/* ich checke auf ==NULL, also schoen saubermachen */
		type=name=parameters=NULL;

		scndTmp=tmp->child;
		while(scndTmp!=NULL){
			if(	strcmp(scndTmp->string,	"type")					==	0) type 				=	scndTmp;
			if(	strcmp(scndTmp->string,	"name")					==	0) name 				=	scndTmp;
			if(	strcmp(scndTmp->string,	"parameters")		==	0) parameters 	=	scndTmp;
			scndTmp=scndTmp->next;
		}

		if ((current = initUnit(type, usLOCAL)) == NULL) {
			printf("Fehler beim initialisieren einer Unit.");
			exit(-2);
		}
		if(addToHashlist(name->valuestring, current, hashList) == NULL ){
			printf("Fehler beim hinzufuegen zur HashList");
			exit(-2);
		}

		scndTmp = (parameters == NULL? NULL:parameters->child);
		while(scndTmp!=NULL){
			if( scndTmp->type == cJSON_String ) {scndTmp=scndTmp->next;  continue;}

			if( (currentLE = getFromHashlist(scndTmp->string, attrList) ) == NULL) {
				printf("verstehe paramenter nicht: %s\n", scndTmp->string);
				exit(-2);
			}
			if( (tmpOS = (OptionSet * )currentLE->dataPtr) == NULL ){
				printf("DataPtr = NULL :( \n");
				exit(-2);
			}
			if(tmpOS->typ == _ANALOG){
				if( scndTmp->type != cJSON_Number ){
					 printf("\"Analoge\" Wert muessen Strings oder Zahlen sein!");
					 exit(-2);
				}
				setParam(current, tmpOS->pt, tmpOS->po, scndTmp->valuedouble);
			}
			if(tmpOS->typ == _DIGITAL){
				if( scndTmp->type == cJSON_True ){
					setBool(current, tmpOS->bt, 1);
					scndTmp=scndTmp->next;
					continue;
				}
				if( scndTmp->type == cJSON_False ){
					setBool(current, tmpOS->bt, 0);
					scndTmp=scndTmp->next;
					continue;
				}
				setBool(current, tmpOS->bt, scndTmp->valueint);
			}
			scndTmp=scndTmp->next;
		}
		tmp=tmp->next;
	}

	/* alle globalen units initialisieren.
	Ein durchgang dieser schleife entspricht einem Unit.*/
	tmp=globals->child;
	while(  tmp != NULL ){
		/* ich checke auf ==NULL, also schoen saubermachen */
		type=name=parameters=NULL;

		scndTmp=tmp->child;
		while(scndTmp!=NULL){
			if(	strcmp(scndTmp->string,	"type")					==	0) type 				=	scndTmp;
			if(	strcmp(scndTmp->string,	"name")					==	0) name 				=	scndTmp;
			if(	strcmp(scndTmp->string,	"parameters")		==	0) parameters 	=	scndTmp;
			scndTmp=scndTmp->next;
		}

		if ((current = initUnit(type, usGLOBAL)) == NULL) {
			printf("Fehler beim initialisieren einer Unit.");
			exit(-2);
		}
		if(addToHashlist(name->valuestring, current, hashList) == NULL ){
			printf("Fehler beim hinzufuegen zur HashList");
			exit(-2);
		}

		scndTmp = (parameters == NULL? NULL:parameters->child);
		while(scndTmp!=NULL){
			if( scndTmp->type == cJSON_String ) {scndTmp=scndTmp->next;  continue;}

			if( (currentLE = getFromHashlist(scndTmp->string, attrList) ) == NULL) {
				printf("verstehe paramenter nicht: %s\n", scndTmp->string);
				exit(-2);
			}
			if( (tmpOS = (OptionSet * )currentLE->dataPtr) == NULL ){
				printf("DataPtr = NULL :( \n");
				exit(-2);
			}
			if(tmpOS->typ == _ANALOG){
				if( scndTmp->type != cJSON_Number ){
					 printf("\"Analoge\" Wert muessen Strings oder Zahlen sein!");
					 exit(-2);
				}
				setParam(current, tmpOS->pt, tmpOS->po, scndTmp->valuedouble);
			}
			if(tmpOS->typ == _DIGITAL){
				if( scndTmp->type == cJSON_True ){
					setBool(current, tmpOS->bt, 1);
					scndTmp=scndTmp->next;
					continue;
				}
				if( scndTmp->type == cJSON_False ){
					setBool(current, tmpOS->bt, 0);
					scndTmp=scndTmp->next;
					continue;
				}
				setBool(current, tmpOS->bt, scndTmp->valueint);
			}
			scndTmp=scndTmp->next;
		}
		tmp=tmp->next;
	}

	/*routing setzen */
	tmp=locals->child;
	while(tmp!=NULL){
		/* ich checke auf ==NULL, also schoen saubermachen */
		type=name=parameters=NULL;

		scndTmp=(tmp == NULL? NULL:tmp->child);
		while(scndTmp!=NULL){
			if(strcmp(scndTmp->string,"type")==0) type = scndTmp;
			if(strcmp(scndTmp->string,"name")==0) name = scndTmp;
			if(strcmp(scndTmp->string,"parameters")==0) parameters = scndTmp;
			scndTmp=scndTmp->next;
		}

		if(name == NULL) printf("ueberall namen angeben\n"), exit(-2);
		if(name->valuestring == NULL) printf("namen muessen strings sein\n"), exit(-2);
		/* get "current" from hashlist start*/
		if( (currentLE = getFromHashlist(name->valuestring,hashList))==NULL){
			printf("Hab in der Hashtable gesucht aber nichts gefunden :(\n");
			exit(-2);
		}
		if(( current = (unit*) currentLE->dataPtr) == NULL){
			printf("Habe in der HT gefunden, aber dataPtr ist NULL, Motherfucka!\n");
			exit(-2);
		}
		/* get "current" from hashlist end*/

		scndTmp = (parameters == NULL? NULL:parameters->child);
		while(scndTmp!=NULL){
			/* if it's a string, it's the routing's job */
			if( scndTmp->type != cJSON_String ) {scndTmp=scndTmp->next;  continue;} 

			/* set tmp variables start */
			if( (currentLE = getFromHashlist(scndTmp->string, attrList) ) == NULL) {
				printf("verstehe paramenter im routing nicht: '%s'\n", scndTmp->string);
				exit(-2);
			}
			if( (tmpOS = (OptionSet * )currentLE->dataPtr) == NULL ){
				printf("DataPtr = NULL :(  '''This shouldn't happen!'''\n");
				exit(-2);
			}
			if( (currentLE = getFromHashlist(scndTmp->valuestring,hashList)) == NULL) {
				printf("hashtable lookup failure: %s\n", scndTmp->valuestring);
				exit(-2);
			}
			if (currentLE->dataPtr ==NULL) {
				printf("dataPtr == NULL :( %s\n",scndTmp->valuestring);
				exit(-2);
			}
			/* set tmp variables End */

			if(tmpOS->typ == _ANALOG)
				routeParam(current, tmpOS->pt, tmpOS->po, (unit *)currentLE->dataPtr );
			
			if(tmpOS->typ == _DIGITAL)
				routeBool(current, tmpOS->bt,  (unit *) currentLE->dataPtr);
			
			scndTmp=scndTmp->next;
		}

		tmp=tmp->next;
	}

	/*routing setzen */
	tmp=globals->child;
	while(tmp!=NULL){
		/* ich checke auf ==NULL, also schoen saubermachen */
		type=name=parameters=NULL;

		scndTmp=(tmp == NULL? NULL:tmp->child);
		while(scndTmp!=NULL){
			if(strcmp(scndTmp->string,"type")==0) type = scndTmp;
			if(strcmp(scndTmp->string,"name")==0) name = scndTmp;
			if(strcmp(scndTmp->string,"parameters")==0) parameters = scndTmp;
			scndTmp=scndTmp->next;
		}

		if(name == NULL) printf("ueberall namen angeben\n"), exit(-2);
		if(name->valuestring == NULL) printf("namen muessen strings sein\n"), exit(-2);
		/* get "current" from hashlist start*/
		if( (currentLE = getFromHashlist(name->valuestring,hashList))==NULL){
			printf("Hab in der Hashtable gesucht aber nichts gefunden :(\n");
			exit(-2);
		}
		if(( current = (unit*) currentLE->dataPtr) == NULL){
			printf("Habe in der HT gefunden, aber dataPtr ist NULL, Motherfucka!\n");
			exit(-2);
		}
		/* get "current" from hashlist end*/

		scndTmp = (parameters == NULL? NULL:parameters->child);
		while(scndTmp!=NULL){
			/* if it's a string, it's the routing's job */
			if( scndTmp->type != cJSON_String ) {scndTmp=scndTmp->next;  continue;} 

			/* set tmp variables start */
			if( (currentLE = getFromHashlist(scndTmp->string, attrList) ) == NULL) {
				printf("verstehe paramenter im routing nicht: '%s'\n", scndTmp->string);
				exit(-2);
			}
			if( (tmpOS = (OptionSet * )currentLE->dataPtr) == NULL ){
				printf("DataPtr = NULL :(  '''This shouldn't happen!'''\n");
				exit(-2);
			}
			if( (currentLE = getFromHashlist(scndTmp->valuestring,hashList)) == NULL) {
				printf("hashtable lookup failure: %s\n", scndTmp->valuestring);
				exit(-2);
			}
			if (currentLE->dataPtr ==NULL) {
				printf("dataPtr == NULL :( %s\n",scndTmp->valuestring);
				exit(-2);
			}
			/* set tmp variables End */

			if(tmpOS->typ == _ANALOG)
				routeParam(current, tmpOS->pt, tmpOS->po, (unit *)currentLE->dataPtr );
			
			if(tmpOS->typ == _DIGITAL)
				routeBool(current, tmpOS->bt,  (unit *) currentLE->dataPtr);
			
			scndTmp=scndTmp->next;
		}

		tmp=tmp->next;
	}


	
	tmp = outputs->child;
	while(tmp!=NULL){
		if(strcmp(tmp->string,"local") == 0){
			if( (currentLE = getFromHashlist(tmp->valuestring, hashList)) != NULL )
				routeVoicesOutput( (unit *) currentLE->dataPtr);
			else{
				printf("Hashtable lookup error...");
				exit(-2);
			}
		}

		if(strcmp(tmp->string,"global") == 0){
			if( (currentLE = getFromHashlist(tmp->valuestring, hashList)) != NULL )
				routeMasterOutput( (unit *) currentLE->dataPtr);
			else{
				printf("Hashtable lookup error...");
				exit(-2);
			}
		}
		tmp = tmp->next;
	}
	cJSON_Delete(root);
	return 0;
}
