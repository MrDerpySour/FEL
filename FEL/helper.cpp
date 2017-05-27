#include "helper.hpp"

#include <fstream>
#include <chrono>
#include <random>
#include <functional>
#include <sstream>

namespace fel {
namespace helper {

std::vector<std::string> tokenize(const std::string& str,
                                  const char& seperator,
                                  const bool& delete_empty) {
  std::vector<std::string> vec;

  size_t last_pos = 0;
  for (size_t i = 0; i < str.size(); ++i) {
    if (str[i] == seperator) {
      if (delete_empty) {
        std::string s = "";
        if ((s = str.substr(last_pos, i - last_pos)) != "" && !s.empty() && s != std::to_string(seperator)) {
          vec.push_back(s);
        }
      } else {
        vec.push_back(str.substr(last_pos, i - last_pos));
      }

      last_pos = i + 1;
    }
  }
  vec.push_back(str.substr(last_pos, str.size() - last_pos));

  return vec;
}

std::string str_replace_all(std::string& str,
                            const std::string& find,
                            const std::string& replace) {
  size_t pos;

  while ((pos = str.find(find)) != std::string::npos) {
    str.replace(pos, find.length(), replace);
  }

  return str;
}

bool fileExists(const std::string& file_path) {
  return std::ifstream(file_path).good();
}

bool createFile(const std::string& file_path, const std::string& contents) {
  bool good = false;
  std::ofstream file(file_path, std::fstream::trunc);
  file << contents;
  good = file.good();

  file.close();
  return good;
}

int getInt(const int& min, const int& max) {

  long long seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

  auto random_int = std::bind(std::uniform_int_distribution<int>(min, max),
                              std::mt19937(static_cast<unsigned int>(seed)));

  return random_int();
}

double parseMathString(std::istringstream* parser);

char peek(std::istringstream* parser) {
  return static_cast<char>(parser->peek());
}

char get(std::istringstream* parser) {
  return static_cast<char>(parser->get());
}

double number(std::istringstream* parser) {
  double result;
  parser->operator >> (result);
  return result;
}

double factor(std::istringstream* parser) {
  if ((peek(parser) >= '0' && peek(parser) <= '9') || peek(parser) == '.')
    return number(parser);
  else if (peek(parser) == '(') {
    get(parser); // '('
    double result = parseMathString(parser);
    get(parser); // ')'
    return result;
  } else if (peek(parser) == '-') {
    get(parser);
    return -parseMathString(parser);
  }
  return 0.0; // Just return 0 on an error
}

double term(std::istringstream* parser) {
  double result = factor(parser);
  while (peek(parser) == '*' || peek(parser) == '/' || peek(parser) == '%') {
    char c = get(parser);
    if (c == '*') {
      result *= factor(parser);
    } else if (c == '/') {
      result /= factor(parser);
    } else if (c == '%') {
      result = std::fmod(result, factor(parser));
    }
  }

  return result;
}

double parseMathString(std::istringstream* parser) {
  double result = term(parser);
  while (peek(parser) == '+' || peek(parser) == '-')
    if (get(parser) == '+')
      result += term(parser);
    else
      result -= term(parser);
  return result;
}

double parseMathString(std::string expression) {
  str_replace_all(expression, " ", "");
  str_replace_all(expression, "	", "");

  std::istringstream parser(expression);

  return parseMathString(&parser);
}

} // namespace helper
} // namespace fel
