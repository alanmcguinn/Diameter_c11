/*
 * main.cpp
 *
 *  Created on: 8 Jul 2016
 *      Author: alanmc
 */

#include "Config.h"

#include "impl/RapidDictionaryParser.h"
#include "impl/MessageGenerator.h"
#include "Application.h"

#include <iostream>

int main(int argc, char** argv) {
    using Diameter::Generator::Config;
    using Diameter::Generator::RapidDictionaryParser;
    using Diameter::Generator::Application;
    using Diameter::Generator::MessageGenerator;

    Config config;

    config.parse(argc, argv);

    RapidDictionaryParser dictParser;
    dictParser.parse(config.getFile());

    //auto parserCtx = dictParser.parse();

    MessageGenerator msgGen(config);

    msgGen.generate(dictParser);

}


