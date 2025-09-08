#ifndef JSON_READER_HPP
#define JSON_READER_HPP


#include <string>
#include <fstream>

#include "utils.hpp"
#include "../thirdparty/json/single_include/nlohmann/json.hpp"


/**
 * @brief Easily read and get values from a .json file.
 */
class JsonReader {
  private:
    /** Object containing the json data */
    static nlohmann::json _data;


  public:
    /**
     * @brief Dummy Constructor/Destructor
     */
    JsonReader() {}
    ~JsonReader() {}


    /**
     * @brief Load from a file
     * @param filename: File to read from
     * @returns bool: True/False of success
     */
    bool loadFromFile(const std::string& filename) {
      // Attempt to open file
      std::ifstream file(filename);
      if (!file.is_open()) {
        eprintln("Could not open config file: ", filename);
        return false;
      }

      // Attempt to read file data
      try {
        file >> _data;
      } catch (const std::exception& e) {
        // Error occured, so early return.
        eprintln("Failed to parse config JSON: ", e.what());
        return false;
      }

      return true;
    }


    /**
     * @brief Generic getter with default value
     * @param key: Key to get the value of
     * @tparam default_value: Default value if the key doesn't exist
     */
    template <typename T>
    T get(const std::string& key, const T& default_value = T{}) {
      if (_data.contains(key)) {
        try {
          return _data.at(key).get<T>();
        } catch (...) {
          std::cerr << "Type mismatch for key: " << key << "\n";
        }
      }

      // Key doesn't exist, so return the default value.
      return default_value;
    }


    // --------------------------------------------------- //
    // --------------- Convenience helpers --------------- //
    // --------------------------------------------------- //


    /**
     * @brief Get a string from the json
     * @param key: Key to read the json at
     * @param def: Default value returned if the key does not exist.
     */
    std::string getString(const std::string& key, const std::string& def = "") {
      return get<std::string>(key, def);
    }


    /**
     * @brief Get an integer from the json
     * @param key: Key to read the json at
     * @param def: Default value returned if the key does not exist.
     */
    int getInt(const std::string& key, int def = 0) {
      return get<int>(key, def);
    }


    /**
     * @brief Get a double from the json
     * @param key: Key to read the json at
     * @param def: Default value returned if the key does not exist.
     */
    double getDouble(const std::string& key, double def = 0.0) {
      return get<double>(key, def);
    }

    /**
     * @brief Get a bool from the json
     * @param key: Key to read the json at
     * @param def: Default value returned if the key does not exist.
     */
    bool getBool(const std::string& key, bool def = false) {
      return get<bool>(key, def);
    }
};


#endif // JSON_READER_HPP
