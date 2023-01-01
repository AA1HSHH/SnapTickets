# 基于muduo库httpserver搭建
环境：Ubuntu 18.04.6 LTS
## 安装muduo库
1.下载muduo库2.0.2
https://github.com/chenshuo/muduo/releases/tag/v2.0.2

2.安装
```
sudo apt install g++ cmake make libboost-dev
tar zxf muduo-2.0.2.tar.gz
cd muduo-2.0.2
BUILD_TYPE=release  ./build.sh -j2
BUILD_TYPE=release  ./build.sh install
```
3.验证
```
unzip muduo-tutorial-master.zip
cd muduo-tutorial-master
mkdir build
cd build
cmake ../cmake
make
```
需要改下CMakeLists.txt里边的MUDUO_PATH 我改为`"/path/to/release-install-cpp11"`
可以顺利编译运行
参考https://blog.csdn.net/weixin_42405632/article/details/88057204

为了include没有下划线
参考https://blog.csdn.net/weixin_60208935/article/details/127562693
```
cd /path/to/release-install-cpp11
cd include/
cp -r muduo/ /usr/include/
cd ..
cd lib/
cp * /usr/local/lib/
```
## httpserver
完全参考muduo-2.0.2/muduo/net/http/tests
g++ HttpServer.cc -o server -lmuduo_net -lmuduo_base -lmuduo_http -lpthread

## 测试
### 安装ab
```
apt-get install apache2-utils 
ab -V
```
## 记录
```
ab -n 1000000 -c 600  http://127.0.0.1:8000/
This is ApacheBench, Version 2.3 <$Revision: 1807734 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 127.0.0.1 (be patient)
apr_socket_recv: Connection reset by peer (104)
Total of 88528 requests completed
```

