#ifndef CONFIG_HPP
#define CONFIG_HPP


#include <string>
#include <fstream>

#include "utils.hpp"
#include "json_reader.hpp"
#include "../thirdparty/json/single_include/nlohmann/json.hpp"


/**
 * @brief Configuration class
 * 
 * STATIC ONLY CLASS
 */
class Config {
  public:
    /**
     * @brief Defines the Keys used in the config file.
     * 
     * STATIC ONLY STRUCT
     */
    struct Keys {
      /**
       * @brief Enforce Static Class
       */
      Keys() = delete;


      /******************************************
      **************** Variables ****************
      ******************************************/


      inline static const std::string WINDOW_SETTINGS_SECTION = "Window Settings";
      inline static const std::string WINDOW_HEIGHT                 = WINDOW_SETTINGS_SECTION + ".Window Height";
      inline static const std::string WINDOW_WIDTH                  = WINDOW_SETTINGS_SECTION + ".Window Width";
      inline static const std::string STYLESHEET_PATH               = WINDOW_SETTINGS_SECTION + ".Stylesheet Path";
    };
  

  private:

    /** Object containing the json data */
    static JsonReader _config_json;

      
    /** Constants */
    inline static const std::string _CONFIG_FILE_PATH = "config/config.json";


  public:
    /**
     * @brief Enforce Static Class
     */
    Config() = delete;


    /**
     * @brief Initialize the config
     */
    static void init() {
      // Load json data
      _config_json.loadFromFile(_CONFIG_FILE_PATH);
    }


    TODO: Create a method for parsing the keys in the 'Keys' class and apply them to the keys in the funcs below.

    
    /**
     * Wrapper on the JsonReader's 'get()' func. 
     * 
     * @brief Generic getter with default value
     * @param key: Key to get the value of
     * @tparam default_value: Default value if the key doesn't exist
     */
    template <typename T>
    T get(const std::string& key, const T& default_value = T{}) {
      return _config_json.get(key, default_value);
    }


    /**
     * Wrapper on the JsonReader's 'getString()' func. 
     * 
     * @brief Get a string from the json
     * @param key: Key to read the json at
     * @param def: Default value returned if the key does not exist.
     */
    std::string getString(const std::string& key, const std::string& def = "") {
      return _config_json.getString(key, def);
    }


    /**
     * Wrapper on the JsonReader's 'getInt()' func. 
     * 
     * @brief Get an integer from the json
     * @param key: Key to read the json at
     * @param def: Default value returned if the key does not exist.
     */
    int getInt(const std::string& key, int def = 0) {
      return _config_json.getInt(key, def);
    }


    /**
     * Wrapper on the JsonReader's 'getDouble()' func. 
     * 
     * @brief Get a double from the json
     * @param key: Key to read the json at
     * @param def: Default value returned if the key does not exist.
     */
    double getDouble(const std::string& key, double def = 0.0) {
      return _config_json.getDouble(key, def);
    }

    /**
     * Wrapper on the JsonReader's 'getBool()' func. 
     * 
     * @brief Get a boolean from the json
     * @param key: Key to read the json at
     * @param def: Default value returned if the key does not exist.
     */
    bool getBool(const std::string& key, bool def = false) {
      return _config_json.getBool(key, def);
    }
};


#endif // CONFIG_HPP
