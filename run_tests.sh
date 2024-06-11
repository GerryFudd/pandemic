if [ -d './out' ]; then
  rm ./out/*
else
  mkdir ./out/
fi

/usr/bin/g++ -std=c++20 -I./include -I./tests/include ./lib/**/*.cpp ./lib/*.cpp ./tests/**/*.cpp ./tests/*.cpp -lunwind -o ./out/testable

./out/testable
