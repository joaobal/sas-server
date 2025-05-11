# Use a recent and stable version of Ubuntu as the base image
# Ubuntu 22.04 LTS (Jammy Jellyfish) is a good choice for stability and support.
FROM ubuntu:22.04

# Set DEBIAN_FRONTEND to noninteractive to prevent interactive prompts during package installation.
ENV DEBIAN_FRONTEND=noninteractive

# Install common Linux tools and utilities
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    ca-certificates \
    curl \
    wget \
    git \
    nano \
    net-tools \
    procps \
    unzip \
    software-properties-common \
    # Clean up apt cache to reduce image size
    && apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Install C++ build tools
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential \
    g++ \
    make \
    cmake \
    pkg-config \
    # Clean up apt cache
    && apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Install FFmpeg (runtime)
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    ffmpeg \
    # Clean up apt cache
    && apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Install GStreamer (runtime plugins and development libraries)
# Also install Xvfb for running autovideosink in a headless environment
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    gstreamer1.0-tools \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly \
    gstreamer1.0-libav \
    gstreamer1.0-gtk3 \
    gstreamer1.0-qt5 \
    gstreamer1.0-gl \
    gstreamer1.0-vaapi \
    libgstreamer1.0-dev \
    libglib2.0-dev \
    libgstreamer-plugins-base1.0-dev \
    libgstreamer-plugins-good1.0-dev \
    libgstreamer-plugins-bad1.0-dev \
    xvfb \
    # Clean up apt cache
    && apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# --- Application Specific Setup ---
# Create a working directory for your application
WORKDIR /app

# Copy the C++ GStreamer test application source code
COPY src/gstreamer_test.cpp .

# Compile the C++ GStreamer test application
# The command uses pkg-config to get the correct compiler and linker flags for GStreamer.
RUN g++ gstreamer_test.cpp -o gstreamer_test $(pkg-config --cflags --libs gstreamer-1.0 glib-2.0)

# Expose any ports your final sas_server application might need (placeholder)
# EXPOSE 8000

# Define the default command to run when the container starts.
# This will run the compiled GStreamer C++ test application using Xvfb.
# Xvfb creates a virtual X server, which can help autovideosink initialize
# without a physical display. You won't see visual output from `docker logs`,
# but the application should run without crashing due to display errors.
CMD ["xvfb-run", "-a", "./gstreamer_test"]

# --- End of Application Specific Setup ---
