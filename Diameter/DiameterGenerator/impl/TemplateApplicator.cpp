/*
 * TemplateApplicator.cpp
 *
 *  Created on: 9 Jul 2016
 *      Author: alanmc
 */

#include "TemplateApplicator.h"

#include <iostream>
#include <fstream>
#include <algorithm>

namespace Diameter {
namespace Generator {

TemplateApplicator::TemplateApplicator(const std::string& templateDir, const std::string& templateFile, const std::string& outputDir) :
    m_templateFile(templateDir + "/" + templateFile), m_outputDir(outputDir) {
}

TemplateApplicator::~TemplateApplicator() {
}

void TemplateApplicator::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

void TemplateApplicator::applyTemplate(const std::unordered_map<std::string, std::string>& templateMap,
        const std::string& outputFile) {

    std::ifstream templateFileStream(m_templateFile);
    std::ofstream outputFileStream;
    outputFileStream.open(m_outputDir + "/" + outputFile, std::ofstream::out | std::ofstream::trunc);

    std::string templateLine;
    while (std::getline(templateFileStream, templateLine)) {
        std::for_each(std::begin(templateMap), std::end(templateMap), [&templateLine](const std::pair<std::string, std::string>& templateMapEntry) {
            replaceAll(templateLine, templateMapEntry.first, templateMapEntry.second);
        });
        outputFileStream << templateLine << std::endl;
    }
    templateFileStream.close();
    outputFileStream.close();
}

} /* namespace Generator */
} /* namespace Diameter */
