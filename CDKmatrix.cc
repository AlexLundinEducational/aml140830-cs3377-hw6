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

// forward class definitions
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


// Records in the file have a fixed length buffer
// that will hold a C-Style string. This is the
// size of the fixed length buffer.
const int maxRecordStringLength = 25;

class BinaryFileRecord
{
	public:
	uint8_t strLength;
	char stringBuffer[maxRecordStringLength];
};

// method prototypes
BinaryFileHeader* readBinaryHeader(std::string binaryRecordFile);
BinaryFileRecord* readBinaryRecord(std::string binaryRecordFile);
std::string itoa(uint64_t val, int base);

int main()
{

	// binary IO
	
	std::string binaryRecordFile = "./cs3377.bin";

	// store header
	BinaryFileHeader *myHeader = readBinaryHeader(binaryRecordFile);	

	
	
	
	

	// CDK Screen Matrix
	
	WINDOW	*window;
	CDKSCREEN	*cdkscreen;
	CDKMATRIX     *myMatrix;           

	const char 		*rowTitles[MATRIX_HEIGHT+1] = {"R0", "R1", "R2", "R3", "R4", "R5"};
	const char 		*columnTitles[MATRIX_WIDTH+1] = {"C0", "C1", "C2", "C3"};
	int		boxWidths[MATRIX_WIDTH+1] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
	int		boxTypes[MATRIX_WIDTH+1] = {vMIXED, vMIXED, vMIXED, vMIXED};



	// Initialize the Cdk screen.
	// Make sure the putty terminal is large enough

	window = initscr();
	cdkscreen = initCDKScreen(window);

	// Start CDK Colors
	initCDKColor();


	// Create the matrix.

	myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
	MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
	boxTypes, 1, 1, ' ', ROW, true, true, false);

	if (myMatrix ==NULL)
	{
		printf("Error creating Matrix\n");
		_exit(1);
	}

	// Display the Matrix
	drawCDKMatrix(myMatrix, true);

	
	// convert the magicNumber to string, which is a integer represenation of a hex number
	std::stringstream myStream;
	myStream << std::hex << (uint32_t)myHeader->magicNumber;
	std::string result( myStream.str() );

	
	// display magicNumber in matrix
	setCDKMatrixCell(myMatrix, 1, 1, result.c_str());
	
	// convert the versionNumber to string, which is a integer represenation of a hex number
	//myStream << std::int << (uint32_t)myHeader->versionNumber;
	//result( myStream.str() );
	result = itoa((uint64_t)myHeader->versionNumber, 10);
	

	
	// display versionNumber in matrix
	setCDKMatrixCell(myMatrix, 1, 2, result.c_str());
	
	// convert the numRecords to string, which is a integer represenation of a hex number
	//std::stringstream myStream;
	//myStream << std::int << (uint64_t)myHeader->numRecords;
	//result( myStream.str() );
	result = itoa(myHeader->numRecords, 10);
	
	// display numRecords in matrix
	setCDKMatrixCell(myMatrix, 1, 3, result.c_str());
	
	
	
	
	
	// actually draw the matrix from values above
	drawCDKMatrix(myMatrix, true);

	// keep results on screen
	sleep (2);


	
	// Cleanup screen
	endCDK();

	// loop based on number of records found in the header

	for (int i = 0; i < (uint32_t)myHeader->numRecords ; i++){
		// read binary data into object
		BinaryFileRecord *myRecord = readBinaryRecord(binaryRecordFile);
		
		// convert the header to string, which is a integer represenation of a hex number
		std::stringstream myStream;
		myStream << std::hex << (uint32_t)myHeader->magicNumber;
		std::string result( myStream.str() );		
		printf(myRecord->stringBuffer,myRecord->strLength);
		// add to matrix during loop
	}

}

// read the header
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


// read a record
BinaryFileRecord* readBinaryRecord(std::string binaryRecordFile)
{
	BinaryFileRecord *myRecord = new BinaryFileRecord();

	ifstream binInfile (binaryRecordFile.c_str(), ios::in | ios::binary);

	binInfile.read((char *) myRecord, sizeof(BinaryFileRecord));
	
	cout << "String Length was: " << myRecord->strLength << endl;
	cout << "String was: " << myRecord->stringBuffer << endl;
	
	
	binInfile.close();
	return myRecord;
}

// integer to string method
std::string itoa(uint64_t value, int base) {
	
	//cout << "Itoa input was: " << setprecision(10) << value << endl;

	
	enum { kMaxDigits = 35 };
	
	std::string buf;
	
	buf.reserve( kMaxDigits ); // Pre-allocate enough space.
	

	
	// check that the base if valid
	
	if (base < 2 || base > 16) return buf;
	


	
	int quotient = value;
	

	
	// Translating number to string with base:
	
	do {
	
		buf += "0123456789abcdef"[ std::abs( quotient % base ) ];
	
		quotient /= base;
	
	} while ( quotient );
	


	
	// Append the negative sign for base 10
	// if the value is less than 0
	if ( value < 0 && base == 10) buf += '-';
	

	
	std::reverse( buf.begin(), buf.end() );
	
	return buf;
	
}