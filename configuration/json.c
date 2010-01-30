#include "json.h"

cJSON* parseJSON(char *filename) {
	FILE *fp;
	long int fsize;
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
		if( (fsize=ftell(fp)) != -1 )
			if( (buf = malloc(sizeof(char)*fsize)) == NULL ) {
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

	json=cJSON_Parse(buf);

	free(data);

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
	else if(strcmp(tmp->valuestring, "mixer")){
		current = addMixer2ch(scope);
		if((tmp = tmp->next) !=NULL) 
			if(tmp->type == cJSON_Number)
				if(tmp->valueint != 2){
					printf("Right now we only have a 2ch Mixer implemented, sorry.\n");
					exit(-1);
				}
					
	}
	else if(strcmp(tmp->valuestring, "lowpass")==0)
		current = addFxLowpass(scope);
	else if(strcmp(tmp->valuestring, "highpass")==0)
		current = addFxHighpass(scope);
	else if(strcmp(tmp->valuestring, "bandpass") == 0)
		current = addFxBandpass(scope);

	return current;
}

int routing(){
	cJSON *root, *config, *locals,*globals,*outputs, *tmp,*scndTmp;

	cJSON *type, *name, *parameters, 
		*vol_range, *vol_val, *vol_mod,
		*param1_range, *param1_val, *param1_mod,
		*freq_range, *freq_val, *freq_mod,
		*activity;
	int i, voices_count;
	
	ListElement *hashList[256];
	ListElement *currentLE;

	unit *current;
	
	for(i=0; i<256;  hashList[i++] = NULL);
	
	root=config=locals=globals=NULL;

	root=parseJSON("routing.pop");
	if(root == NULL) exit(-1);
	
	addToHashlist("voiceFreq", voiceFreq, hashList);
	addToHashlist("voiceVelocity", voiceVelocity, hashList);
	addToHashlist("voiceActive", voiceActive, hashList);

	addToHashlist("voicesOut", voicesOut, hashList);

	addToHashlist("voiceActive", voiceActive, hashList);
	
	

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

	/* alle lokalen units initialisieren.
	Ein durchgang dieser schleife entspricht einem Unit.*/
	tmp=locals->child;
	while(  tmp != NULL ){
		/* ich checke auf ==NULL, also schoen saubermachen */
		type=name=parameters=
		vol_range=vol_val=vol_mod=
		param1_range=param1_val=param1_mod=
		freq_range=freq_val=freq_mod=
		activity=NULL;

		/* Variablenbenennen anfang */
		scndTmp=(tmp == NULL? NULL:tmp->child);
		while(scndTmp!=NULL){
			if(	strcmp(scndTmp->string,	"type")					==	0) type 				=	scndTmp;
			if(	strcmp(scndTmp->string,	"name")					==	0) name 				=	scndTmp;
			if(	strcmp(scndTmp->string,	"parameters")		==	0) parameters 	=	scndTmp;
			scndTmp=scndTmp->next;
		}
		scndTmp = (parameters == NULL? NULL:parameters->child);
		while(scndTmp!=NULL){
			if(	strcmp(scndTmp->string, "param1_mod")		==	0) param1_mod		=	scndTmp;
			if(	strcmp(scndTmp->string, "param1_value")	==	0) param1_val		=	scndTmp;
			if(	strcmp(scndTmp->string, "param1_range")	==	0) param1_range	=	scndTmp;
			if(	strcmp(scndTmp->string, "freq_mod")			==	0) freq_mod			=	scndTmp;
			if(	strcmp(scndTmp->string, "freq_value")		==	0) freq_val			=	scndTmp;
			if(	strcmp(scndTmp->string, "freq_range")		==	0) freq_range		=	scndTmp;
			if(	strcmp(scndTmp->string, "vol_mod")			==	0) vol_mod			=	scndTmp;
			if(	strcmp(scndTmp->string, "vol_value")		==	0) vol_val			=	scndTmp;
			if(	strcmp(scndTmp->string, "vol_range")		==	0) vol_range		=	scndTmp;
			if(	strcmp(scndTmp->string, "activity")			==	0) activity			=	scndTmp;
			scndTmp=scndTmp->next;
		}
		/* Variablenbenennen ende */
		
		if ((current = initUnit(type, usLOCAL)) == NULL) {
			printf("Fehler beim initialisieren einer Unit.");
			exit(-2);
		}
		if(addToHashlist(name->valuestring, current, hashList) == NULL ){
			printf("Fehler beim hinzufuegen zur HashList");
			exit(-2);
		}
		
		if(param1_val !=NULL && param1_val->type ==cJSON_Number) 
			setParam(current, ptPARAM1, poVAL, param1_val->valuedouble);
		if(param1_range !=NULL && param1_range->type ==cJSON_Number) 
			setParam(current, ptPARAM1, poRANGE, param1_range->valuedouble);
		if(param1_mod !=NULL && param1_mod->type ==cJSON_Number) 
			setParam(current, ptPARAM1, poMOD, param1_mod->valuedouble);
		
		if(vol_val !=NULL && vol_val->type ==cJSON_Number) 
			setParam(current, ptVOL, poVAL, vol_val->valuedouble);
		if(vol_range !=NULL && vol_range->type ==cJSON_Number) 
			setParam(current, ptVOL, poRANGE, vol_range->valuedouble);
		if(vol_mod !=NULL && vol_mod->type ==cJSON_Number) 
			setParam(current, ptVOL, poMOD, vol_mod->valuedouble);

		if(freq_val !=NULL && freq_val->type ==cJSON_Number) 
			setParam(current, ptFREQ, poVAL, freq_val->valuedouble);
		if(freq_range !=NULL && freq_range->type ==cJSON_Number) 
			setParam(current, ptFREQ, poRANGE, freq_range->valuedouble);
		if(freq_mod !=NULL && freq_mod->type ==cJSON_Number) 
			setParam(current, ptFREQ, poMOD, freq_mod->valuedouble);

		if(activity !=NULL && ( activity->type ==cJSON_True ||activity->type ==cJSON_False  || activity->type == cJSON_Number) ) 
			if(activity->type == cJSON_Number)
				setBool(current, btACT, freq_mod->valueint);
			else if (activity->type == cJSON_True)
				setBool(current, btACT, 1);
			else if (activity->type == cJSON_False)
				setBool(current, btACT, 0);

		tmp=tmp->next;
	}

	/*routing setzen */
	tmp=locals->child;
	while(tmp!=NULL){
		/* ich checke auf ==NULL, also schoen saubermachen */
		type=name=parameters=
		vol_range=vol_val=vol_mod=
		param1_range=param1_val=param1_mod=
		freq_range=freq_val=freq_mod=
		activity=NULL;

		/* Variablenbenennen anfang */
		scndTmp=(tmp == NULL? NULL:tmp->child);
		while(scndTmp!=NULL){
			if(strcmp(scndTmp->string,"type")==0) type = scndTmp;
			if(strcmp(scndTmp->string,"name")==0) name = scndTmp;
			if(strcmp(scndTmp->string,"parameters")==0) parameters = scndTmp;
			scndTmp=scndTmp->next;
		}

		scndTmp = (parameters == NULL? NULL:parameters->child);
		while(scndTmp!=NULL){
			if(	strcmp(scndTmp->string, "param1_mod")		==	0) param1_mod		=	scndTmp;
			if(	strcmp(scndTmp->string, "param1_value")	==	0) param1_val		=	scndTmp;
			if(	strcmp(scndTmp->string, "param1_range")	==	0) param1_range	=	scndTmp;
			if(	strcmp(scndTmp->string, "freq_mod")			==	0) freq_mod			=	scndTmp;
			if(	strcmp(scndTmp->string, "freq_value")		==	0) freq_val			=	scndTmp;
			if(	strcmp(scndTmp->string, "freq_range")		==	0) freq_range		=	scndTmp;
			if(	strcmp(scndTmp->string, "vol_mod")			==	0) vol_mod			=	scndTmp;
			if(	strcmp(scndTmp->string, "vol_value")		==	0) vol_val			=	scndTmp;
			if(	strcmp(scndTmp->string, "vol_range")		==	0) vol_range		=	scndTmp;
			if(	strcmp(scndTmp->string, "activity")			==	0) activity			=	scndTmp;
			scndTmp=scndTmp->next;
		}
		/* Variablenbenennen ende */
		if(name == NULL) printf("ueberall namen angeben\n"), exit(-2);
		if(name->valuestring == NULL) printf("namen muessen strings sein\n"), exit(-2);

		if( (currentLE = getFromHashlist(name->valuestring,hashList))==NULL){
			printf("Hab in der Hashtable gesucht aber nichts gefunden :(\n");
			exit(-2);
		}
		if(( current = (unit*) currentLE->dataPtr) == NULL){
			printf("Habe in der HT gefunden, aber dataPtr ist NULL. Motherfucka!");
			exit(-2);
		}
		
		if(param1_val !=NULL && param1_val->type ==cJSON_String)
			routeParam(current,  ptPARAM1, poVAL, (unit*)(getFromHashlist(param1_val->valuestring, hashList)->dataPtr));
		if(param1_range !=NULL && param1_range->type ==cJSON_String)
			routeParam(current, ptPARAM1,poRANGE,(unit*)(getFromHashlist(param1_range->valuestring, hashList)->dataPtr));
		if(param1_mod !=NULL && param1_mod->type ==cJSON_String)
			routeParam(current, ptPARAM1,poMOD,(unit*)(getFromHashlist(param1_mod->valuestring, hashList)->dataPtr));

		if(vol_val !=NULL && vol_val->type ==cJSON_String)
			routeParam(current,  ptVOL, poVAL, (unit*)(getFromHashlist(vol_val->valuestring, hashList)->dataPtr));
		if(vol_range !=NULL && vol_range->type ==cJSON_String)
			routeParam(current, ptVOL,poRANGE,(unit*)(getFromHashlist(vol_range->valuestring, hashList)->dataPtr));
		if(vol_mod !=NULL && vol_mod->type ==cJSON_String)
			routeParam(current, ptVOL,poMOD,(unit*)(getFromHashlist(vol_mod->valuestring, hashList)->dataPtr));

		if(freq_val !=NULL && freq_val->type ==cJSON_String)
			routeParam(current,  ptFREQ, poVAL, (unit*)(getFromHashlist(freq_val->valuestring, hashList)->dataPtr));
		if(freq_range !=NULL && freq_range->type ==cJSON_String)
			routeParam(current, ptFREQ,poRANGE,(unit*)(getFromHashlist(freq_range->valuestring, hashList)->dataPtr));
		if(freq_mod !=NULL && freq_mod->type ==cJSON_String)
			routeParam(current, ptFREQ,poMOD,(unit*)(getFromHashlist(freq_mod->valuestring, hashList)->dataPtr));

		if(activity != NULL && activity->type == cJSON_String)
			routeBool(current, btACT,(unit*)(getFromHashlist(activity->valuestring, hashList)->dataPtr));

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
