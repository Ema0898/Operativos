sudo apt-get install -y libfreetype6
sudo apt-get install -y freetype2-demos
sudo apt-get install -y libfreetype6-dev

echo "LIBFREETYPE2 INSTALLED";

sudo apt-get install libconfig-dev

echo "LIBCONFIG INSTALLED";

cd ./extras
unzip SDL2-2.0.12.zip
unzip SDL2_image-2.0.5.zip
unzip SDL2_ttf-2.0.15.zip

cd SDL2-2.0.12
./configure 
make
sudo make install
cd ..

echo "SDL INSTALLED";

cd SDL2_image-2.0.5
./configure
make
sudo make install
cd ..

echo "SDL IMAGE INSTALLED";

cd SDL2_ttf-2.0.15
./configure 
make
sudo make install
cd ..

echo "SDL TTF INSTALLED";

echo "DONE";