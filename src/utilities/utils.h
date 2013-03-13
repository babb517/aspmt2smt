#include<string>
#include<boost/filesystem/operations.hpp>



namespace utils {

	/**
	 * @brief Copies elements from one array to another in reverse.
	 * @param src The base address of the array to copy from.
	 * @param dest The base address of the array to copy to (pointing the destination of the last element to copied from src).
	 */
	template<typename T>
	void reverse_copy(T const* src, T* dest, size_t n) {
		for (size_t i = 0; i < n; i++) {
			dest[n-i-1] = src[i];
		}
	}

	/**
	 * @brief Gets teh working directory for the application.
	 * @return the boost::filesystem::path representing the working directory.
	 */
	inline boost::filesystem::path working_path() {
		return boost::filesystem::current_path();
	}

	/**
	 * @brief Gets the working directory for the application.
	 * @return A string representing the working directory.
	 */
	inline std::string working_dir() {
		return working_path().string();
	}

};