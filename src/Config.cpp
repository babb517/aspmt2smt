#include <cstring>
#include <string>
#include <iostream>

#include "Config.h"

// Initializes config to defaults.
Config::Config() {
	memset(mModified, 0, _OPT_LENGTH * sizeof(int));
	mOutputModified = 0;


	memset(mOptions, 0, _OPT_INC * sizeof(int));

	// TODO: Defaults
	// options
	// mOutput
}


// Attempts to add an input file to the list.
bool Config::addInput(std::string const& file) {
	// TODO
}

// Attempts to open all of the input files and generate a compound input stream.
std::istream* openInputs() {
	// TODO
}

// Attempts to open the output file for writing.
std::ostream* openOutput() {
	// TODO
}