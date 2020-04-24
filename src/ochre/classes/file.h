#ifndef file_h
#define file_h



struct OcText;

struct OcFile
{
	void *file;
	char path[1024];
	bool trunc;

	OcFile();

	void SetPath(OcText *, bool);
	void Writeln(OcText *);
	void Write(OcText *);
	void Close();
};

#endif
