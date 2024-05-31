include_dir=./include
content_dir=./lib
cpp_standard=c++23

if [ -d './out' ]; then
  rm ./out/*
else
  mkdir ./out/
fi

g++ -std=${cpp_standard} -I${include_dir} ./lib/**/*.cpp "${content_dir}/main.cpp" -o ./out/pandemic
