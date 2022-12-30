#pragma once

#include "wrapsock.h"

const char *GetBlock(int sockfd); 
void PutBlock(int sockfd, const char *message); 
