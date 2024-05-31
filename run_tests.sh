if [ -d './out' ]; then
  rm ./out/*
else
  mkdir ./out/
fi

/usr/bin/g++ -std=c++20 -I./include -I./tests/include ./lib/**/*.cpp ./tests/**/*.cpp ./tests/main.cpp -lunwind -o ./out/testable

./out/testable
