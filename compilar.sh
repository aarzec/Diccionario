shopt -s globstar
g++ -Wall -g **/*.cpp -o app.out `pkg-config gtkmm-3.0 --cflags --libs` `pkg-config gstreamer-1.0 --cflags --libs` -lboost_iostreams -lboost_system -lboost_filesystem -lcurl