#ifndef CONFIG_HPP
#define CONFIG_HPP


#include <string>
#include <fstream>

#include "../misc/utils.hpp"
#include "../misc/json_reader.hpp"


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
      **************** Constants ****************
      ******************************************/

      inline static const std::string SEPERATOR = ".";

      /** Section: Window Settings */
      inline static const std::string WINDOW_SETTINGS_SECTION = "Window Settings";
      inline static const std::string WINDOW_HEIGHT                 = WINDOW_SETTINGS_SECTION + SEPERATOR + "Window Height";
      inline static const std::string WINDOW_WIDTH                  = WINDOW_SETTINGS_SECTION + SEPERATOR + "Window Width";
      inline static const std::string STYLESHEET_PATH               = WINDOW_SETTINGS_SECTION + SEPERATOR + "Stylesheet Path";


      /** Section: Filesystem Settings */
      inline static const std::string FILESYSTEM_SETTINGS_SECTION = "Filesystem Settings";
      inline static const std::string DEFAULT_DIRECTORY             = FILESYSTEM_SETTINGS_SECTION + SEPERATOR + "Default Directory";
    };
  

  private:

    /** Object containing the json data */
    inline static JsonReader _config_json{};

      
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
      if (DEBUG) {
        // NOTE: You must personally change the path to the correct file.
        // Using CMake, my executable is simply one directory too deep, so
        // I go back one to read the config path.
        _config_json.loadFromFile("../" + _CONFIG_FILE_PATH);
      }
      else {
        _config_json.loadFromFile(_CONFIG_FILE_PATH);
      }
    }

    
    /**
     * Wrapper on the JsonReader's 'get()' func. 
     * 
     * @brief Generic getter with default value
     * @param key: Key to get the value of
     * @tparam default_value: Default value if the key doesn't exist
     */
    template <typename T>
    static T get(const std::string& key, const T& default_value = T{}) {
      return _config_json.get(key, default_value, Keys::SEPERATOR);
    }


    /**
     * Wrapper on the JsonReader's 'getString()' func. 
     * 
     * @brief Get a string from the json
     * @param key: Key to read the json at
     * @param def: Default value returned if the key does not exist.
     */
    static std::string getString(const std::string& key, const std::string& def = "") {
      return _config_json.getString(key, def, Keys::SEPERATOR);
    }


    /**
     * Wrapper on the JsonReader's 'getInt()' func. 
     * 
     * @brief Get an integer from the json
     * @param key: Key to read the json at
     * @param def: Default value returned if the key does not exist.
     */
    static int getInt(const std::string& key, int def = 0) {
      return _config_json.getInt(key, def, Keys::SEPERATOR);
    }


    /**
     * Wrapper on the JsonReader's 'getDouble()' func. 
     * 
     * @brief Get a double from the json
     * @param key: Key to read the json at
     * @param def: Default value returned if the key does not exist.
     */
    static double getDouble(const std::string& key, double def = 0.0) {
      return _config_json.getDouble(key, def, Keys::SEPERATOR);
    }

    /**
     * Wrapper on the JsonReader's 'getBool()' func. 
     * 
     * @brief Get a boolean from the json
     * @param key: Key to read the json at
     * @param def: Default value returned if the key does not exist.
     */
    static bool getBool(const std::string& key, bool def = false) {
      return _config_json.getBool(key, def, Keys::SEPERATOR);
    }
};


#endif // CONFIG_HPP
