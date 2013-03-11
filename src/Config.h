#ifndef __H_CONFIG__
#define __H_CONFIG__

#include <string>
#include <list>
#include <iostream>

/**
 * @brief A structure used to maintain and enforce configuration options.
 */
class Config
{
public:
	/***********************************************************************/
	/* Public Types */
	/***********************************************************************/

	/**
	 * @brief An enumeration of the different configuration options that can be used.
	 */
	enum Option
	{
		_OPT_BEGIN = 0x00,			///< Fake option used to indicate the beginning of the options enum.
		_OPT_INC = 0x01,				///< Fake option used for conveniently incrementing the options.

		// TODO

		_OPT_LENGTH = 0x01			///< Fake option used to determine the number of options available.
	};

private:
	/***********************************************************************/
	/* Members */
	/***********************************************************************/

	int mOptions[_OPT_LENGTH];		///< The set values of each of the options.
	int mModified[_OPT_LENGTH];		///< The number of times each option has been modified.
	std::list<std::string> mInputs;	///< A list of files that we will be reading as input.

	std::string mOutput;			///< The file we will be outputting to.
	int mOutputModified;			///< The  of times the output file has been modified by the user.

public:
	/***********************************************************************/
	/* Constructors / Destructors */
	/***********************************************************************/

	/**
	 * @brief Basic Constructor.
	 */
	Config();

	/**
	 * @brief Basic Destructor.
	 * Does nothing.
	 */
	virtual inline ~Config()		{ /* Intentionally Left Blank */ }

	/***********************************************************************/
	/* Accessors / Mutators */
	/***********************************************************************/
	
	/**
	 * @brief Gets the (integer) value of a configuration option.
	 * @param opt The option to read.
	 * @return The integer value of the option.
	 */
	inline int intOpt(Option opt) const								{ return mOptions[opt]; }

	/**
	 * @brief Sets the (integer) value of a configuration option.
	 * @param opt The option to write.
	 * @param val The new value of the option.
	 * @param user Whether we should tally this as a user configuration.
	 * @return The number of times the option has been written by the user previously.
	 */
	inline int intOpt(Option opt, int val, bool user = true)		{ mOptions[opt] = val; return (user) ? mModified[opt]++ : mModified[opt]; }

	/**
	 * @brief Gets the (boolean) value of a configuration option.
	 * @param opt The option to read.
	 * @return The boolean value of the option.
	 */
	inline bool boolOpt(Option opt) const							{ return (bool)mOptions[opt]; }

	/**
	 * @brief Sets the (boolean) value of a configuration option.
	 * @param opt The option to write.
	 * @param val The new value of the option.
	 * @param user Whether we should tally this as a user configuration.
	 * @return The number of times the option has been written by the user previously.
	 */
	inline int boolOpt(Option opt, bool val, bool user = true)		{ mOptions[opt] = (int)val; return (user) ? mModified[opt]++ : mModified[opt]; }


	/**
	 * @brief Determines the number of times an option has been written to by a user.
	 * @param opt The option to access.
	 * @return The number of times a user has written to the provided option.
	 */
	inline int modCount(Option opt)									{ return mModified[opt]; }

	/**
	 * @brief Attempts to add an input file to the list.
	 * @param file The input file to add.
	 * @return True if the file could be resolved, false otherwise.
	 */
	bool addInput(std::string const& file);				

	/**
	 * @brief Gets an iterator pointing to the beginning of the input files list.
	 * @return The requested iterator.
	 */
	inline std::list<std::string>::const_iterator
		beginInputs() const											{ return mInputs.begin(); }

	/**
	 * @brief Gets an iterator pointing to the end of the input files list.
	 * @return The requested iterator.
	 */
	inline std::list<std::string>::const_iterator
		endInputs() const											{ return mInputs.end(); }

	/**
	 * @brief Gets the name of the currently configured output file.
	 * @return The name of the output file.
	 */
	inline std::string const& output() const						{ return mOutput; }

	/**
	 * @brief Sets the name of the file to write output to.
	 * @param file The new output file.
	 * @param user Whether this is a user-issued configuration or not.
	 * @return The number of times the user has changed the output file previously.
	 */
	inline int output(std::string const& file, bool user = true)	{ mOutput = file; return (user) ? mOutputModified++ : mOutputModified; }

	/**
	 * Opens each configured input file and produces a compound input stream.
	 * @return A compound input stream for all input files or NULL if one or more input file cannot be opened or there are no input files.
	 */
	std::istream* openInputs();

	/**
	 * Opens the configured output file.
	 * @return The output stream corresponding to the output file or NULL if the output file cannot be opened.
	 */
	std::ostream* openOutput();


};



#endif
