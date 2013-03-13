#ifndef __H_COMPOUND_IFSTREAM__
#define __H_COMPOUND_IFSTREAM__

#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/iostreams/stream.hpp>

#include <string>
#include <list>


namespace utils {

/**
	* @brief An extension of the ifstream in order to allow for reading from multiple files seamlessly.
	* @param Ch The character type.
	*/
class CompoundFileSource {

public:
	/***********************************************************************/
	/* Types */
	/***********************************************************************/

	/**
		* @brief The type to use as the character type.
		*/
	typedef char char_type;
	
	/**
		* @brief An enumeration of the various states that the file stream can take.
		*/
	enum state_t {
		GOOD = 0,			///< Indicates the stream is ready and has more characters to read.
		ERROR,				///< Indicates that an error ocurred and the stream has been forcefully closed.
		END,				///< Indicates that the stream has reached the end of all files.
		CLOSED				///< Indicates that the stream has been closed and there are no more characters to read.
	};


	/**
		* @brief The definition of exactly what this device is capable of.
		* source_tag				- The device is able to read characters.
		* closable_tag				- The device should be closed when it's obsolete.
		* peekable_tag				- The device allows for the next character to be seen without reading it.
		* localizable_tag			- The device should be notified when the locality of the stream changes.
		* optimally_buffered_tag	- The device provides a default buffer size to use.
		*/
	typedef struct
		: boost::iostreams::source_tag,
			boost::iostreams::closable_tag,
			boost::iostreams::peekable_tag,
			boost::iostreams::localizable_tag,
			boost::iostreams::optimally_buffered_tag
	{ } category;

private:

	/***********************************************************************/
	/* Private types */
	/***********************************************************************/

	/**
		* @brief A simple structure representing the current state of a file we are reading.
		*/
	struct FileContext {
		std::string filename;					///< The file name specified by the user.
		std::string resolved;					///< The complete path
		char* buf;								///< A buffer used to store data that has be put back into the stream.
		size_t bufsize;							///< The size of buf.
		size_t bufpos;							///< The position that we're at within buf.
		std::ifstream* source;					///< The source used to read from, or null if we don't have a stream open.

		/**
			* @brief Initializes the context, allocated a buffer if a non-zero buffer size was specified.
			*/
		inline FileContext(std::string const& _filename, std::string const& _resolved, std::ifstream* _source, size_t _bufsize)
			: filename(_filename), resolved(_resolved), buf(NULL), bufsize(_bufsize), bufpos(0), source(_source)
			{ if (bufsize) buf = new char[bufsize]; }


		/**
			* @brief Deallocated the buffer, closes, and deallocates the source.
			*/
		~FileContext();

	};

	/***********************************************************************/
	/* Members */
	/***********************************************************************/
	std::list<FileContext*> mStack;			///< The stack of files we are currently reading through.

	std::locale mLocale;					///< Our current locale. Used to propogate to each source.

	state_t mState;							///< The current state of the stream.


public:
	/***********************************************************************/
	/* Constructors / Destructors */
	/***********************************************************************/

	/**
	 * @brief Basic Constructor.
 	 * Initializes the filestream.
	 * @param nullptr_hack a hack argument that can be used to allow boost::iostreams::stream.open to be called. Functionally serves no purpose.
	 */
	CompoundFileSource(void* nullptr_hack = NULL);

	/**
		* @brief Basic Destructor.
		* Closes all open input streams.
		*/
	virtual inline ~CompoundFileSource() { if (state() != CLOSED) close(); };

	/***********************************************************************/
	/***********************************************************************/

	/**
		* @brief adds the specified file to the end of the effective input stream.
		* @param filename The file to add.
		* @param searchPath A list of locations to search for the file, or NULL to just check the working directory.
		*/
	inline bool append(std::string const& filename, std::list<std::string> const* searchPath = NULL) throw(...) { 
		return place(filename, false, searchPath);
	}

	/**
		* @brief adds the specified file to the current location in the effective input stream.
		* @param filename The file to add.
		* @param searchPath A list of locations to search for the file, or NULL to just check the working directory.
		*/
	inline bool insert(std::string const& filename, std::list<std::string> const* searchPath = NULL) throw(...) {
		return place(filename, true, searchPath);
	}

	/**
	* @brief Attempts to move to the next file in the stream.
	* @return True if there are still files in the stream. False otherwise.
	* @throws InvalidStateException If the stream is in an error state and hasn't been reset.
	*/
	bool nextFile();


	inline std::ifstream const* source() const { return mStack.size() ? mStack.front()->source : NULL; }

	/// Gets the name of the file at the top of the stack, or NULL.
	inline std::string const* filename() const { return mStack.size() ? &mStack.front()->filename : NULL; }

	/// Gets the absolute name of the file at the top of the stack, or NULL.
	inline std::string const* resolved() const { return mStack.size() ? &mStack.front()->resolved : NULL; }

	/**
	 * A function used to tell boost that the input should be unbuffered (from their perspective).
	 * This is to allow for us to push file contexts onto the read stack dynamically.
	 */
	inline std::streamsize optimal_buffer_size() const { return 0; }

	/**
	 * @brief Determines the state of the stream.
	 * @returns The current state of the stream.
	 */
	inline state_t state() const { return mState; }

	/// Determines if the stream is open.
	inline bool is_open() const { return state() != CLOSED; }

	/// Determines if the stream is ready to read from.
	inline bool good() const { return state() == GOOD; }

	/**
	 * @brief Attempts to reset the IO stream after an error has occurred.
	 */
	void reset();

	/**
	 * @brief Closes all input streams.
	 */
	void close();

	/**
	 * @brief Puts the character back into the read stream.
	 * @param c The character to put back.
	 * @return True if successful, false otherwise.
	 */
	bool putback(char c) { return putback(&c, 1); }

	/**
	 * @brief Places a number of buffered characters into the input stream to be reread later.
	 * @return True if successful, false otherwise.
	 */
	bool putback(char const* c, std::streamsize n);


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

protected:

	/// Gets the source stream for the top of the stack, or NULL.
	inline std::ifstream* source() { return mStack.size() ? mStack.front()->source : NULL; }

	/// Signals that an error has occurred.
	inline void error() { state(ERROR); }

	/// Sets the state of the stream.
	inline void state(state_t state) { mState = state; }

private:


	/**
	 * @brief Resolves and opens the provided file name, placing the resulting context on the stack.
	 * @param filename The name of the file to open.
	 * @param top Whether the file should be placed at the top of the stack, or the bottom.
	 * @param searchPath A list of locations to search for the file, or NULL to just check the working directory.
	 */
	bool place(std::string const& filename, bool top, std::list<std::string> const* searchPath = NULL);

};

/**
 * @brief A stream wrapper for working with multiple concurrent source files.
 */
typedef boost::iostreams::stream<CompoundFileSource> CompoundFileStream;

}

#endif