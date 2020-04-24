#ifndef enums_h
#define enums_h



enum IssueType {
	itNone		= 0,
	itError		= 1,
	itWarning	= 2,
	itNote		= 3
};

enum TokenType {
	ttUnknown	= 0x1,	// todo: rename to identifier
	ttKey		= 0x2,
	ttLiteral	= 0x4,
	ttClass		= 0x8,
	ttType		= 0x10,
	ttStatic	= 0x18,	// ttClass | ttType
	ttRes		= 0x20,
	ttFuncCall	= 0x40,
	ttOperand	= 0x7F,	// ttUnknown | ttKey | ttLiteral | ttClass | ttType | ttRes | ttFuncCall
	ttStruct	= 0x80,
	ttOp		= 0x100
};

enum VarModifier {
	vmNone		= 0,
	vmPrivate	= 1,
	vmPublic	= 2,
	vmConst		= 3
};

enum TokenInfoType {
	tiUndefined	= 0x00,
	tiMember	= 0x01,
	tiProto		= 0x02,
	tiFunc		= 0x04,
	tiVar		= 0x08,
	tiReference	= 0x10,
	tiStatic	= 0x20
};

#define OC_PREF(op_type)	((op_type) & 0xF0000)
#define OC_ASSC(op_type)	((op_type) & 0x0F000)	// 0 <-    1 ->
#define OC_PREC(op_type)	((op_type) & 0x00F00)
#define OC_BNDL(op_type)	((op_type) & 0x0FF00)
#define OC_ENUM(op_type)	((op_type) & 0x000FF)

enum OpType {			//   apee
	otNoOp				= 0x00000,

	otReference			= 0x01101,
	otIndex				= 0x01102,
	otArgList			= 0x01104,	// from stOpenBrace

	otPrefMinus			= 0x10201,	// from otMinus
	otCast				= 0x00208,

	otMultiply			= 0x01301,
	otDivide			= 0x01302,

	otPlus				= 0x01401,
	otMinus				= 0x01402,

	otLess				= 0x01501,
	otLessOrEqual		= 0x01502,
	otGreater			= 0x01504,
	otGreaterOrEqual	= 0x01508,

	otEqual				= 0x01601,
	otNotEqual			= 0x01602,

	otAnd				= 0x01701,

	otOr				= 0x01801,

	otAssign			= 0x00901,
	otPlusAssign		= 0x00902,
	otMinusAssign		= 0x00904,
	otMultiplyAssign	= 0x00908,
	otDivideAssign		= 0x00910,

	otIf				= 0x10A01,
	otElse				= 0x10A02,
	otElseIf			= 0x10A04,
	otIfElse			= 0x10A07,	// otIf | otElse | otElseIf
	otWhile				= 0x10A08,
	otFlow				= 0x10A0F,

	otComma				= 0x01B01
};

enum StructType {
	stBlockBegin	= 0x1,
	stBlockEnd		= 0x2,
	stOpenBrace		= 0x4,
	stCloseBrace	= 0x8
};

enum KeyType {
	ktNoKey			= 0x0,
	ktSet			= 0x1,
	ktSee			= 0x2,
	ktMod			= 0x4,
	ktInter			= 0x6,		// ktMod | ktSee
	ktAct			= 0x10,
	ktDo			= 0x20,
	ktStatic		= 0x21,		// ktSet | ktDo
	ktRequires		= 0x100,
	ktFlow			= 0x100,	// ktRequires
	ktThis			= 0x200,
	ktOther			= 0x400,
	ktType			= 0x800
};

#define OC_USE(use_subj,use_obj)	((use_obj & use_subj) ^ use_subj)

enum UsageType {
	ptNone				= 0x00,
	ptRead				= 0x01,
	ptWrite				= 0x02,
	ptModify			= 0x04,
	ptAppend			= 0x08,
	ptReadModify		= 0x05,		// ptRead | ptModify
	ptWriteModify		= 0x06,		// ptWrite | ptModify
	ptWriteModifyAppend	= 0x0E,		// ptWrite | ptModify | ptAppend
	ptReadWriteModify	= 0x07, 	// ptRead | ptWrite | ptModify
	ptWriteAppend		= 0x0A,		// ptWrite | ptAppend
	ptReadModifyAppend	= 0x0D,		// ptRead | ptModify | ptAppend
	ptAll				= 0x0F,		// ptRead | ptWrite | ptModify | ptAppend
	ptCaller			= 0x10,
	ptReferenced		= 0x8000	// referenced var flag
};

enum OwnerType {	// owner of variables declared @ the root of section
	owtLocal		= 0x00,	// local var (x)
	owtStatic		= 0x01,	// type var (T.x)
	owtAgent		= 0x02,	// agent member var (a.x)
	owtObject		= 0x04,	// object member var (p.x)
	owtResult		= 0x08,	// result member var ((a + b).x)
	owtUndefined	= 0xFF	// undefined
};

enum EnvState {
	envIdle = 0,
	envRunning,
	envPaused,
	envSuspended
};

enum IterPhase {
	itpSet		= 0x01,
	itpImplSee	= 0x02,
	itpSpecSee	= 0x04,
	itpSpecMod 	= 0x08,
	itpAct		= 0x20,
	itpDo		= 0x40,
	itpDecl		= 0x80,
	itpSort		= 0x100,
	itpSerial	= 0x49 	// itpSet | itpDo | itpSpecMod
};

enum RefsQualifier {
	rqNotRefs = 0,	// not a reference collection
	rqAgentRefs,	// can be declared as agent var only
	rqLocalRefs		// can be declared as local var only
};

#endif
