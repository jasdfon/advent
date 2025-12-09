for dir in ./*/
do
    if [ -f "$dir/main.c" ]; then
        echo "running input for $dir";
        cd $dir;
        gcc -o ./main.bin main.c -lm && ./main.bin example.txt
        cd ..;
        echo "";
    fi
done
