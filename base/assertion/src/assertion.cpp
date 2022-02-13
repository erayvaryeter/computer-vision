#include <logger/logger.h>
#include "assertion/assertion.h"

namespace base {

std::shared_ptr<base::Logger> Assertion::m_logger = std::make_shared<base::Logger>();

}
