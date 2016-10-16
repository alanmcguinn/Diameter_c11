/*
 * Config.cpp
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#include "Config.h"

#include <tclap/CmdLine.h>
#include <tclap/ValueArg.h>
#include <tclap/SwitchArg.h>

namespace Diameter {
namespace Generator {

Config::Config() : m_dontGenerateBaseAVPs(false) {
}

Config::~Config() {
}

bool Config::parse(int argc, char** argv) {
    using TCLAP::ValueArg;
    using TCLAP::SwitchArg;
    using TCLAP::CmdLine;
    using TCLAP::ArgException;

    try {

        ValueArg<std::string> brandingArg("b", "branding", "Branding string (e.g. Gx840)", false, "", "String");
        ValueArg<std::string> fileArg("f", "file", "File", false, "dictionary.xml", "Dictionary File");
        ValueArg<std::string> outputDirArg("o", "outputdir", "OutputDirectory", false, ".", "Output Directory");
        ValueArg<std::string> templateDirArg("t", "templates", "Template Directory", false, ".", "Directory");
        SwitchArg dontGenerateBase("n", "nobase", "Do not generate Base AVPs");

        CmdLine cmdLine("DiameterGenerator", ' ', "0.1");
        cmdLine.add(brandingArg);
        cmdLine.add(fileArg);
        cmdLine.add(outputDirArg);
        cmdLine.add(templateDirArg);
        cmdLine.add(dontGenerateBase);

        cmdLine.parse(argc, argv);

        m_branding = brandingArg.getValue();
        m_file = fileArg.getValue();
        m_outputDir = outputDirArg.getValue();
        m_templateDir = templateDirArg.getValue();
        m_dontGenerateBaseAVPs = dontGenerateBase.getValue();

    } catch (ArgException& ae) {
        std::cerr << "Failure parsing command line arguments: " << ae.error() << std::endl;
        return false;
    }

    return true;
}

const std::string& Config::getBranding() const {
    return m_branding;
}

const std::string& Config::getFile() const {
	return m_file;
}

const std::string& Config::getOutputDir() const {
    return m_outputDir;
}

const std::string& Config::getTemplateDir() const {
	return m_templateDir;
}

bool Config::willGenerateBaseAVPs() const {
    return !m_dontGenerateBaseAVPs;
}

} /* namespace Generator */
} /* namespace Diameter */

