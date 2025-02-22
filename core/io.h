#pragma once
#include <iostream>

#ifndef IO_H
#define IO_H
#include "tree.h"
tree::deep_desition get_args(const std::string& filename);
void saveResult(const rects_t& results);
#endif