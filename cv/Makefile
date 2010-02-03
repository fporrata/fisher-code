CVFILES = edge_detect.cpp

APP = edge

${APP}: ${CVFILES}
	g++ -bind_at_load `pkg-config --cflags opencv` ${CVFILES} -o ${APP} `pkg-config --libs opencv`
