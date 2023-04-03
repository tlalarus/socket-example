//
// Created by minkyung on 23. 3. 29.
//

#ifndef SOCKET_MODEL_API_H
#define SOCKET_MODEL_API_H

#include "../include/protocol.h"

bool initEngine();
bool runEngine();
bool registerHandle(NotiCallbackFn pfunc);

#endif //SOCKET_MODEL_API_H
