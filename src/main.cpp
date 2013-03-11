

//#include "Translator.h"
//#include "SMTWriter.h"
//#include "Config.h"

#include "utilities/CompoundFileSource.h"


/**
 * @brief Main driver routine for the ASPMT2SMT system.
 * Performs command line parsing and configuration setup and then transfers control to the Translator.
 */
int main(int argc, char** argv) {
	// TODO: Command line parsing

	utils::CompoundFileStream fstream;


	std::list<std::string> files;

	files.push_back("F:\\\\users\\jbabb\\tmp\\one.in");
	files.push_back("F:\\\\users\\jbabb\\tmp\\two.in");
	files.push_back("F:\\\\users\\jbabb\\tmp\\three.in");

	fstream.open(&files);

	char c;


	do {
		fstream >> c;
		std::cout << "File: " << fstream->filename() << ": Got '" << c << "'\n";
	} while (c != 'x' && fstream.good());

	// insert!
	fstream->insert("F:\\\\users\\jbabb\\tmp\\four.in");

	do {
		fstream >> c;
		std::cout << "File: " << fstream->filename() << ": Got '" << c << "'\n";
	} while (fstream.good());


//	while (


	// TODO:
	return 0;
}
