#include <exception>
#include <string>
#include <list>

/**
 * @brief A class that is used to signal that a file name was unable to be resolved.
 */
class FileNotFoundException : std::exception
{
private:
	/**
	 * @brief The file that caused the exception.*
	 */
	std::list<std::string> mFiles;

	/**
	 * @brief A description of the exception that ocurred.
	 */
	std::string mWhat;

public:
	/**
	  * @brief Initializes the exception.
	  * @param The file that was unable to be located.
	  */
	inline FileNotFoundException(std::string const& file) { 
		mFiles.push_back(file); 
		mWhat = "An error occurred opening the file '" + file + "'. Check that the file exists and you have permission to read it."; 
	}

	/**
	  * @brief Initializes the exception.
	  * @param A list of files that could not be found.
	  */
	inline FileNotFoundException(std::list<std::string> const& files) { 
		mWhat = "The following files could not be opened for reading: ";
		for (std::list<std::string>::const_iterator it = files.begin(); it != files.end();) {
			mFiles.push_back(*it);
			mWhat += "'" + *it + "'";
			if (++it != files.end()) mWhat += ", ";
		}
		mWhat += ". Check that the files exists and you have permission to read them.";
	}

	/**
	 * @brief Gets a message based about the exception that occurred.
	 * @return A message describing the exception.
	 */
	inline virtual char const* what() const { return mWhat.c_str(); }

	/**
	 * @brief Determines the file that caused the exception.
	 * @return A list of the files that could not be resolved.
	 */
	inline std::list<std::string> const& files() const { return mFiles; }

};