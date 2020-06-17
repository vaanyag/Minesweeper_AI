echo "Dimensions and mine num:"
read X Y Z
curr_dir=$PWD
echo $PWD | tail -c 4 | read end_dir

if [[ $end_dir == *"src"* ]]; then
    cd ..
fi
pwd
make
cd ../WorldGenerator/
rm Problems/*
python3 WorldGenerator.py 1000 customMedium $X $Y $Z
cd ../Minesweeper_Cpp
echo "Debug?"
read A
start=`date +%s`
if [[ $A == *"N"* ]]; then
    ./bin/Minesweeper -fv ../WorldGenerator/Problems
else
    ./bin/Minesweeper -fdv ../WorldGenerator/Problems
fi
end=`date +%s`
echo EXECUTION TIME: `expr $end - $start` SECONDS
cd $curr_dir
