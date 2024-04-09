#pragma once
#include <memory>

#define USING_SHARED_PTR(name) using name##Ref = std::shared_ptr<class name>;

USING_SHARED_PTR(Player)
USING_SHARED_PTR(Room)
