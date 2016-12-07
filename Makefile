CC = g++
PLATFORM = Linux-amd64-64
CPPFLAGS = -m64 -I$(HADOOP_INSTALL)/include

REMOTE_USERNAME = hduser
REMOTE_HOST = 167.205.35.25
REMOTE_DIRECTORY = ~/sasuga

all: compile run

run: start.sh
	./start.sh 2> error.log > output.log &

compile: init.cpp pr.cpp fin.cpp
	$(CC) $(CPPFLAGS) init.cpp -Wall -L$(HADOOP_INSTALL)/lib/native -lhadooppipes \
	-lssl -lcrypto -lhadooputils -lpthread -g -O2 -o init
	$(CC) $(CPPFLAGS) pr.cpp -Wall -L$(HADOOP_INSTALL)/lib/native -lhadooppipes \
	-lssl -lcrypto -lhadooputils -lpthread -g -O2 -o pr
	$(CC) $(CPPFLAGS) fin.cpp -Wall -L$(HADOOP_INSTALL)/lib/native -lhadooppipes \
	-lssl -lcrypto -lhadooputils -lpthread -g -O2 -o fin

deploy:
	scp -r * $(REMOTE_USERNAME)@$(REMOTE_HOST):$(REMOTE_DIRECTORY)
