#include "classes/file.h"
#include "classes/text.h"
#include <string.h>
#include <stdio.h>



OcFile::OcFile() : file(0), trunc(true) {
	strcpy_s(path, "./ochre_out.txt");
}

void OcFile::SetPath(OcText *t, bool _trunc) {

	Close();

	if (t && t->t)
		strcpy_s(path, t->t);

	trunc = _trunc;
}

void OcFile::Writeln(OcText *t) {

	if (file == 0) {
		fopen_s((FILE **)&file, path, trunc ? "w" : "a");
		if (file == 0)
			return;
	}

	if (t && t->t)
		fputs(t->t, (FILE *)file);

	fputc('\n', (FILE *)file);
}

void OcFile::Write(OcText *t) {

	if (file == 0) {
		fopen_s((FILE **)&file, path, "w");
		if (file == 0)
			return;
	}

	if (t && t->t)
		fputs(t->t, (FILE *)file);
}

void OcFile::Close() {
	if (file) {
		fclose((FILE *)file);
		file = 0;
	}
}
