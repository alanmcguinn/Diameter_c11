/*
 * Config.h
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>

namespace Diameter {
namespace Generator {

class Config {
public:
    Config();
    virtual ~Config();

    bool parse(int argc, char** argv);

    const std::string& getBranding() const;
    const std::string& getFile() const;
    const std::string& getOutputDir() const;
    const std::string& getTemplateDir() const;
    bool willGenerateBaseAVPs() const;

private:
    std::string m_branding;
    std::string m_file;
    std::string m_outputDir;
    std::string m_templateDir;
    bool m_dontGenerateBaseAVPs;
};

} /* namespace Generator */
} /* namespace Diameter */

#endif /* CONFIG_H_ */
