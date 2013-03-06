#ifndef __H_COMPOUND_IFSTREAM__
#define __H_COMPOUND_IFSTREAM__


#include <fstream>
#include <string>
#include <list>
/**
 * @brief An extension of the ifstream in order to allow for reading from multiple files seamlessly.
 */
class CompoundIFStream : public std::ifstream {
private:
	/***********************************************************************/
	/* Members */
	/***********************************************************************/
	std::list<std::string> mInputs;			///< The input files we're reading from.


public:
	/***********************************************************************/
	/* Constructors / Destructors */
	/***********************************************************************/

	/**
	 * @brief Basic Constructor.
	 * Initializes the filestream with the provided inputs, checking if the files are valid.
	 * Sets the fail bit if one or more of the inputs cannot be opened.
	 * @param inputs A list of inputs to use (or null to read from none).
	 */
	CompoundIFStream(std::list<std::string> const* inputs = NULL);

	/**
	 * @brief Basic Destructor.
	 * Does Nothing. Closing the stream is accomplished by the ifstream destructor.
	 */
	virtual inline ~CompoundIFStream()		{ /* Intentionally Left Blank */ }


	/***********************************************************************/
	/* Public Methods */
	/***********************************************************************/

	// TODO:
	// open
	// close
	// get
	// >>
	// getline
	// peek
	// read
	// readsome
	// putback
	// unget
	// tellg
	// seekg
	// sync
	// sentry


};

#endif
