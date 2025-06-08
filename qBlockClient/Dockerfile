FROM ubuntu:22.04

# 设置环境变量
ENV DEBIAN_FRONTEND=noninteractive
ENV QT_VERSION=6.6.2
ENV QT_INSTALL_DIR=/opt/qt
ENV QT_MIRROR="https://mirrors.tuna.tsinghua.edu.cn/qt/"
ENV ICU_VERSION=56.2
ENV ICU_SRC_DIR=/tmp/icu-src

# 安装基础依赖（含编译工具）
RUN apt-get update && apt-get install -y \
    wget \
    build-essential \
    libgl1-mesa-dev \
    # 其他基础依赖...（保留之前已安装的依赖）
    libx11-dev \
    libxext-dev \
    libxfixes-dev \
    libxi-dev \
    libxrender-dev \
    libxcb1-dev \
    libx11-xcb-dev \
    libssl-dev \
    libxcursor-dev \
    libxcomposite-dev \
    libxdamage-dev \
    libxrandr-dev \
    libxtst-dev \
    libpulse-dev \
    libudev-dev \
    libasound2-dev \
    libgstreamer1.0-dev \
    libgstreamer-plugins-base1.0-dev \
    python3 \
    python3-pip \
    ninja-build \
    cmake \
    curl \
    unzip \
    xz-utils \
    libnss3-dev \
    libatk1.0-dev \
    libatk-bridge2.0-dev \
    libcups2-dev \
    libdrm-dev \
    libgbm-dev \
    libgtk-3-dev \
    libpango1.0-dev \
    libcairo2-dev \
    libxkbfile-dev \
    libxcb-icccm4-dev \
    libxcb-image0-dev \
    libxcb-shm0-dev \
    libxcb-sync0-dev \
    libxcb-shape0-dev \
    libxcb-render-util0-dev \
    libxcb-glx0-dev \
    libxkbcommon-dev \
    libxcb-xkb-dev \
    libxslt1-dev \
    mesa-common-dev \
    libglu1-mesa-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# ---------------------- 手动编译安装ICU 56 ---------------------- #
RUN mkdir -p $ICU_SRC_DIR && cd $ICU_SRC_DIR \
    # 下载ICU 56源代码
    && wget -q https://github.com/unicode-org/icu/releases/download/release-56-2/icu4c-56_2-src.tgz \
    && tar -xzf icu4c-56_2-src.tgz \
    && cd icu/source \
    # 配置编译参数（指定安装路径为/usr/local/icu-56）
    && ./configure --prefix=/usr/local/icu-${ICU_VERSION} \
    && make -j$(nproc) \
    && make install \
    # 清理临时文件
    && cd /tmp \
    && rm -rf $ICU_SRC_DIR

# 创建系统链接（关键步骤：将ICU 56库链接到系统默认路径）
RUN ln -s /usr/local/icu-${ICU_VERSION}/lib/libicui18n.so.56 /usr/lib/x86_64-linux-gnu/libicui18n.so.56 \
    && ln -s /usr/local/icu-${ICU_VERSION}/lib/libicuuc.so.56 /usr/lib/x86_64-linux-gnu/libicuuc.so.56 \
    && ln -s /usr/local/icu-${ICU_VERSION}/lib/libicudata.so.56 /usr/lib/x86_64-linux-gnu/libicudata.so.56

# 设置ICU库路径到环境变量
ENV LD_LIBRARY_PATH="/usr/local/icu-${ICU_VERSION}/lib:$LD_LIBRARY_PATH"

# 使用国内镜像源安装aqtinstall
RUN pip3 install -i https://pypi.tuna.tsinghua.edu.cn/simple --upgrade pip \
    && pip3 install -i https://pypi.tuna.tsinghua.edu.cn/simple aqtinstall

# 创建下载目录
RUN mkdir -p $AQT_DOWNLOAD_DIR $QT_INSTALL_DIR

# 使用正确的aqtinstall命令，增加重试机制，确保安装所有必要模块
RUN set -eux; \
    # 第一次尝试安装
    aqt install-qt linux desktop $QT_VERSION gcc_64 \
    -m qtwebengine qtwebchannel qtpositioning \
    -O $QT_INSTALL_DIR \
    --archives qtbase qtdeclarative qtwebengine qtwebchannel qtpositioning || \
    # 安装失败时，清理并重新尝试
    (rm -rf $QT_INSTALL_DIR/$QT_VERSION/gcc_64 && \
     aqt install-qt linux desktop $QT_VERSION gcc_64 \
     -m qtwebengine qtwebchannel qtpositioning \
     -O $QT_INSTALL_DIR \
     --archives qtbase qtdeclarative qtwebengine qtwebchannel qtpositioning); \
    chmod -R 755 $QT_INSTALL_DIR

# 设置Qt环境变量
ENV PATH="$QT_INSTALL_DIR/$QT_VERSION/gcc_64/bin:$PATH"
ENV LD_LIBRARY_PATH="$QT_INSTALL_DIR/$QT_VERSION/gcc_64/lib:$LD_LIBRARY_PATH"

WORKDIR /app
CMD ["/bin/bash"]