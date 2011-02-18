#ifndef SIGSCAN_H
#define SIGSCAN_H

#include "tier0/dbg.h"

#pragma warning( disable: 4267 )	// conversion from 'size_t' to 'int', possible loss of data
#pragma warning( disable: 4311 )	// pointer truncation from 'IMAGE_DOS_HEADER *' to 'unsigned long'
#pragma warning( disable: 4312 )	// conversion from 'unsigned long' to 'IMAGE_NT_HEADERS *' of greater size

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#define BEGIN_CHECK_UNSUCCESSFUL_SIGNATURE_GLOBAL( signature, function ) \
	if ( !signature.is_set ) \
	{ \
		Warning( "attempt to call global '" function "' (an unsuccessful signature scan)\n" );

#define BEGIN_CHECK_UNSUCCESSFUL_SIGNATURE_METHOD( signature, function ) \
	if ( !signature.is_set ) \
	{ \
		Warning( "attempt to call method '" function "' (an unsuccessful signature scan)\n" );

#define END_CHECK_UNSUCCESSFUL_SIGNATURE() \
	}

class CSigScan {
private:
	/* Private Variables */
	/* Base Address of the server module in memory */
	static unsigned char *base_addr;
	/* The length to the module's ending address */
	static size_t base_len;

    /* The signature to scan for */
	unsigned char *sig_str;
	/* A mask to ignore certain bytes in the signature such as addresses
	   The mask should be as long as all the bytes in the signature string
	   Use '?' to ignore a byte and 'x' to check it
	   Example: "xxx????xx" - The first 3 bytes are checked, then the next 4 are
	   ignored, then the last 2 are checked */
	char *sig_mask;
	/* The length of sig_str and sig_mask (not including a terminating null for sig_mask) */
	size_t sig_len;

	/* Private Functions */
	void* FindSignature(void);

public:
	/* Public Variables */
	/* sigscan_dllfunc should be set to the function "gameServerFactory" in Load() */
	static void *(*sigscan_dllfunc)(const char *pName, int *pReturnCode);
	/* If the scan was successful or not */
	char is_set;
	/* Starting address of the found function */
	void *sig_addr;

	/* Public Functions */
	CSigScan(void): sig_str(NULL), sig_mask(NULL), sig_len(0), sig_addr(NULL) {}
	~CSigScan(void);

	static bool GetDllMemInfo(void);
	void Init(unsigned char *sig, char *mask, size_t len);
};

/* Sigscanned member functions are casted to member function pointers of this class
   and called with member function pointer syntax */
class CEmpty { };
 
#endif
