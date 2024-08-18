shopt -s globstar
g++ -Wall -g **/*.cpp -o app.out -lboost_iostreams -lboost_system -lboost_filesystem -lcurl
