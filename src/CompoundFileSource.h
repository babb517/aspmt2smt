#ifndef __H_COMPOUND_IFSTREAM__
#define __H_COMPOUND_IFSTREAM__

#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/categories.hpp>

#include <string>
#include <list>

/**
 * @brief An extension of the ifstream in order to allow for reading from multiple files seamlessly.
 * @param Ch The character type.
 */
template<typename Ch>
class CompoundFileSource {
public:
	/***********************************************************************/
	/* Types */
	/***********************************************************************/

	/**
	 * @brief The type to use as the character type.
	 */
	typedef Ch char_type;

	/**
	 * @brief The definition of exactly what this device is capable of.
	 * source_tag		- The device is able to read characters.
	 * closable_tag		- The device should be closed when it's obsolete.
	 * peekable_tag		- The device allows for the next character to be seen without reading it.
	 * localizable_tag	- The device should be notified when the locality of the stream changes.
	 */
	typedef struct
		: boost::iostreams::source_tag,
		  boost::iostreams::closable_tag,
		  boost::iostreams::peekable_tag,
		  boost::iostreams::localizable_tag
	{ } category;


	/**
	 * @brief A simple structure representing the current location of the file stream.
	 */
	tyepdef struct {
		std::string filename;			///< The file name specified by the user.
		std::string resolvedNmae;		///< The complete path
		size_t line;					///< The line we're reading from.
		size_t column;					///< The column within the line we're at.
		size_t pos;						///< The character within the file we're at.
	} Location;


private:

	/**
	 * @brief The internal context for a file we are currently going through.
	 */
	typedef struct {
		Location loc;													///< Our location in the file.
		boost::iostreams::basic_file_source<Ch>* source;				///< The source used to read from, or null if we don't have a stream open.
	} FileContext;



private:
	/***********************************************************************/
	/* Members */
	/***********************************************************************/
	std::list<std::string> mInputs;			///< The input files we are reading from.

	std::list<FileContext> mContextStack;	///< The stack of files we are currently reading through.

	std::locale mLocale;					///< Our current locale. Used to propogate to each source.

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
	CompoundFileSource(std::list<std::string> const* inputs = NULL);

	/**
	 * @brief Basic Destructor.
	 * Closes all open input streams.
	 */
	virtual inline ~CompoundFileSource() { if (isOpen()) close(); };




	/***********************************************************************/
	/* Public Methods */
	/***********************************************************************/

	/**
	 * @brief adds the specified file to the end of the effective input stream.
	 * @param filename The file to add.
	 * @throws TODO if the file couldn't be resolved.
	 */
	void append(std::string const& filename);

	/**
	 * @brief adds the specified file to the current location in the effective input stream.
	 * @param filename The file to add.
	 * @throws TODO if the file couldn't be resolved.
	 */
	void insert(std::string const& filename);

	/**
	 * @brief Determines if the stream has been properly opened.
	 */
	bool isOpen();


	/**
	 * @brief Returns an object describing the current location of the input stream.
	 * @return The current location of the input stream.
	 */
	Location const& loc();

	/**
	 * @brief Closes all input streams.
	 */
	void close();

	/**
	 * @brief Puts the character back into the read stream.
	 * @param c The character to put back.
	 * @return True if successful, false otherwise.
	 */
	bool putback(Ch c);

	/**
	 * @brief Imbues the device with the provided locality.
	 * @param the new locality.
	 */
	void imbue(std::locale const& loc);

	/**
	 * @brief Attempts to read from the input stream.
	 * @param c The buffer to write to.
	 * @param n The number of characters to read.
	 * @returns The number of characters read, or -1 to indicate the end of stream.
	 */
	std::streamsize read(char* c, std::streamsize n);

};

#endif
