FROM mcr.microsoft.com/dotnet/sdk:6.0-jammy

RUN apt-get update && apt-get install -y locales \
	&& localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8
ENV LANG en_US.utf8

RUN apt-get install build-essential libflatbuffers-dev -y
RUN apt-get install cmake wget sudo -y

RUN rm -rf /var/lib/apt/lists/*
WORKDIR /
