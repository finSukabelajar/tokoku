FROM gcc:13
WORKDIR /app
COPY main.cpp logic.h httplib.h ./
COPY public ./public
RUN g++ -std=c++17 main.cpp -o tokoku -pthread
CMD ["./tokoku"]