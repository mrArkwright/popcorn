#include "tools.h"

void initMixer(mixer *m) {
	int i;

	m->busses = (bus *) malloc(sizeof(bus) * m->busCount);

	for (i = 0; i < m->busCount; i++) {
		m->busses[i].act = 0;
		initParam(&(m->busses[i].vol), NULL, NULL);
	}
}

void compMixer(mixer *m) {
	int i;

	m->val = 0;

	for (i = 0; i < m->busCount; i++) {
		if (m->busses[i].act == 1) {
			m->val += *(m->busses[i].input) * compParam(&(m->busses[i].vol));
		}
	}
}
