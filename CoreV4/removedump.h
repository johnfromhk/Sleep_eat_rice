#pragma once
#include "shared_uorder_set.h"
#include "vec.h"
#include "vechash.h"
typedef shared_unordered_set<graph::vec2_t, graph::MatrixHash> dump_t;
extern dump_t* matrix_set; //记忆内容 去重用
