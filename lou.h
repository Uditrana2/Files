#ifndef GENCPP_COMM3_H
#define GENCPP_COMM3_H
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<atomic>
#include<omp.h>
#include"../graph.hpp"
#include"../atomicUtil.h"

#include <vector>
#include <unordered_map>
#include <algorithm>


auto LouvainCommunityDetection(graph& g);

#endif


