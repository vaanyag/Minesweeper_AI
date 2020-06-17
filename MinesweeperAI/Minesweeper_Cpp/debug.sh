echo "easy or hard?"
read X
curr_dir=$PWD
end_dir=$(echo $PWD | tail -c 4)
if [[ $end_dir == *"src"* ]]; then
    cd ..
fi
pwd
make
cd ../WorldGenerator
if [[ $X == *"easy"* ]]; then
    sh generateSuperEasy.sh
    echo "Easy tournament generated"
else 
    sh generateTournament.sh
    echo "Hard tournament generated"
fi
cd ../Minesweeper_Cpp
./bin/Minesweeper -fd ../WorldGenerator/Problems
cd $curr_dir
