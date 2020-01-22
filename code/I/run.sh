rm good bad
${CXX} -Wall -O3 bad.cpp -I./ -o bad
${CXX} -Wall -O3 good.cpp -I./ -o good
echo "** Running bad example..."
./bad
echo "** Running good example..."
./good
