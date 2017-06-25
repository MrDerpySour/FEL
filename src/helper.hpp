#ifndef FEL_HELPER_HPP_
#define FEL_HELPER_HPP_

#include <vector>
#include <string>

namespace fel {
namespace helper {

/**
 * It takes a string and returns a vector of each word seperated by the seperator
 * @param str The string to tokenize
 * @param seperator The seperation char
 * @param delete_empty Whether or not it should delete empty entries (e.g. mutliple seperators in a row)
 * @return A vector of all the string spliced at the seperator
 */
std::vector<std::string> tokenize(const std::string& str,
                                          const char& seperator = ' ',
                                          const bool& delete_empty = true);

/**
 * Replaces every occurance of a string with another string
 * @param str The string to modify
 * @param find The string to find
 * @param replace The string to replace it with
 * @return A copy of the processed string
 */
std::string str_replace_all(std::string& str,
                                    const std::string& find,
                                    const std::string& replace);

/**
 * Checks whether or not a file exists
 * @param file_path The path to the file
 * @return Whether or not it exists
 */
bool fileExists(const std::string& file_path);

/**
 * Creates a file
 * @param file_path The path to the file
 * @param contents The contents of the file
 * @return Whether or not the operation was successful
 */
bool createFile(const std::string& file_path,
                        const std::string& contents);

/**
 * Returns a random integer between the specified range
 * @param min The minimum range
 * @param max The maximum range
 * @return The random integer
 */
int getInt(const int& min, const int& max);

/**
 * Parses a math equation in string format
 * @param expression The expression to pars
 * @return The result of the expression
 */
double parseMathString(std::string expression);

} // namespace helper
} // namespace fel

#endif // FEL_HELPER_HPP_
