//
// Created by serg on 10/15/16.
//

#include <cstdint>
#include <cstddef>
#include "WSServerImpl.h"

WSServerImpl::WSServerImpl(std::string endpoint, uint16_t port, size_t thread_count) : _server(port, thread_count) {

}

void WSServerImpl::run() {

}
