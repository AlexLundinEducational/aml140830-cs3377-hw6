//Alex Lundin
//AML140830@utdallas.edu
//SE 3377.501 - CC++ in  UNIX
#include "program6.h"
#include <stdio.h>
#include <stdlib.h>

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 15
#define MATRIX_NAME_STRING "Test Matrix"
using namespace std;

// forward definitions
class BinaryFileHeader;
class BinaryFileRecord;

// class definitions
class BinaryFileHeader
{
	public:
	uint32_t magicNumber; /* Should be 0xFEEDFACE */
	uint32_t versionNumber;
	uint64_t numRecords;
};

/*
* Records in the file have a fixed length buffer
* that will hold a C-Style string. This is the
* size of the fixed length buffer.
*/
const int maxRecordStringLength = 25;

class BinaryFileRecord
{
	public:
	uint8_t strLength;
	char stringBuffer[maxRecordStringLength];
};

// method prototypes
BinaryFileHeader* readBinaryHeader(std::string binaryRecordFile);
char* itoa(int val, int base);

int main()
{

	// binary IO
	// store header
	std::string binaryRecordFile = "./cs3377.bin";

	BinaryFileHeader *myHeader = readBinaryHeader(binaryRecordFile);	

	
	// loop based on number of records
	
	


	WINDOW	*window;
	CDKSCREEN	*cdkscreen;
	CDKMATRIX     *myMatrix;           // CDK Screen Matrix
	//binaryRecordHeader.c_str()
	const char 		*rowTitles[MATRIX_HEIGHT+1] = {"R0", "R1", "R2", "R3", "R4", "R5"};
	const char 		*columnTitles[MATRIX_WIDTH+1] = {"C0", "C1", "C2", "C3"};
	int		boxWidths[MATRIX_WIDTH+1] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
	int		boxTypes[MATRIX_WIDTH+1] = {vMIXED, vMIXED, vMIXED, vMIXED};


	/*
	* Initialize the Cdk screen.
	*
	* Make sure the putty terminal is large enough
	*/
	window = initscr();
	cdkscreen = initCDKScreen(window);

	/* Start CDK Colors */
	initCDKColor();

	/*
	* Create the matrix.  Need to manually cast (const char**) to (char **)
	*/
	myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
	MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
	boxTypes, 1, 1, ' ', ROW, true, true, false);

	if (myMatrix ==NULL)
	{
		printf("Error creating Matrix\n");
		_exit(1);
	}

	/* Display the Matrix */
	drawCDKMatrix(myMatrix, true);

	/*
	* Dipslay a message
	*/
	setCDKMatrixCell(myMatrix, 2, 2, "Test Message");
	
	char *buffer = itoa(myHeader->magicNumber,16);
	
	setCDKMatrixCell(myMatrix, 1, 1, buffer);
	
	drawCDKMatrix(myMatrix, true);    /* required  */

	/* so we can see results */
	sleep (2);


	// Cleanup screen
	endCDK();
	cout << *buffer << endl;

}


BinaryFileHeader* readBinaryHeader(std::string binaryRecordFile)
{
	BinaryFileHeader *myHeader = new BinaryFileHeader();

	ifstream binInfile (binaryRecordFile.c_str(), ios::in | ios::binary);

	binInfile.read((char *) myHeader, sizeof(BinaryFileHeader));
	
	cout << "magicNumber was: " << setprecision(10) << myHeader->magicNumber << endl;
	cout << "versionNumber was: " << setprecision(10) << myHeader->versionNumber << endl;
	cout << "Number of records was: " << setprecision(10) << myHeader->numRecords << endl;
	
	binInfile.close();
	return myHeader;
}

char* itoa(int val, int base){
	
	static char buf[32] = {0};
	
	int i = 30;
	
	for(; val && i ; --i, val /= base)
	
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
	
}