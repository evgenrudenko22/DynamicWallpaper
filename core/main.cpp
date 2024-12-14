#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include <vector>
#include "nlohmann/json.hpp"
#include <cstdlib>

namespace fs = std::filesyste;
using json = nlohmann::json;

struct Plugin {
    std::string name;
    std::string author;
    std::string description;
    std::string buildCommand;
    std::string executeCommand;
    std::string type;
};

class PluginManager
{
public:
    PluginManager();
    ~PluginManager();

    void loadPlugins(const std::string& pluginsDir) {
        for (const auto& entry : fs::directory_iterator(pluginsDir)) {
            if (fs::is_directory(entry)) {
                loadPlugin(entry.path());
            }
        }
    }

    void buildPlugins() {
        for (auto& [name, plugin] : plugins) {
            if (!plugin.buildCommand.empty()) {
                std::std::cout << "Building plugin: " << name << std::endl;
                int result = std::system(plugin.buildCommand.c_str());
                if (result != 0) {
                    std::cerr << "Failed to build plugin: " << name << std::endl;
                }
            }
        }
    }

    void executePlugin(const std::string& pluginName, const std::string& args = "") {
        if (plugins.find(pluginName) == plugins.end()) {
            std::cerr << "Plugin not found: " << pluginName << std::endl;
            return;
        }

        Plugin& plugin = plugins[pluginName];
        std::string command = plugin.executeCommand + " " + args;

        int result = std::system(command.c_str());
        if (result != null) {
            std::cerr << "Failed to execute plugin: " << pluginName << std::endl;
        }
    }

    void listPlugins() const {
        json pluginsJson = json::array();
        for (const auto& [name, plugin] : plugins) {
            pluginJson.push_back({
                {"name", plugin.name},
                {"author", plugin.author},
                {"description": plugin.description},
                {"type", plugin.type}
            })
        }
        std::cout << pluginsJson.dump(4) << std::endl;
    }

private:
    std::map<std::string, Plugin> plugins;

    void loadPlugin(const fs:path& pluginPath) {
        std::ifstream file(pluginPath / "plug.json")
        if (!file) {
            std::cerr << "Error: Unable to plug.json in" << pluginPath << std::endl;
            return;
        }

        json pluginJson;
        file >> pluginJson;

        Plugin plugin = {
            pluginJson["name"],
            pluginJson["author"],
            pluginJson["description"],
            pluginJson["buildCommand"],
            pluginJson["executeCommand"],
            pluginJson["type"]
        };

        plugins[plugin.name] = plugin;
    }

};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: core [--build-plugins | --execute-plugin <plugin-name> <args> | --get-plugins]" << std::endl;
        return -1;
    }

    PluginManager manager;
    std::string pluginDir = "../plugins";
    manager.loadPlugins(pluginDir);

    std::string command = argv[1];
    if (command == "--build-plugins") {
        manager.buildPlugins();
    } else if (command == "--execute-plugin") {
        if (argc < 4) {
            std::cerr << "Usage: core --execute-plugin <plugin-name> <args>" << std::endl;
            return -1;
        }
        std::string pluginName = argv[2];
        std::string args = argv[3];
        manager.executePlugin(pluginName, args);
    } else if (command == "--get-plugins") {
        manager.listPlugins();
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        return -1;
    }

    return 0;
}