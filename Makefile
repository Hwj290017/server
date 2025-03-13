server:
	g++ src/util.cpp client.cpp -o client && \
	g++ -g src/util.cpp server.cpp src/Epoll.cpp src/LoopThread.cpp src/loopThreadPool.cpp src/Socket.cpp src/Channel.cpp src/connection.cpp src/EventLoop.cpp src/acceptor.cpp  src/Server.cpp -o server
clean:
	rm server && rm client