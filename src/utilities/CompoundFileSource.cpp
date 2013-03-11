#include <list>
#include <string>
#include <exception>
#include <fstream>
#include <cstring>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/iostreams/device/file.hpp>

#include "utilities/utils.h"
#include "exceptions/FileNotFoundException.h"
#include "exceptions/InvalidStateException.h"
#include "CompoundFileSource.h"

namespace utils {

/**
 * @brief The number of bytes to allocate to a buffer at a time.
 */
#define BUFFER_BLOCK_SIZE 1024


/*****************************************************************************************/
/* File Context */
/*****************************************************************************************/

/// Free the context
CompoundFileSource::FileContext::~FileContext() {
	if (buf) delete buf;
	if (source) {
		if (source->is_open()) source->close();
		delete source;
	}
}

/******************************************************************************************/
/* Compound File Source */
/******************************************************************************************/

// Constructor
CompoundFileSource::CompoundFileSource(std::list<std::string> const* inputs) {

	std::list<std::string> unresolvedFiles;

	if (inputs) {
		for (std::list<std::string>::const_iterator it = inputs->begin(); it != inputs->end(); it++) {
			try {
				insert(*it);
			} catch (FileNotFoundException e) {
				unresolvedFiles.push_back(e.files().front());
			}
		}
	}

	if (unresolvedFiles.size()) {
		// One or more files were not successfully resolved.
		state(ERROR);
		throw new FileNotFoundException(unresolvedFiles);
	} else if (inputs && inputs->size()) {
		state(GOOD);
	} else {
		// They didn't appear to specify any files.
		// That's interesting.
		state(CLOSED);
	}
}

// Place a file on the stack.
void CompoundFileSource::place(std::string const& filename, bool top, std::list<std::string> const* searchPath) {
	// State handling...
	if (state() == ERROR) throw new InvalidStateException("The stream must be reset prior to adding inputs after an error has occurred. ");


	// initialize the new context
	FileContext* context = new FileContext(filename, "", NULL, 0);


	// Step 1) Validate the name..
	if (!boost::filesystem::native(filename)) {
		// The file's name isn't a valid format.
		delete context;
		throw new FileNotFoundException(filename);
	}


	// Step 2) Attempt to resolve the file.

	// Try the present working directory...
	boost::filesystem::path filepath;
	bool found = false;

	if (searchPath) {
		// Check the search path.
		for (std::list<std::string>::const_iterator it = searchPath->begin(); it != searchPath->end(); it++) {
			try {
				filepath = boost::filesystem::absolute(filename, *it);
				if (boost::filesystem::exists(filepath)) {
					found = true;
					break;
				}
			} catch (boost::filesystem::filesystem_error e) {
				// Something went wrong with our filesystem shenanigans.
				// I have no idea what this could be.
				// TODO: Throw this to some sort of debugging output.
			}
		}
	} else {
		// Just check the working directory.
		filepath = boost::filesystem::absolute(filename);
		if (boost::filesystem::exists(filepath)) found = true;
	}

	// Check if we (eventually) found it.
	if (!found) {
		// We don't really know where the file is.
		delete context;
		throw new FileNotFoundException(filename);
	}

	// Resolve all symlinks and finalize the path we're using.
	context->resolved = boost::filesystem::canonical(filepath).string();

	// Step 3) Ensure the file is readable and open it.
	try {
		context->source = new boost::filesystem::ifstream(filepath);
	} catch (std::exception e) {
		// We can't open the source stream.
		// TODO: We should really throw this to some sort of debugging output.
	}
	if (!context->source || !context->source->good()) {
		// We can't open the file for some reason.
		delete context;
		throw new FileNotFoundException(filename);
	}

	// make sure we copy our locality!
	context->source->imbue(mLocale);

	// Step 4) Add the file context to the stack.
	if (top) {
		mStack.push_front(context);
	} else {
		mStack.push_back(context);
	}

	// Indicate that our state is good to go.
	state(GOOD);
}

// Pops the top file context off the stack.
bool CompoundFileSource::nextFile() {
	if (state() == ERROR) throw new InvalidStateException("The stream must be reset after an error has ocurred. ");

	size_t size = mStack.size();

	// pop off the old state.
	if (size > 1) {
		delete mStack.front();
		mStack.pop_front();
		state(GOOD);
	} else {
		state(END);
	}
	return state() == GOOD;
}

void CompoundFileSource::reset() {
	if (source()) {
		source()->clear();
		state(GOOD);
	} else {
		state(END);
	}
}

// Closes the stream, freeing all available resourced.
void CompoundFileSource::close() {
	for (std::list<FileContext*>::iterator it = mStack.begin(); it != mStack.end(); it++) {
		delete *it;
	}
	mStack.clear();
	state(CLOSED);
}

// Places a number of buffered characters into the input stream to be reread later.
bool CompoundFileSource::putback(char const* c, std::streamsize n) {
	switch (state()) {
	case ERROR:
	case CLOSED:
		return false;

	case END:
	case GOOD:
		// We should have the last file that we read from
		// at the top of stack, so let's add it to the buffer
		FileContext* context = mStack.front();
		std::streamsize newpos = context->bufpos + n;

		if (n >= context->bufsize) {
			// Overflow, expand the buffer.
			context->bufsize = (size_t)(((n + 1) / BUFFER_BLOCK_SIZE) + 1) * BUFFER_BLOCK_SIZE;
			context->buf = (char*)realloc(context->buf, context->bufsize);
		}

		// Copy the contents...
		// Note that this should be done in reverse order, so the last
		// part of the buffer comes out first.
		utils::reverse_copy(c, context->buf + context->bufpos, (size_t)n);
		context->bufpos = (size_t)newpos;
		return true;
	}
}

// Imbues the steam with a provided locality for locality sensitive operations.
void CompoundFileSource::imbue(std::locale const& loc) {
	mLocale = loc;

	// Propogate the changes to each of the open streams.
	for (std::list<FileContext*>::iterator it = mStack.begin(); it != mStack.end(); it++) {
		if ((*it)->source) (*it)->source->imbue(mLocale);
	}
}

// Reads from any open files
std::streamsize CompoundFileSource::read(char* c, std::streamsize n) {
	std::streamsize needed = n;

	// If we've already hit then end, make sure to tell them.
	if (state() == END) return -1;


	// Read from each file starting from the top until we have 
	// all the characters we that we need (or run out of files).
	while (needed && state() == GOOD) {
		FileContext* context = mStack.front();
		std::streamsize size;

		// Check the buffer.
		if (context->bufpos) {
			// we have something...
			size = (context->bufpos  > needed) ? needed : context->bufpos;
			utils::reverse_copy(context->buf + context->bufpos,c, (size_t)size);
			c += size;
			needed -= size;
		}

		// now try the device...
		if (needed) {
			context->source->read(c, needed);
			if (context->source->eof()) {
				// We've read the entire file.
				needed -= context->source->gcount();
				c += context->source->gcount();
				nextFile();
			} else if (context->source->fail()) {
				// Something just went horribly wrong...
				error();
			} else {
				// Everything is a-ok and we have everything we need!
				needed = 0;
			}
		}
	}

	return n - needed;
}

}