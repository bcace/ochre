#ifndef ochreapi_h
#define ochreapi_h

#define OC_NO_INS					-1
#define OC_HAS_INS					-2
#define OC_NO_TYPE					-3
#define OC_OK						1

#define OC_ADDTYPE_SOURCE_PATH		0
#define OC_ADDTYPE_SOURCE_SOURCE	1
#define OC_ADDTYPE_FAIL				-2

#define OC_SELECTTYPE_FAIL			0

#define OC_GETISSUELOC_FAIL			-4

#define OC_STATE_IDLE				0
#define OC_STATE_RUNNING			1
#define OC_STATE_PAUSED				2
#define OC_STATE_SUSPENDED			3



extern "C" {

	int OcCreate();
	int OcDelete();

	int OcAddType(const char *pathOrCode, int source);
	int OcRemoveType();
	int OcClearTypes();
	int OcReloadType(const char *pathOrCode, int source);
	int OcParse();

	const char *OcGetOutput();
	void OcClearOutput();
	int OcIssueCount();
	int OcGetIssueLocation(int);
	const char *OcGetIssueText(int);

	int OcSelectType(int);
	int OcSelectFirstType();
	int OcSelectNextType();
	int OcSelectPool(int);
	int OcSelectFirstPool();
	int OcSelectNextPool();
	const char *OcGetPoolName();

	int OcRun();
	int OcRunTo(int, int);
	int OcStop();
	int OcPause();
	int OcResume();
	int OcReset();
	int OcGetState();

	void OcStep();
	int OcThreadCount();
	int OcBreakType();
	int OcBreakPosition();
	const char *OcPositionalInfo(int, int);

	int OcSelectFirst();
	int OcSelectNext();
	void *OcGetData(int);
	void *OcGetStaticData(int);
	int OcGetVarOffset(const char *, const char *, int);
	const char *OcGetVarName(int, int);
	const char *OcGetVarType(int, int);

	int OcSetThreadCount(int);
	double OcGetGrid(int *);
	void OcGetBounds(double *);
	void OcSetCursorRay(double *);
}

#endif
