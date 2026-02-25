
  FROM quay.io/centos/centos:stream8

  # CentOS Stream 8 已于 2024 年 5 月 EOL，需切换到 vault
  RUN sed -i \
      -e 's|mirrorlist=|#mirrorlist=|g' \
      -e 's|#baseurl=http://mirror.centos.org/$contentdir/$stream|baseurl=https://vault.centos.org/centos/8-stream|g' \
      /etc/yum.repos.d/CentOS-Stream-*.repo

  RUN dnf install -y python38 python38-pip gcc python38-devel && \
      pip3.8 install --upgrade pip

  WORKDIR /dvt
  COPY requirements.txt .
  RUN pip3.8 install -r requirements.txt

  COPY . .

  # install.py 会 copytree 这两个目录，不存在会报错
  RUN mkdir -p Scripts Files

  CMD ["python3.8", "install.py"]
