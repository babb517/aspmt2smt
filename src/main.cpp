

//#include "Translator.h"
//#include "SMTWriter.h"
//#include "Config.h"

#include "utilities/CompoundFileSource.h"

/**
 * @brief Test function for the compound file source.
 */
void testCompoundFileSource(void);

/**
 * @brief Main driver routine for the ASPMT2SMT system.
 * Performs command line parsing and configuration setup and then transfers control to the Translator.
 */
int main(int argc, char** argv) {
	// TODO: Command line parsing

	for (;;) { }

	// TODO:
	return 0;
}


// Test function for the compound file source.
// TODO: Remove this (eventually)
void testCompoundFileSource() {
	
	utils::CompoundFileStream fstream;
	fstream.open(NULL);

	if (!fstream->append("F:/users/jbabb.ORIGIN/tmp/one.in")) {
		std::cout << "Couldn't find one.in.\n";
	}
	if (!fstream->append("F:/users/jbabb.ORIGIN/tmp/two.in")) {
		std::cout << "Couldn't find two.in.\n";
	}
	if (!fstream->append("F:/users/jbabb.ORIGIN/tmp/three.in")) {
		std::cout << "Couldnt find three.in.\n";
	}

	char c;

	do {
		fstream >> c;
		std::cout << "File: " << *fstream->filename() << ": Got '" << c << "'\n";
	} while (c != 'x' && fstream.good());

	// test putback...
	fstream->putback("xkcd",4);

	// insert!
	if (!fstream->insert("F:/users/jbabb.ORIGIN/tmp/four.in")) {
		std::cout << "Couldn't find four.in.\n";
	}


	do {
		fstream >> c;
		std::cout << "File: " << *fstream->filename() << ": Got '" << c << "'\n";
	} while (fstream.good());


	fstream.close();

}