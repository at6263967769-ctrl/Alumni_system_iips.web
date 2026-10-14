# Multi-stage Dockerfile for AlumniVerse C++ Crow Backend

# Stage 1: Build environment
FROM gcc:13-bookworm AS builder

RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake \
    ninja-build \
    libpthread-stubs0-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy backend source code and vendor libraries
COPY . .

# Build with CMake and Ninja
RUN cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
RUN ninja -C build alumniverse_backend graph_tests

# Run unit tests during container build
RUN ./build/graph_tests

# Stage 2: Minimal runtime image
FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    libstdc++6 \
    curl \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy compiled backend binary from builder
COPY --from=builder /app/build/alumniverse_backend /app/alumniverse_backend

# Set environment defaults
ENV PORT=8080
ENV JWT_SECRET="alumniverse_production_jwt_secret_key_2026"

EXPOSE 8080

HEALTHCHECK --interval=30s --timeout=5s --start-period=5s --retries=3 \
  CMD curl -f http://localhost:8080/health || exit 1

CMD ["/app/alumniverse_backend"]
