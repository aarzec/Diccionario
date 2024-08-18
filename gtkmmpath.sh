pathstr=`pkg-config --cflags gstreamer-1.0`

IFS=' ' read -ra paths <<< "$pathstr"
for path in "${paths[@]}"; do
    if [[ $path == -I* ]]; then
        path=${path:2}
        echo \"$path\",
    fi
done




