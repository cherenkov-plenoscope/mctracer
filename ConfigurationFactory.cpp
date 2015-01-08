#include "ConfigurationFactory.h"
//------------------------------------------------------------------------------
void ConfigurationFactory::create_default_config_file() {

	if(FileTools::can_be_opened(default_config_path)) {
		complain_default_config_must_not_be_used_when_no_config_specified();
	}else{
		write_default_config_file();
		quit_mctracer_and_inform_user_about_config_file_usage();
	}
}
//------------------------------------------------------------------------------
void ConfigurationFactory::complain_default_config_must_not_be_used_when_no_config_specified()const {
	std::stringstream info;
	info << "Mctracer was called without giving it a path to a config.xml file.\n";
	info << "Usually this is fine and a default config file is created in ";
	info << "the directory where mctracer was called and mctracer is ended.\n";
	info << "The Problem now is, that there already is a default config file ";
	info << "called: '" << default_config_path << "'.\n";
	info << "In order to not accidently use this default file and to not ";
	info << "override possible changes you have made in '";
	info << default_config_path<< "' mctracer is ended here.\n";
	info << "Do call mctracer again and give it a config file of yours.\n";
	info << "  -> you(at)your_machine: path/to/where/mctracer/shall/run$ ";
	info << "mctracer path/to/my/config.xml\n";	
	throw TracerException(info.str());		
}
//------------------------------------------------------------------------------
void ConfigurationFactory::write_default_config_file()const {

}
//------------------------------------------------------------------------------
void ConfigurationFactory::quit_mctracer_and_inform_user_about_config_file_usage()const {
	std::stringstream info;
	info << "Mctracer was called without giving it a path to a config.xml file.\n";
	info << "The configuration file is needed to tell mctracer how to run the ";
	info << "simulation.\n";
	info << "A default config file called '" << default_config_path << "' ";
	info << "has been created in the directory where mctracer was called.\n";
	info << "You may edit this config file to fit your demands.\n";
	info << "Do call mctracer again and give it a config file of yours.\n";
	info << "  -> you(at)your_machine: path/to/where/mctracer/shall/run$ ";
	info << "mctracer path/to/my/config.xml\n";	
	info << "You can also give it the default config just created here.\n";
	throw TracerException(info.str());	
}
//------------------------------------------------------------------------------