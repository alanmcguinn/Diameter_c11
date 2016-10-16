/*
 * Generator.h
 *
 *  Created on: 9 Jul 2016
 *      Author: alanmc
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_

namespace Diameter {
namespace Generator {

class Generator {
public:
	virtual ~Generator() {}

	virtual void generate(DictionaryParser& dictParser) = 0;
};

}
}



#endif /* GENERATOR_H_ */
