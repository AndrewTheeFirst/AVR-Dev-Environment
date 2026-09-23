FROM --platform=linux/amd64 ubuntu:24.04

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Etc/UTC \
    LANG=C.UTF-8 \
    LC_ALL=C.UTF-8

# 1. Install host build tools, QoL utilities, and AVR flashing software (avrdude)
RUN apt-get update && apt-get install -y --no-install-recommends \
        ca-certificates wget curl \
        make cmake ninja-build \
        git vim less file \
        avrdude usbutils udev \
    && rm -rf /var/lib/apt/lists/*

# Download & Extract AVR toolchain
ENV AVR_TOOLCHAIN_DIR=/opt/avr8-gnu-toolchain
RUN wget -q https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/SoftwareTools/avr8-gnu-toolchain-4.0.0.52-linux.any.x86_64.tar.gz -O /tmp/avr-toolchain.tar.gz && \
    mkdir -p ${AVR_TOOLCHAIN_DIR} && \
    tar -xzf /tmp/avr-toolchain.tar.gz -C ${AVR_TOOLCHAIN_DIR} --strip-components=1 && \
    rm /tmp/avr-toolchain.tar.gz

# Add toolchain to PATH
ENV PATH="${AVR_TOOLCHAIN_DIR}/bin:${PATH}" 

# 4. Set up the non-root developer user to prevent permission issues with mounts
ARG UNAME=dev
ARG UID=1000
ARG GID=1000

RUN if ! getent group ${GID} >/dev/null; then groupadd -g ${GID} ${UNAME}; fi && \
    if ! getent passwd ${UID} >/dev/null; then \
        useradd -m -u ${UID} -g ${GID} -s /bin/bash ${UNAME}; \
    else \
        existing=$(getent passwd ${UID} | cut -d: -f1); \
        usermod -l ${UNAME} -d /home/${UNAME} -m ${existing} || true; \
    fi

WORKDIR /workspace
USER ${UNAME}

CMD ["bash"]