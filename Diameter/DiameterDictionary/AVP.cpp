#include "AVP.h"

namespace Diameter {
namespace Dictionary {

AVP makeAVP(const std::string& name, std::uint32_t avpCode, std::uint32_t vendorId, Mandatory mbit, Protected pbit) {
   return std::make_shared<AVPRaw>(name, avpCode, vendorId, mbit, pbit);
}

}
}
