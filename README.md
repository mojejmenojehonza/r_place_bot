# r_place_bot
Automatic placer for r/slash.

# Installation
```
sudo apt install libopencv-dev
git clone https://github.com/mojejmenojehonza/r_place_bot
cd r_place_bot
g++ -o image_splitter image_splitter.cpp `pkg-config --cflags --libs opencv4`
npm install
```

# Start the server
```
PASSWORD=yourpassword node index.js
```
