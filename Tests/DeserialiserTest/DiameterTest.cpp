/*
 * DiameterTest.cc
 *
 *  Created on: 9 Jul 2016
 *      Author: alanmc
 */

#include "Gx970_Dictionary.h"
#include "Gx970_Credit_ControlRequest.h"

#include "Serialiser.h"

#include <iostream>
#include <iomanip>

int main() {

    Diameter::Dictionary::DictionaryDefinition gx970Dictionary = Diameter::Dictionary::Gx970::makeDictionary();

    using namespace Diameter::Events::Gx970;

    Dia_Gx970_Credit_ControlRequest_Ptr msg;
    msg = makeDia_Gx970_Credit_ControlRequest();
    std::cout << "Command Code is " << msg->getHeader()->getCommandCode() << std::endl;
    gx970Dictionary.getMessage(Diameter::Events::getType(msg));

    msg->set_Origin_Host("fe.openet.com");
    msg->set_Origin_Realm("testrealm.openet.com");
    msg->set_Gx970_RAT_Type(DiaEnum_Gx970_RAT_Type::E_GAN);

    DiaAttr_Subscription_Id_Ptr subscriptionId = makeDiaAttr_Subscription_Id();

    subscriptionId->set_Subscription_Id_Type(DiaEnum_Subscription_Id_Type::E_END_USER_IMSI);

    msg->set_Subscription_Id(subscriptionId);

    using Diameter::Serialiser::Serialiser;
    using Diameter::Serialiser::Buffer;

    Buffer buffer = Serialiser::serialise(gx970Dictionary, msg);

    std::cout << buffer << std::endl;

//    buffer.reset();
    Diameter::Events::Gx970::Dia_Gx970_Credit_ControlRequest_Ptr msg2 = Serialiser::deserialise<Diameter::Events::Gx970::Dia_Gx970_Credit_ControlRequest_Ptr>(gx970Dictionary, buffer);

    std::cout << "Deserialised command code = " << msg2->getHeader()->getCommandCode() << std::endl;

    return 0;
}

