/*
 * TemplateApplicator.h
 *
 *  Created on: 9 Jul 2016
 *      Author: alanmc
 */

#ifndef TEMPLATEAPPLICATOR_H_
#define TEMPLATEAPPLICATOR_H_

#include <string>
#include <unordered_map>

namespace Diameter {
namespace Generator {

class TemplateApplicator {
public:
    TemplateApplicator(const std::string& templateDir, const std::string& templateFile, const std::string& outputDir);
    virtual ~TemplateApplicator();

    void applyTemplate(const std::unordered_map<std::string, std::string>& templateMap,
            const std::string& outputFile);

    static void replaceAll(std::string& str, const std::string& from, const std::string& to);

private:

    std::string m_templateFile;
    std::string m_outputDir;
};

} /* namespace Generator */
} /* namespace Diameter */

#endif /* TEMPLATEAPPLICATOR_H_ */
