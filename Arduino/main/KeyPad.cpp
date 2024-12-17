#include "KeyPadUtils.h"

String getKey() {
    return String(myKeypad.getKey());
}