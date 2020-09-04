#pragma once

#define _CRT_SECURE_NO_WARNINGS

/**
* warning C4244: conversion from 'float' to 'int', possible loss of data
*/
#pragma warning(disable : 4244)

#include "Win.h"
#include <windowsx.h>


#pragma comment(lib, "d2d1.lib")
#include <d2d1.h>
#include <wrl.h>

/**
* Database Management System
*/
#include <QRD/QRD.h>


#include <vector>
#include <string>
#include <queue>
#include <bitset>
#include <optional>
#include <functional>
#include <future>
#include <sstream>

