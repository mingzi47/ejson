#!/bin/bash

# 格式化
clang-format -i `find include/ -type f -name *.hh`
clang-format -i `find src/ example/ test/ -type f -name *.cc`

# 统计代码行数
cloc --git `git branch --show-current`
