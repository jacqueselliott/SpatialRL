#!/bin/bash

function isWindows() {
  [[ ! ("$(uname -s)" == "Darwin" || "$(uname -s)" == "Linux") ]];
}

if isWindows; then
  cmake -G "Visual Studio 14 2015 Win64" ..
else
  cmake ..
fi