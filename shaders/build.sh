#!/usr/bin/bash

set -xe

slangc -o slang.spv -entry vertMain -entry fragMain -target spirv shader.slang
