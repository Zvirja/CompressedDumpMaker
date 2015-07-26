// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the COMPRESSEDDUMPMAKER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// COMPRESSEDDUMPMAKER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef COMPRESSEDDUMPMAKER_EXPORTS
#define COMPRESSEDDUMPMAKER_API __declspec(dllexport)
#else
#define COMPRESSEDDUMPMAKER_API __declspec(dllimport)
#endif

// This class is exported from the CompressedDumpMaker.dll
class COMPRESSEDDUMPMAKER_API CCompressedDumpMaker {
public:
	CCompressedDumpMaker(void);
	// TODO: add your methods here.
};

extern COMPRESSEDDUMPMAKER_API int nCompressedDumpMaker;

COMPRESSEDDUMPMAKER_API int fnCompressedDumpMaker(void);

