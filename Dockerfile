# Stage 1: Build
FROM alpine:3.19 AS builder

RUN apk add --no-cache \
    g++ \
    cmake \
    ninja \
    make

WORKDIR /build
COPY CMakeLists.txt .
COPY src/ src/

RUN cmake -B build -G Ninja \
    -DBUILD_TESTS=OFF \
    -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build

# Stage 2: Runtime
FROM alpine:3.19

RUN apk add --no-cache libstdc++

WORKDIR /app
COPY --from=builder /build/build/app .

EXPOSE 8080
CMD ["./app"]
