CC = g++
PLATFORM = Linux-amd64-64
CPPFLAGS = -m64 -I$(HADOOP_INSTALL)/include

all: compile run

run:
	./start.sh 2> error.log > output.log &

compile: init.cpp pr.cpp
	$(CC) $(CPPFLAGS) init.cpp -Wall -L$(HADOOP_INSTALL)/lib/native -lhadooppipes \
	-lssl -lcrypto -lhadooputils -lpthread -g -O2 -o init
	$(CC) $(CPPFLAGS) pr.cpp -Wall -L$(HADOOP_INSTALL)/lib/native -lhadooppipes \
	-lssl -lcrypto -lhadooputils -lpthread -g -O2 -o pr
	$(CC) $(CPPFLAGS) fin.cpp -Wall -L$(HADOOP_INSTALL)/lib/native -lhadooppipes \
	-lssl -lcrypto -lhadooputils -lpthread -g -O2 -o fin

deploy:
	scp -r * hduser@167.205.35.25:~/sasuga
