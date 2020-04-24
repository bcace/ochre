#include "ochreapi.h"
#include "ochre.h"



extern "C" {



	int OcCreate() {
		if (Ochre::ins == 0) {
			Ochre::ins = new Ochre();
			return OC_OK;
		}

		return OC_HAS_INS;
	}

	int OcDelete() {
		if (Ochre::ins) {
			delete Ochre::ins;
			Ochre::ins = 0;
			return OC_OK;
		}

		return OC_NO_INS;
	}



	int OcAddType(const char *pathOrCode, int source) {

		if (Ochre::ins) {
		//	if (source == OC_ADDTYPE_SOURCE_PATH)
				return Ochre::ins->AddType(pathOrCode);
		//	else
			//	return OC_ADDTYPE_FAIL;
		}

		return OC_NO_INS;
	}

	int OcRemoveType() {
		if (Ochre::ins) {
			Ochre::ins->RemoveType();
			return 1;
		}

		return OC_NO_INS;
	}

	int OcReloadType(const char *pathOrCode, int source) {

		if (Ochre::ins)
			return Ochre::ins->ReloadType(pathOrCode, source);

		return OC_NO_INS;
	}



	int OcParse() {
		if (Ochre::ins)
			return Ochre::ins->Parse();

		return OC_NO_INS;
	}



	const char *OcGetOutput() {
		if (Ochre::ins)
			return Ochre::ins->GetOutput();

		return 0;
	}

	void OcClearOutput() {
		if (Ochre::ins)
			Ochre::ins->ClearOutput();
	}



	int OcSelectType(int id) {
		if (Ochre::ins)
			return (Ochre::ins->SelectType(id)) ? OC_OK : OC_SELECTTYPE_FAIL;

		return OC_NO_INS;
	}

	int OcSelectFirstType() {
		if (Ochre::ins)
			return Ochre::ins->SelectFirstType();

		return OC_NO_INS;
	}

	int OcSelectNextType() {
		if (Ochre::ins)
			return Ochre::ins->SelectNextType();

		return OC_NO_INS;
	}

	int OcSelectPool(int id) {
		if (Ochre::ins)
			return Ochre::ins->SelectPool(id);

		return OC_NO_INS;
	}

	int OcSelectFirstPool() {
		if (Ochre::ins)
			return Ochre::ins->SelectFirstPool();

		return OC_NO_INS;
	}

	int OcSelectNextPool() {
		if (Ochre::ins)
			return Ochre::ins->SelectNextPool();

		return OC_NO_INS;
	}

	const char *OcGetPoolName() {
		if (Ochre::ins)
			return Ochre::ins->GetPoolName();

		return 0;
	}



	int OcIssueCount() {
		if (Ochre::ins)
			return Ochre::ins->IssueCount();

		return OC_NO_INS;
	}

	int OcGetIssueLocation(int index) {
		if (Ochre::ins) {
			const OcIssue *issue = Ochre::ins->Issue(index);
			return issue ? issue->loc.ps : OC_GETISSUELOC_FAIL;
		}

		return OC_NO_INS;
	}

	const char *OcGetIssueText(int index) {
		if (Ochre::ins) {
			const OcIssue *issue = Ochre::ins->Issue(index);
			return issue ? issue->txt.c_str() : 0;
		}

		return 0;
	}



	int OcRun() {
		if (Ochre::ins)
			return Ochre::ins->Run(-1, -1) ? OC_OK : 0;

		return OC_NO_INS;
	}

	int OcRunTo(int typeId, int pos) {
		if (Ochre::ins)
			return Ochre::ins->Run(typeId, pos) ? OC_OK : 0;

		return OC_NO_INS;
	}

	int OcStop() {
		if (Ochre::ins)
			return Ochre::ins->Stop() ? OC_OK : 0;

		return OC_NO_INS;
	}

	int OcPause() {
		if (Ochre::ins)
			return Ochre::ins->Pause() ? OC_OK : 0;

		return OC_NO_INS;
	}

	int OcResume() {
		if (Ochre::ins)
			return Ochre::ins->Resume() ? OC_OK : 0;

		return OC_NO_INS;
	}

	int OcReset() {
		if (Ochre::ins)
			return Ochre::ins->Reset() ? OC_OK : 0;

		return OC_NO_INS;
	}

	int OcGetState() {
		if (Ochre::ins)
			return Ochre::ins->GetState();

		return OC_NO_INS;
	}

	// debug

	void OcStep() {
		Ochre::ins->Step();
	}

	int OcThreadCount() {
		return Ochre::ins->ThreadCount();
	}

	int OcBreakType() {
		return Ochre::ins->BreakType();
	}

	int OcBreakPosition() {
		return Ochre::ins->BreakPosition();
	}

	const char *OcPositionalInfo(int typeId, int pos) {
		return Ochre::ins->PositionalInfo(typeId, pos);
	}

	// data

	int OcSelectFirst() {
		return Ochre::ins->SelectFirst();
	}

	int OcSelectNext() {
		return Ochre::ins->SelectNext();
	}

	void *OcGetData(int o) {
		return Ochre::ins->GetData(o);
	}

	void *OcGetStaticData(int o) {
		return Ochre::ins->GetStaticData(o);
	}

	int OcGetVarOffset(const char *n, const char *t, int o) {
		return Ochre::ins->GetVarOffset(n, t, o);
	}

	const char *OcGetVarName(int i, int s) {
		return Ochre::ins->GetVarName(i, s);
	}

	const char *OcGetVarType(int i, int s) {
		return Ochre::ins->GetVarType(i, s);
	}



	int OcSetThreadCount(int threads) {
		return Ochre::ins->SetThreadCount(threads);
	}

	double OcGetGrid(int *grid) {
		return Ochre::ins->GetGrid(grid);
	}

	void OcGetBounds(double *bounds) {
		Ochre::ins->GetBounds(bounds);
	}

	void OcSetCursorRay(double *ray) {
		Ochre::ins->SetCursorRay(ray);
	}
}
