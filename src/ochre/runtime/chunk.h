typedef unsigned char CHUNK;



class CopySchema;
class MemScheme;

void ChkAllocate(CHUNK **, int);

void ChkAppendValue(CHUNK **, int, int, void *);

void ChkReallocateAndCopy(CHUNK **, int, int, CopySchema *);

void ChkDelete(CHUNK **, MemScheme &);
void ChkDelete(CHUNK **);

void ChkClear(CHUNK **, int, int);
