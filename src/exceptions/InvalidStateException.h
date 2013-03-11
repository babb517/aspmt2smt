#include <exception>
#include <string>
#include <list>

/**
 * @brief A class that is used to signal that an operation was performed which is
 * invalid for the current state of an object.
 */
class InvalidStateException : std::exception
{
private:

	/**
	 * @brief A description of the exception that ocurred.
	 */
	std::string mWhat;

public:
	/**
	  * @brief Initializes the exception.
	  * @param what A description of the error that has ocurred.
	  */
	inline InvalidStateException(std::string const& what) { 
		mWhat = what;
	}


	/**
	 * @brief Gets a message based about the exception that occurred.
	 * @return A message describing the exception.
	 */
	inline virtual char const* what() const { return mWhat.c_str(); }

};