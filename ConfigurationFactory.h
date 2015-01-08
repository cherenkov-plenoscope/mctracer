//=================================
// include_guard
#ifndef __CONFIGURATION_FACTORY_H_INCLUDE__
#define __CONFIGURATION_FACTORY_H_INCLUDE__

//=================================
// forward_declared_dependencies

//=================================
// included_dependencies
#include "Functions.h"
#include "FileTools.h"
#include "TracerException.h"
//=================================
class ConfigurationFactory {
    const std::string default_config_path = "default_mctracer_config.xml";
public:
    void create_default_config_file();
private:
    void complain_default_config_must_not_be_used_when_no_config_specified()const;
    void quit_mctracer_and_inform_user_about_config_file_usage()const;
    void write_default_config_file()const;
};
#endif // __CONFIGURATION_FACTORY_H_INCLUDE__ 